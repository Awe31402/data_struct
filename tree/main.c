#include <stdio.h>
#include "tree_node.h"
#include "itree_node.h"
#include <obj.h>

int main()
{
	int x = 3;
	int y = 6;

	tree_node *n1 = new(tree_node, &x);
	tree_node *n2 = new(tree_node, &y);

  tree_node node;
  init_tree_node(&node, NULL);
	printf("n1:\nn1 = %p, n1->parent = %p\nn1->left = %p, n1->right = %p\n",
			n1, n1->parent, n1->left, n1->right);
	printf("n2:\nn2 = %p, n2->parent = %p\nn2->left = %p, n2->right = %p\n",
			n2, n2->parent, n2->left, n2->right);
	printf("node:\nnode = %p, node.parent = %p\nnode.left = %p, node.right = %p\n",
			&node, node.parent, node.left, node.right);

	delete(n1);
	delete(n2);
	return 0;
}
