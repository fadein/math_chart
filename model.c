#define _XOPEN_SOURCE 500
#include <stdlib.h>

#include "model.h"


struct problem new_problem(int l, int r, char o) {
	problem p = {.left = l, .right = r, .op = o};

	switch (o) {
		case '+':
			p.isol = p.fsol = l + r;
			break;

		case '-':
			p.isol = p.fsol = l - r;
			break;

		case '*':
			p.isol = p.fsol = l * r;
			break;

		case '/':
			if (r != 0) {
				p.isol = l / r;
				p.fsol = (float)l / (float)r;
			}
			else {
				p.isol = 0;
				p.fsol = 0.0;
			}
			break;
	}

	return p;
}

// TODO - take into account the types of ops we want, and the range of operands
struct problem rand_problem() {
	static int inited = 0;

	if (!inited) {
		srandom(42);
		inited = 1;
	}
	
	// TODO - mod sucks; do something smarter
	int l = random() % 20;
	int r = random() % 20;

	char c;
	switch (random() % 4) {
		case 0:
			c = '+';
			break;

		case 1:
			c = '-';
			break;

		case 2:
			c = '*';
			break;

		case 3:
			c = '/';
			break;
	}

	return new_problem(l, r, c);
}

