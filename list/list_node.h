#ifndef LIST_NODE_H
#define LIST_NODE_H

#include "base.h"
#include "ilist_node.h"

typedef struct _list_node list_node;

struct _list_node {
	void      *data;
	list_node *prev;
	list_node *next;

	ilist_node *ops;
};

list_node* list_node_constructor(void*);

void list_node_destructor(list_node* ptr);

#endif
