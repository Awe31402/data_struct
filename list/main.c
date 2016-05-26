#include "base.h"
#include "list.h"
#include "list_node.h"
#include <stdio.h>

int main()
{
	int i = 0;
	list* l = new(list);
	list_node *c;
	list_node *s = new(list_node);
	list_node *t = new(list_node);

	l->ops->append(l ,s);
	l->ops->append(l ,t);

	s->ops->set_next(s,t);

	printf("s = %p\n", s);
	printf("t = %p\n", t);
	printf("s->next = %p\n", s->next);
	printf("t->prev = %p\n", t->prev);

	while (i < 4) {
		l->ops->append(l, new(list_node));
		printf("append: %p, size = %d\n",
				l->tail, l->size);
		i++;
	}

	l->ops->insert(l, t, new(list_node));

	printf("--list size %d --\n", l->size);
	c = l->root->next;
	while (c != l->root) {
		printf("%p\n", c);
		c = c->next;
	}

	l->ops->clear(l);
	printf("clear, size = %d\n", l->size);
	delete(list, l);
	return 0;
}
