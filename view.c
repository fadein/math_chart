#include <stdio.h>

#include "model.h"
#include "view.h"

void print_problem(struct problem p) {
	// TODO - detect/specify how wide each figure is
	printf("%4d\n", p.left);
	printf("%c%3d\n", p.op, p.right);

	// TODO - somehow indicate whenever we're in answer mode
	// answer mode = show the solution
	// regular mode = hide the solution
	printf("----\n%4d\n\n", p.isol);
}
