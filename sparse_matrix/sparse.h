#ifndef SPARSE_H
#define SPARSE_H

#define MAX_TERMS 100

typedef struct _sparse_term sparse_term;

struct _sparse_term {
	int col;
	int row;
	int value;
};

typedef struct _IMatrix IMatrix;

typedef struct _sparse_matrix sparse_matrix;

struct _sparse_matrix {
	int row;
	int col;
	int terms_num;
	sparse_term data[MAX_TERMS];
	IMatrix *ops;
};

struct _IMatrix {
	int  (*transpose) (sparse_matrix* in, sparse_matrix* out);
	int  (*add)       (sparse_matrix* in1, sparse_matrix* in2, sparse_matrix* out);
	int  (*multiply)  (sparse_matrix* in1, sparse_matrix* in2, sparse_matrix* out);
	int  (*set_element) (sparse_matrix* m, const int row, const int col, const int value);
	void (*print) (sparse_matrix* m);
	int  (*get_element) (sparse_matrix*, const int row, const int col, int* ret);
};

sparse_matrix *sparse_matrix_constructor(void *, int row, int col);
void sparse_matrix_destructor(sparse_matrix *ptr);
#endif
