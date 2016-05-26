#include "list_node.h"
#include "base.h"

int set_prev_impl(list_node* l, list_node* r)
{
	if (unlikely(!l || !r))
		return FAILED;
	l->prev = r;
	r->next = l;
	return SUCCESS;
}

int set_next_impl(list_node* l, list_node* r)
{
	if (unlikely(!l || !r))
		return FAILED;

	l->next = r;
	r->prev = l;
	return SUCCESS;
}

static ilist_node list_node_ops = {
	.set_prev = set_prev_impl,
	.set_next = set_next_impl,
};

list_node* list_node_constructor()
{
	void *addr = malloc(sizeof(list_node));
	if (unlikely(!addr))
		return NULL;
	
	list_node* ret = (list_node*) addr;
	ret->prev = ret->next = ret;
	ret->ops = &list_node_ops;
	return ret;
}

void list_node_destructor(list_node* ptr)
{
	free(ptr);
}
