#include <stdio.h>

#include "model.h"
#include "view.h"


void test(void) {
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

int main(int argc, char** argv) {

	test();

	return 0;
}
