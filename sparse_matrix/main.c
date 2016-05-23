#include "base.h"
#include "sparse.h"

#define LOOPNUM 1000000

int main()
{
	int i = 0;
	sparse_matrix *a = new (sparse_matrix, 100, 100);
	sparse_matrix *b = new (sparse_matrix, 0, 0);
	sparse_matrix *c = new (sparse_matrix, 0, 0);

	a->ops->set_element(a, 1, 2, 4);
	a->ops->set_element(a, 1, 1, 5);
	a->ops->set_element(a, 2, 2, 6);
	a->ops->set_element(a, 2, 2, 10);
	a->ops->set_element(a, 3, 1, 7);
	a->ops->set_element(a, 2, 3, 8);
	a->ops->set_element(a, 3, 4, 9);
	a->ops->set_element(a, 99, 89, 100);

	a->ops->print(a);

	while (i < LOOPNUM) {
		b->ops->clear(b);
		a->ops->transpose(a, b);
		i++;
	}

	b->ops->print(b);
	i = 0;
	while (i < LOOPNUM) {
		c->ops->add(a, b, c);
		i++;
	}
	c->ops->print(c);
	delete(sparse_matrix, a);
	delete(sparse_matrix, b);
	delete(sparse_matrix, c);
	return 0;
}
