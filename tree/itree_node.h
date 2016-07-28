#ifndef ITREE_NODE_H
#define ITREE_NODE_H
#include "tree_node.h"
typedef struct  _tree_node tree_node;
typedef struct _itree_node itree_node;

struct _itree_node {
	int   (*set_parent)  (tree_node*, tree_node*);
	int   (*set_left)  (tree_node*, tree_node*);
	int   (*set_right) (tree_node*, tree_node*);
	void  (*set_data)  (tree_node*, void*);
	void* (*get_data)  (tree_node*);
};
#endif
