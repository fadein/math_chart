#define _XOPEN_SOURCE 500
#include <stdlib.h>

#include "model.h"


struct problem new_problem(int l, int r, char o, struct config *conf) {
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

struct problem rand_problem(struct config *conf) {
	// TODO - mod sucks; do something smarter
	int l = random() % abs(conf->high - conf->low + 1) + conf->low;
	int r = random() % abs(conf->high - conf->low + 1) + conf->low;
	char c = conf->operations[random() % conf->numOps];

	return new_problem(l, r, c, conf);
}

struct problem* makeWorksheet(struct config *c) {
	// TODO: check errors on malloc
	// I'm also sure Ben Klemens had a better way of doing this dance
	problem *p = (problem *)malloc(sizeof(struct config) * c->width * c->height);

	for (int i = 0; i < c->width * c->height; ++i)
		p[i] = rand_problem(c);

	return p;
}
