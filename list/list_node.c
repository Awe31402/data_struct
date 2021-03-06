#include "list_node.h"
#include "base.h"
#include <obj.h>
#include <stdio.h>

int set_prev_impl(list_node* l, list_node* r)
{
	if (unlikely(!l || !r))
		return FAILED;
	l->prev = r;
	return SUCCESS;
}

int set_next_impl(list_node* l, list_node* r)
{
	if (unlikely(!l || !r))
		return FAILED;

	l->next = r;
	return SUCCESS;
}

void set_data_impl(list_node *l, void *data)
{
	if (unlikely(!l))
		return;
	l->data = data;
}

void* get_data_impl(list_node *l)
{
	if (unlikely(!l))
		return NULL;
	return l->data;
}

static ilist_node list_node_ops = {
	.set_prev = set_prev_impl,
	.set_next = set_next_impl,
	.set_data = set_data_impl,
	.get_data = get_data_impl,
};

list_node* list_node_constructor(void *data)
{
	Obj *addr = malloc(sizeof(list_node) + sizeof(Obj));
	if (unlikely(!addr))
		return NULL;

	addr->ref_count = 1;
	addr->destructor = list_node_destructor;

	list_node* ret = (list_node*) ((char*)addr + sizeof(Obj));
	ret->prev = ret->next = ret;
	ret->ops = &list_node_ops;
	ret->data = data;
	return ret;
}

void list_node_destructor(void* ptr)
{
	printf("delete list node: %p\n", ptr);
}
