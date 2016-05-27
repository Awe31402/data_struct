#include "base.h"
#include "list.h"
#include "list_node.h"
#include <stdio.h>

typedef struct _poly_item poly_item;
typedef struct _iplynomial ipolynomial;
typedef struct _polynomial polynomial;

void poly_item_destructor(poly_item *ptr);

struct _poly_item {
	int coef;
	int expo;
	list_node *node;
};

struct _iplynomial {
	int (*add_item) (polynomial*, poly_item* i);
	void (*print) (polynomial*);
	int (*get_degree)(polynomial*);
};

struct _polynomial {
	list *list;

	ipolynomial *ops;
};

void print_impl(polynomial* p)
{
	list_node* curr = p->list->ops->get_head(p->list);
	poly_item* item;
	while (curr != p->list->root) {
		item = curr->ops->get_data(curr);
		printf("(%d)x^%d +",item->coef, item->expo);
		curr = curr->next;
	}
	printf("\n");
}


int add_item_impl(polynomial* p, poly_item* i)
{
	list_node *curr;
	poly_item* data;

	if (unlikely(!p || !i))
		return FAILED;

	if (p->list->size == 0) {
		p->list->ops->append(p->list, i->node);
	} else {
		curr = p->list->ops->get_head(p->list);
		for (; curr != p->list->root; curr = curr->next) {
			data = curr->ops->get_data(curr);
			if (data->expo < i->expo) {
				p->list->ops->insert(p->list, curr, i->node);
				return SUCCESS;
			} else if (data->expo == i->expo) {
				data->coef = i->coef;
				delete(poly_item, i);
				return SUCCESS;
			}
		}
		p->list->ops->append(p->list, i->node);
		return SUCCESS;
	}
	return FAILED;
}

ipolynomial poly_ops = {
	.add_item = add_item_impl,
	.print = print_impl,
};

poly_item* poly_item_constructor(int c, int e)
{
	if (unlikely(e < 0))
		return NULL;
	poly_item* ret = malloc(sizeof(poly_item));
	if (unlikely(!ret))
		return NULL;
	ret->coef = c;
	ret->expo = e;
	ret->node = new (list_node, ret);
	return ret;
}

void poly_item_destructor(poly_item *ptr)
{
	list_node* node = ptr->node;
	node->next->prev = node->prev;
	node->prev->next = node->next;
	delete(list_node, node);
	free(ptr);
}

polynomial* polynomial_constructor()
{
	polynomial* ret = malloc(sizeof(polynomial));
	
	if (unlikely(!ret))
		return NULL;
	
	ret->list = new (list);
	ret->ops = &poly_ops;
	return ret;
}

void polynomial_destructor(polynomial* ptr)
{
	list_node* curr = ptr->list->ops->get_head(ptr->list);
	list_node* prev;
	while (curr != ptr->list->root) {
		curr = curr->next;
		prev = curr->prev;
		ptr->list->ops->remove(ptr->list, prev);
		delete(poly_item, prev->ops->get_data(prev));
	}
	delete(list, ptr->list);
	free(ptr);
}

int main()
{
	polynomial* poly = new (polynomial);
	poly->ops->add_item(poly, new(poly_item, 3, 5));
	poly->ops->add_item(poly, new(poly_item, 2, 4));
	poly->ops->add_item(poly, new(poly_item, 1, 7));
	poly->ops->add_item(poly, new(poly_item, 4, 8));
	poly->ops->add_item(poly, new(poly_item, 9, -1));
	poly->ops->print(poly);
	delete(polynomial, poly);
	return 0;
}
