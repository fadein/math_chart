#define _XOPEN_SOURCE 500
#include <stdlib.h>

#include <stdio.h>
#include "model.h"


struct problem new_problem(int l, int r, char o, struct config *conf) {
	problem p = {.left = l, .right = r, .op = o};

	switch (p.op) {
		case '+':
			p.isol = p.fsol = p.left + p.right;
			break;

		case '-':
			// Make sure the outcomes are non-negative, if so specified
			if (!conf->negDiff && p.left < p.right) {
				p.left ^= p.right;
				p.right  = p.left ^ p.right;
				p.left  = p.left ^ p.right;
			}
			p.isol = p.fsol = p.left - p.right;
			break;

		case '*':
			p.isol = p.fsol = p.left * p.right;
			break;

		case '/':
			if (p.right != 0) {
				p.isol = p.left / p.right;
				p.fsol = (float)p.left / (float)p.right;
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
