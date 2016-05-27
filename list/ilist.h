#ifndef ILIST_H
#define ILIST_H

#include "base.h"
#include "list.h"

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

#endif
