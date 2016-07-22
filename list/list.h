#ifndef LIST_H
#define LIST_H

#include "base.h"
#include "list_node.h"
#include "ilist.h"

typedef struct _ilist ilist;
typedef struct _list list;

struct _list {
	int size;
	list_node *root;
	
	ilist *ops;
};

list* list_constructor();

void list_destructor(void* ptr);

#endif
