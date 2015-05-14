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
	config c;
	c.width = 10;
	c.height = 10;
	c.showAnswers = 0;


	problem p[c.width * c.height];

	for (int i = 0; i < c.width * c.height; ++i)
		p[i] = rand_problem();

	print_problems(&c, p);
}

int main(int argc, char** argv) {

	test1();

	return 0;
}
