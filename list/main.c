#include "base.h"
#include "list.h"
#include "list_node.h"
#include <stdio.h>
#include <string.h>
#include <obj.h>

/*
 * This is an example to show how to use
 * list & list_node classes and it's related
 * interface by a polynomial example
 */
typedef struct _poly_item poly_item;
typedef struct _iplynomial ipolynomial;
typedef struct _polynomial polynomial;

struct _poly_item {
	int coef;
	int expo;
	list_node *node;
};

struct _iplynomial {
	int (*set_item) (polynomial*, poly_item* i);
	void (*print) (polynomial*);
	int (*get_degree)(polynomial*);
};

struct _polynomial {
	list *list;

	ipolynomial *ops;
};

int get_degree_impl(polynomial *p)
{
	if (p->list->size == 0)
		return -1;

	ilist *list_interface = p->list->ops;
	ilist_node *node_interface = p->list->root->ops;

	poly_item *data = node_interface->get_data(list_interface->get_head(p->list));
	return (data)? data->expo : -1;
}

void print_impl(polynomial* p)
{
	if (unlikely(!p))
		return;
	printf("f(x) = ");

	if (p->list->size == 0) {
		printf("none\n");
		return;
	}

	list_node* curr = p->list->ops->get_head(p->list);
	poly_item* item;
	while (curr != p->list->root) {
		item = curr->ops->get_data(curr);
		printf("(%d)x^%d %c",item->coef, item->expo,
				(curr->next == p->list->root) ? '\0':'+');
		curr = curr->next;
	}
	printf("\n");
}


int set_item_impl(polynomial* p, poly_item* i)
{
	list_node *curr;
	poly_item* data;

	if (unlikely(!p || !i))
		return FAILED;

	ilist* list_interface = p->list->ops;
	ilist_node* node_interface = i->node->ops;

	printf("set_item_impl: coef= %d, expo = %d\n",
			i->coef, i->expo);

	if (p->list->size == 0) {
		list_interface->append(p->list, i->node);
	} else {
		curr = list_interface->get_head(p->list);
		for (; curr != p->list->root; curr = curr->next) {
			data = node_interface->get_data(curr);
			if (data->expo < i->expo) {
				list_interface->insert(p->list, curr, i->node);
				return SUCCESS;
			} else if (data->expo == i->expo) {
				data->coef = i->coef;
				delete(i);
				return SUCCESS;
			}
		}
		list_interface->append(p->list, i->node);
		return SUCCESS;
	}
	return FAILED;
}

ipolynomial poly_ops = {
	.set_item = set_item_impl,
	.print = print_impl,
	.get_degree = get_degree_impl,
};

static int polynomial_clear_impl(list* l)
{
	if (unlikely(!l))
		return FAILED;

	if (l->size == 0)
		goto done;

	ilist *list_interface = l->ops;

	list_node* curr = list_interface->get_head(l);
	list_node* prev;
	while (curr != l->root) {
		prev = curr;
		curr = curr->next;
		list_interface->remove(l, prev);
		delete(prev->ops->get_data(prev));
	}
done:
	return SUCCESS;
}

void poly_item_destructor(void *ptr);

poly_item* poly_item_constructor(int c, int e)
{
	if (unlikely(e < 0 || !c))
		return NULL;

	Obj* addr = malloc(sizeof(poly_item) + sizeof(Obj));
	if (unlikely(!addr))
		return NULL;

	addr->ref_count = 1;
	addr->destructor = poly_item_destructor;

	poly_item* ret = (poly_item*)((char*)addr + sizeof(Obj));
	ret->coef = c;
	ret->expo = e;
	ret->node = new (list_node, ret);
	return ret;
}

void poly_item_destructor(void *ptr)
{
	list_node* node = ((poly_item*) ptr)->node;
	node->next = node;
	node->prev = node;
	delete(node);
}

static ilist poly_list_ops;

static ilist* copy_list_interface(ilist *ops)
{
	static int done = 0;
	ilist *ret = &poly_list_ops;
	if (unlikely(done == 0)) {
		*ret = *ops;
		done = 1;
	}
	return ret;
}

void polynomial_destructor(void* ptr);

polynomial* polynomial_constructor()
{
	Obj* obj = malloc(sizeof(polynomial) + sizeof(Obj));

	if (unlikely(!obj))
		return NULL;
	obj->ref_count = 1;
	obj->destructor = polynomial_destructor;

	polynomial* ret = (polynomial*)((char*) obj + sizeof(Obj));
	ret->list = new (list);
	ilist *list_interface = ret->list->ops
		= copy_list_interface(ret->list->ops);
	list_interface->clear = polynomial_clear_impl;

	ret->ops = &poly_ops;
	return ret;
}

void polynomial_destructor(void* p)
{
	if (unlikely(!p))
		return;

	polynomial* ptr = p;

	printf("polynomial_destructor: %p\n", p);
	ilist *list_interface = ptr->list->ops;
	list_interface->clear(ptr->list);

	delete(ptr->list);
}

int main()
{
	polynomial* poly = new (polynomial);
	polynomial* poly2 = new (polynomial);
	if (unlikely(!poly)) {
		printf("system out of memory\n");
		return 0;
	}

	ipolynomial *poly_interface = poly->ops;
	poly_interface->print(poly);
	printf("deg(f(x)) = %d\n", poly_interface->get_degree(poly));
	poly_interface->set_item(poly, new(poly_item, 3, 5));
	poly_interface->set_item(poly, new(poly_item, 2, 4));
	poly_interface->set_item(poly, new(poly_item, 1, 7));
	poly_interface->set_item(poly, new(poly_item, -4, 4));
	poly_interface->set_item(poly, new(poly_item, 9, -1));
	poly_interface->set_item(poly, new(poly_item, 8, 9));
	poly_interface->print(poly);
	printf("deg(f(x)) = %d\n", poly_interface->get_degree(poly));
	delete(poly);
	delete(poly2);
	return 0;
}
