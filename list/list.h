#ifndef LIST_H
#define LIST_H

#include "base.h"
#include "list_node.h"

typedef struct _ilist ilist;
typedef struct _list list;

struct _ilist {
	int (*append) (list*, list_node*);
	int (*clear) (list*);
	int (*insert) (list*, list_node*, list_node*);
};

struct _list {
	int size;
	list_node *root;
	list_node *tail;
	
	ilist *ops;
};

list* list_constructor();

void list_destructor(list* ptr);

#endif
