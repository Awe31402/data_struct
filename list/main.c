#include <stdio.h>
#include "list.h"
#include "base.h"

struct test {
	List *list_head;
	int a;
};

int main()
{
	struct test t;
	t.list_head = new(List);
	t.a = 3;

	struct test *pt;
	pt = container_of(&t.list_head, struct test, list_head);
	printf("pt->a = %d\n", pt->a);
	printf("pt->list_head->next = %p\n", pt->list_head->next);
	printf("pt->list_head->prev = %p\n", pt->list_head->prev);
	pt->list_head->ops->setPrev(pt->list_head->prev, NULL);
	printf("pt->list_head->next = %p\n", pt->list_head->next);
	printf("pt->list_head->prev = %p\n", pt->list_head->prev);
	delete(List, t.list_head);
	return 0;
}
