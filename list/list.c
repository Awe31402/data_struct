#include "base.h"
#include "list.h"
#include "list_node.h"

static int insert_impl(list *l, list_node *pos, list_node* node)
{
	if (unlikely(!l || !pos || !node))
		return FAILED;
	pos->next->ops->set_prev(pos->next, node);
	pos->ops->set_next(pos, node);
	l->size++;
	return SUCCESS;
}

static int clear_impl(list *l)
{
	if (unlikely(!l))
		return FAILED;

	list_node* curr = l->root->next;

	while(curr != l->root) {
		curr = curr->next;
		delete(list_node, curr->prev);
	}

	l->root->prev = l->root->next = l->tail = l->root;
	l->size = 0;
	return SUCCESS;
}

static int append_impl(list* l, list_node *node)
{
	if(unlikely(!l || !node))
		return FAILED;
	l->tail->ops->set_next(l->tail, node);
	l->tail = node;
	l->root->ops->set_prev(l->root, node);
	l->size++;
	return SUCCESS;
}

static ilist list_ops = {
	.clear = clear_impl,
	.append = append_impl,
	.insert = insert_impl,
};

list* list_constructor()
{
	void *addr = malloc(sizeof(list));

	if (unlikely(!addr))
		return NULL;
	list* ret = (list*) addr;
	ret->root = ret->tail = new(list_node);
	ret->size = 0;

	ret->ops = &list_ops;
	return ret;
}

void list_destructor(list* ptr)
{
	ptr->ops->clear(ptr);
	delete(list_node, ptr->root);
	free(ptr);
}
