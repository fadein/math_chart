#ifndef CONTROL_H
#define CONTROL_H

typedef struct config {
	// size of worksheet
	int width, height;

	// whether to display the solutions or not
	int showAnswers;

	// which arithmetic operations to allow on this worksheet
	int numOps;
	char operations[4];

	// range of values for exercises
	int low, high;

	// PRNG seed value
	unsigned int seed;

} config;

// getopt parsing -> configuration object
void configureWorksheet(int argc, char* argv[], struct config* conf);

void printArguments(struct config* conf);

#endif /* CONTROL_H */
