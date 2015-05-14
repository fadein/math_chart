#include <stdio.h>

#include "model.h"
#include "view.h"
#include "control.h"


void test1(struct config *c) {
	problem *p = makeWorksheet(c);

	print_problems(c, p);
}

int main(int argc, char** argv) {
	config c;
	argParsing(argc, argv, &c);

	argPrinting(&c);
	test1(&c);

	return 0;
}
