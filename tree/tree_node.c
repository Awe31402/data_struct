#include <stdio.h>
#include "base.h"
#include "tree_node.h"
#include "itree_node.h"
#include <stdlib.h>
#include <obj.h>

static itree_node tree_node_ops = {
	.set_right = NULL,
	.set_left = NULL,
	.set_parent = NULL,
	.set_data = NULL,
	.get_data = NULL,
};

void tree_node_destructor(void* ptr);

#define INIT_TREE_NODE(node, dat) \
  do { \
    node->left = node->right = node->parent = node; \
    node->data = dat; \
    node->ops = &tree_node_ops; \
  }while(0);

inline void init_tree_node(tree_node *n, void *d)
{
  INIT_TREE_NODE(n, d);
}

tree_node* tree_node_constructor(void *data)
{
	Obj *alloc = malloc(sizeof(tree_node) + sizeof(Obj));

	if (unlikely(!alloc))
		return NULL;

	alloc->ref_count = 1;
	alloc->destructor = tree_node_destructor;

	tree_node *ret = (tree_node*) ((char*) alloc + sizeof(Obj));

  INIT_TREE_NODE(ret,data);
	return ret;
}

void tree_node_destructor(void *ptr)
{
	printf("delete tree node: %p\n", ptr);
}
