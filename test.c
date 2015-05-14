#include <stdio.h>

#include "model.h"
#include "view.h"
#include "control.h"


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

void test1(struct config *c) {
	problem p[c->width * c->height];

	for (int i = 0; i < c->width * c->height; ++i)
		p[i] = rand_problem();

	print_problems(c, p);
}

int main(int argc, char** argv) {
	config c;

	argParsing(argc, argv, &c);
	test1(&c);

	return 0;
}
