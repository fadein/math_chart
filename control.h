#ifndef CONTROL_H
#define CONTROL_H

#include <inttypes.h>

#define VERSION "1.4"

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
	uint32_t seed;

	// should subtraction problems be constrained to have only non-negative solutions?
	unsigned char negDiff;
} config;

// Print this worksheet's UUID
void printKey(struct config* conf);

// getopt parsing -> configuration object
void configureWorksheet(int argc, char* argv[], struct config* conf);

#endif /* CONTROL_H */
