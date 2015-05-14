#include <stdio.h>

#include "model.h"
#include "view.h"
#include "control.h"

void print_problem(struct problem p) {
	// TODO - detect/specify how wide each figure is
	printf("%4d\n", p.left);
	printf("%c%3d\n", p.op, p.right);

	// TODO - somehow indicate whenever we're in answer mode
	// answer mode = show the solution
	// regular mode = hide the solution
	printf("----\n%4d\n\n", p.isol);
}

void print_problems(struct config *conf, struct problem *p) {

	for (int r = 0; r < conf->height; ++r) {
		for (int c = 0; c < conf->width; ++c)
			printf("%4d   ", p[r * conf->width + c].left);
		printf("\n");

		for (int c = 0; c < conf->width; ++c)
			printf("%c%3d   ", p[r * conf->width + c].op, p[r * conf->width + c].right);
		printf("\n");

		for (int c = 0; c < conf->width; ++c)
			printf("----   ");
		printf("\n");

		if (conf->showAnswers)
			for (int c = 0; c < conf->width; ++c)
				printf("%4d   ", p[r * conf->width + c].isol);
		else
			for (int c = 0; c < conf->width; ++c)
				printf("       ");
		printf("\n\n");
	}
}
