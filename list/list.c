#include "list.h"
#include <stdio.h>

static void setNext_impl(List *l, List *r);
static void setPrev_impl(List *l, List *r);

static IList list_ops = {
	.setPrev = setPrev_impl,
	.setNext = setNext_impl,
};

List* List_constructor(void* addr) 
{
	printf("going to construct list node..\n");
	if (!addr) return NULL;
	List *ret = addr;
	ret->prev = ret->next = ret;

	ret->ops = &list_ops;
	return ret;
}

void List_destructor(List *l)
{
	printf("going to delete list node..\n");
	free(l);
}

static void setPrev_impl(List *l, List *r)
{
	l->prev = r;
}

static void setNext_impl(List *l, List *r)
{
	l->next = r;
}
