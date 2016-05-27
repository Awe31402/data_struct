#include "base.h"
#include "list.h"
#include "list_node.h"

static list_node* get_head_impl(list* l)
{
	if(unlikely(!l || !l->size))
		return NULL;
	return l->root->next;
}

static list_node* get_tail_impl(list* l)
{
	if (unlikely(!l))
		return NULL;
	return l->root->prev;
}

static int insert_impl(list *l, list_node *pos, list_node* node)
{
	if (unlikely(!l || !pos || !node))
		return FAILED;

	ilist_node *node_interface = node->ops;

	node_interface->set_next(node, pos);
	node_interface->set_prev(node, pos->prev);
	node_interface->set_next(pos->prev, node);
	node_interface->set_prev(pos, node);
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

	l->root->prev = l->root->next = l->root;
	l->size = 0;
	return SUCCESS;
}

static int append_impl(list* l, list_node *node)
{
	if(unlikely(!l || !node))
		return FAILED;
	list_node *tail = l->root->prev;
	ilist_node *node_interface = tail->ops;

	node_interface->set_prev(node, tail);
	node_interface->set_next(node, l->root);

	node_interface->set_next(tail, node);
	node_interface->set_prev(l->root, node);
	l->size++;
	return SUCCESS;
}

static int remove_impl(list *l, list_node *node)
{
	if (unlikely(!l || !node || !l->size))
		return FAILED;

	ilist_node *node_interface = node->ops;
	node_interface->set_next(node->prev, node->next);
	node_interface->set_prev(node->next, node->prev);
	if (node_interface->get_data(node) == NULL)
		delete(list_node, node);
	l->size--;
	return SUCCESS;
}

static ilist list_ops = {
	.clear =    clear_impl,
	.append =   append_impl,
	.insert =   insert_impl,
	.get_tail = get_tail_impl,
	.get_head = get_head_impl,
	.remove =   remove_impl,
};

list* list_constructor()
{
	void *addr = malloc(sizeof(list));

	if (unlikely(!addr))
		return NULL;
	list* ret = (list*) addr;
	ret->root = new(list_node, NULL);
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
