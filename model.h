#ifndef MODEL_H
#define MODEL_H

typedef struct problem {
	int left, right;
	char op;
	float fsol;
	int isol;
} problem;

struct problem rand_problem();

struct problem new_problem(int l, int r, char o);

#endif /* MODEL_H */
