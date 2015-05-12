#include <stdio.h>

#include "model.h"
#include "view.h"

void print_problem(struct problem p) {
	// TODO - detect/specify how wide each figure is
	printf("%4d\n", p.left);
	printf("%c%3d\n", p.op, p.right);

	// TODO - somehow indicate whenever we're in answer mode
	// answer mode = show the solution
	// regular mode = hide the solution
	printf("----\n%4d\n\n", p.isol);
}

void print_problems(struct problem *p, int rows, int cols) {

	for (int r = 0; r < rows; ++r) {
		for (int c = 0; c < cols; ++c)
			printf("%4d   ", p[r * cols + c].left);
		printf("\n");

		for (int c = 0; c < cols; ++c)
			printf("%c%3d   ", p[r * cols + c].op, p[r * cols + c].right);
		printf("\n");

		for (int c = 0; c < cols; ++c)
			printf("----   ");
		printf("\n");

		for (int c = 0; c < cols; ++c)
			printf("%4d   ", p[r * cols + c].isol);
		printf("\n\n");
	}
}
