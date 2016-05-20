#include <stdio.h>

#define SIZE 3
int arr[SIZE] = {0};

void list_all(int *arr, int index);

void print_arr(int *arr);

int main()
{

	list_all(arr, 0);
	return 0;
}

void print_arr(int *arr)
{
	int i;
	for (i = 0; i < SIZE; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

void list_all(int *arr, int index)
{
	if (index == SIZE -1) {
		arr[SIZE - 1] = 1;
		print_arr(arr);
		arr[SIZE -1] = 0;
		print_arr(arr);
		return;
	} else {
		arr[index] = 1;
		list_all(arr, index + 1);
		arr[index] = 0;
		list_all(arr, index + 1);
	}
}
