#ifndef TREE_NODE_H
#define TREE_NODE_H
#include "base.h"

typedef struct _tree_node tree_node;
typedef struct _itree_node itree_node;

struct _tree_node
{
	void *data;

	tree_node* parent;
	tree_node* left;
	tree_node* right;

	itree_node* ops;
};

void init_tree_node(tree_node*, void*);

tree_node* tree_node_constructor(void*);
void tree_node_destructor(void*);

#endif

