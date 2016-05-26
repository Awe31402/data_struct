#ifndef LIST_H
#define LIST_H

#include "base.h"
#include "list_node.h"

typedef struct _ilist ilist;
typedef struct _list list;

struct _ilist {
	list_node* (*get_tail) (list*);
	list_node* (*get_head) (list*);
	int (*append) (list*, list_node*);
	int (*clear) (list*);
	int (*insert) (list*, list_node*, list_node*);
	int (*remove) (list*, list_node*);
};

struct _list {
	int size;
	list_node *root;
	
	ilist *ops;
};

list* list_constructor();

void list_destructor(list* ptr);

#endif
