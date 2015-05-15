#include <stdio.h>

#include "model.h"
#include "view.h"
#include "control.h"


void test1(struct config *c) {
}

int main(int argc, char** argv) {
	config c;

	configureWorksheet(argc, argv, &c);
	problem *p = makeWorksheet(&c);

	printKey(&c);
	print_problems(&c, p);

	return 0;
}
