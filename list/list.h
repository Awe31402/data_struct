#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <stddef.h>

typedef struct _list List;

typedef struct _IList IList;

struct _IList {
	void (*setPrev) (List*, List*);
	void (*setNext) (List*, List*);
	void (*addTail) (List*, List*);
};

struct _list {
	List *next;
	List *prev;
	
	IList *ops;
};


List* List_constructor(void* addr);

void List_destructor(List *l);

#endif
