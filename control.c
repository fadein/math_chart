#include <stdio.h>

#include "model.h"
#include "view.h"


void test0(void) {
	problem p = new_problem(2, 2, '+');
	print_problem(p);

	p = new_problem(3, 9, '+');
	print_problem(p);

	p = new_problem(3, 9, '*');
	print_problem(p);

	p = new_problem(3, 9, '-');
	print_problem(p);

	p = new_problem(9, 3, '-');
	print_problem(p);

	p = new_problem(27, 3, '/');
	print_problem(p);

	p = new_problem(26, 3, '/');
	print_problem(p);

	p = new_problem(25, 3, '/');
	print_problem(p);

	p = new_problem(24, 3, '/');
	print_problem(p);
}

void test1() {
	int rows = 5, cols = 4;
	problem p[rows * cols];

	for (int i = 0; i < rows * cols; ++i)
		p[i] = rand_problem();

	print_problems(p, rows, cols);
}

int main(int argc, char** argv) {

	test1();

	return 0;
}
