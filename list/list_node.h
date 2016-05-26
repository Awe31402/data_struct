#ifndef LIST_NODE_H
#define LIST_NODE_H

#include "base.h"

typedef struct _list_node list_node;
typedef struct _ilist_node ilist_node;

struct _ilist_node {
	int (*set_prev)(list_node*, list_node*);
	int (*set_next)(list_node*, list_node*);
};

struct _list_node {
	list_node *prev;
	list_node *next;

	ilist_node *ops;
};

list_node* list_node_constructor();

void list_node_destructor(list_node* ptr);

#endif
