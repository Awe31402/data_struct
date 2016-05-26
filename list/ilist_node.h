#ifndef ILIST_NODE_H
#define ILIST_NODE_H

#include "list_node.h"

typedef struct _list_node list_node;
typedef struct _ilist_node ilist_node;

struct _ilist_node {
	int (*set_prev)(list_node*, list_node*);
	int (*set_next)(list_node*, list_node*);
	void (*set_data) (list_node*, void*);
	void* (*get_data) (list_node*);
};

#endif
