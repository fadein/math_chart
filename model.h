#ifndef MODEL_H
#define MODEL_H

#include "control.h"

typedef struct problem {
	int left, right;
	char op;
	float fsol;
	int isol;
} problem;

struct problem new_problem(int l, int r, char o, struct config *conf);

struct problem rand_problem(struct config *conf);

struct problem* makeWorksheet(struct config *conf);

#endif /* MODEL_H */
