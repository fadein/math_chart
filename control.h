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
} config;

// getopt parsing -> configuration object
void argParsing(int argc, char* argv[], struct config* conf);

#endif /* CONTROL_H */
