#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sparse.h"
#include "base.h"
#define SUCCESS 1
#define FAILED  0

#define IS_EQUAL 0
#define IS_SMALL -1
#define IS_LARGE 1

static int comp_row(sparse_term a, sparse_term b) {
	int ret = a.row - b.row;
	if (!ret)
		return IS_EQUAL;
	else
		return (ret > 0) ? IS_LARGE : IS_SMALL;
}

static int comp_col(sparse_term a, sparse_term b) {
	int ret = a.col - b.col;
	if (!ret)
		return IS_EQUAL;
	else
		return (ret > 0) ? IS_LARGE : IS_SMALL;
}

static int sparse_add_impl (sparse_matrix* i1, sparse_matrix* i2,
		sparse_matrix *o1)
{
	int index1 = 0, index2 = 0;

	if ((i1->row != i2->row) || (i1->col != i2->col))
		return FAILED;

	o1->row = i1->row;
	o1->col = i1->col;
	o1->terms_num = 0;

	while((index1 < i1->terms_num) && (index2 < i2->terms_num)) {
		switch(comp_row(i1->data[index1], i2->data[index2])) {
		case IS_SMALL:
			o1->data[o1->terms_num++] = i1->data[index1++];
			break;
		case IS_LARGE:
			o1->data[o1->terms_num++] = i2->data[index2++];
			break;
		case IS_EQUAL:
			switch (comp_col(i1->data[index1], i2->data[index2])) {
			case IS_SMALL:
				o1->data[o1->terms_num++] = i1->data[index1++];
				break;
			case IS_LARGE:
				o1->data[o1->terms_num++] = i2->data[index2++];
				break;
			case IS_EQUAL:
				o1->data[o1->terms_num] = i2->data[index2++];
				o1->data[o1->terms_num++].value += i1->data[index1++].value;
				break;
			}
			break;
		}
	};

	while (index1 < i1->terms_num)
		o1->data[o1->terms_num++] = i1->data[index1++];
	while (index2 < i2->terms_num)
		o1->data[o1->terms_num++] = i2->data[index2++];
	return SUCCESS;
}

static inline int sparse_multiply_impl (sparse_matrix* i1, sparse_matrix* i2,
		sparse_matrix *o1);

static void clear_impl(sparse_matrix *m)
{
	m->row = m->col = m->terms_num = 0;
}

static inline void swap_term(sparse_term *a, sparse_term *b)
{
	sparse_term tmp;
	tmp = *b;
	*b = *a;
	*a = tmp;
}

static void sparse_print_impl(sparse_matrix *m)
{
	int i;

	printf("sparse_matrix: %p\n", m);
	printf("row: %4d, col: %4d, terms: %4d\n",
				m->row, m->col, m->terms_num);
	printf("----------------------------------\n");

	for (i = 0; i < m->terms_num; i++) {
		printf("row: %4d, col: %4d, value: %4d\n",
				m->data[i].row, m->data[i].col, m->data[i].value);
	}
	printf("\n");
}

static int sparse_set_element_impl(sparse_matrix* m, const int r, const int c,
		const int v)
{
	int i;
	sparse_term swap;

	if ((r <= 0 || c  <= 0 || v == 0)) return FAILED;

	if ((r > m->row) || (c > m->col) ||
			(m->terms_num == MAX_TERMS))
		return FAILED;

	swap.col = c,
	swap.row = r,
	swap.value = v;

	for (i = 0; i < m->terms_num; i++) {
		if (m->data[i].row == swap.row) {
			if (m->data[i].col == swap.col) {
				m->data[i].value = swap.value;
				return SUCCESS;
			} else if (swap.col < m->data[i].col) {
				swap_term(&m->data[i], &swap);
			}
		} else if (swap.row < m->data[i].row) {
			swap_term(&m->data[i], &swap);
		}
	}

	m->data[m->terms_num++] = swap;
	return SUCCESS;
}

static int sparse_transpose_impl(sparse_matrix* i, sparse_matrix* o) {
	int index, jndex;
	int current = 0;
	if (!o) return FAILED;

	o->row = i->col;
	o->col = i->row;
	o->terms_num = i->terms_num;
	
	/* if i has more than one non zero terms */
	if (o->terms_num) {
		for (index = 1; index <= i->col; index++) {
			for (jndex = 0; jndex < i->terms_num; jndex++) {
				if (i->data[jndex].col == index) {
					o->data[current].row = i->data[jndex].col;
					o->data[current].col = i->data[jndex].row;
					o->data[current].value = i->data[jndex].value;
					current++;
				}
			}
		}
	}
	return SUCCESS;
}

static void q_sort(sparse_term a[], int low, int high) {
	if(low >= high) return;

	int i, l;

	sparse_term pivot = a[low];
	l = low;
	for (i = l; i < high; i++) {
		if (a[i].row < pivot.row) {
			l++;
			swap_term(&a[i], &a[l]);
		}
	}
	swap_term(&a[low], &a[l]);

	q_sort(a, low, l-1);
	q_sort(a, l+1, high);
}

static int sparse_transpose_qs_impl(sparse_matrix* i, sparse_matrix* o) {	
	int index, tmp;
	if (!o) return FAILED;

	o->row = i->col;
	o->col = i->row;
	o->terms_num = i->terms_num;

	memcpy(o->data, i->data, i->terms_num * sizeof(sparse_term));
	for (index = 0; index < i->terms_num; index++) {
		tmp = o->data[index].col;
		o->data[index].col =  o->data[index].row;
		o->data[index].row = tmp;
	}

	q_sort(o->data, 0, i->terms_num);
	return SUCCESS;
}

static int sparse_transpose_setele_impl(sparse_matrix* i, sparse_matrix* o) {
	int index;
	if (!o) return FAILED;
	
	o->row = i->col;
	o->col = i->row;

	for (index = 0; index < i->terms_num; index++)
		o->ops->set_element(o, i->data[index].col,
				i->data[index].row, i->data[index].value);

	return SUCCESS;
}

static IMatrix sparse_ops = {
	.transpose = sparse_transpose_qs_impl,
	.print = sparse_print_impl,
	.set_element = sparse_set_element_impl,
	.clear = clear_impl,
	.add = sparse_add_impl,
	.multiply = NULL,
};

sparse_matrix* sparse_matrix_constructor(void *addr, int r, int c)
{
	if (!addr) return NULL;
	sparse_matrix* ret = (sparse_matrix*) addr;

	ret->row = r;
	ret->col = c;
	ret->terms_num = 0;
	ret->ops = &sparse_ops;
	memset(ret->data, 0, MAX_TERMS * sizeof(int));
	return ret;
}

void sparse_matrix_destructor(sparse_matrix* ptr)
{
	free(ptr);
}
