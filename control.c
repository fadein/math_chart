#include <stdio.h>
#include <assert.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "model.h"
#include "view.h"
#include "control.h"

typedef union bitpack {
	unsigned int ui;
	int i;
	unsigned char uc;
} bitpack;

static unsigned long int formKey(struct config* conf) {
	//seed, rangeLo, rangeHi, width, height, ops
	unsigned char o = 0;

	if (memchr(conf->operations, '+', conf->numOps))
		o |= 1 << 0;
	if (memchr(conf->operations, '-', conf->numOps))
		o |= 1 << 1;
	if (memchr(conf->operations, '*', conf->numOps))
		o |= 1 << 2;
	if (memchr(conf->operations, '/', conf->numOps))
		o |= 1 << 3;

	bitpack rangeLo  = {.i  = abs(conf->low)},
			rangeHi  = {.i  = abs(conf->high)},
			width    = {.ui = conf->width},
			height   = {.ui = conf->height},
			ops      = {.uc = o},
			negLo    = {.uc = conf->low < 0},
			negHi    = {.uc = conf->high < 0},
			negDiff  = {.uc = conf->negDiff};

	unsigned int bits =
		(negDiff.uc & 0x1 ) << 30 |
		(negHi.uc              & 0x1 ) << 29 |
		(negLo.uc              & 0x1 ) << 28 |
		(ops.uc                & 0xF ) << 24 |
		(width.uc              & 0xF ) << 20 |
		(height.uc             & 0xF ) << 16 |
		(rangeHi.uc            & 0xFF) <<  8 |
		(rangeLo.uc            & 0xFF);
	bits ^= conf->seed;

	unsigned long int key = ((unsigned long int )(bits & 0xFFffFFff) << 32)
		| conf->seed;
	return key;
}

static void unformKey(unsigned long int key, struct config* conf) {
	unsigned int bits = (unsigned int)((key >> 32) & 0xFFffFFff),
				 seed = (unsigned int)(key & 0xFFffFFff);

	conf->seed = seed;
	bits ^= seed;

	bitpack rangeLo  = {.uc = bits  & 0x000000FF},
			rangeHi  = {.uc = (bits & 0x0000FF00) >> 8},
			height   = {.uc = (bits & 0x000F0000) >> 16},
			width    = {.uc = (bits & 0x00F00000) >> 20},
			ops      = {.uc = (bits & 0x0F000000) >> 24},
			negLo    = {.uc = (bits & 0x10000000) >> 28},
			negHi    = {.uc = (bits & 0x20000000) >> 29},
			negDiff  = {.uc = (bits & 0x40000000) >> 30};

	conf->low     = rangeLo.i;
	if (negLo.uc) conf->low *= -1;

	conf->high    = rangeHi.i;
	if (negHi.uc) conf->high *= -1;

	conf->height  = height.ui;
	conf->width   = width.ui;
	conf->negDiff = negDiff.uc;

	int i = 0;
	if (ops.uc & 1 << 0)
		conf->operations[i++] = '+';
	if (ops.uc & 1 << 1)
		conf->operations[i++] = '-';
	if (ops.uc & 1 << 2)
		conf->operations[i++] = '*';
	if (ops.uc & 1 << 3)
		conf->operations[i++] = '/';
	conf->numOps = i;
}

void printKey(struct config* conf) {
	printf("Math Worksheet v" VERSION " [%lX]\n\n", formKey(conf));
}

// get a random uint from /dev/urandom
static void getRandomUint(struct config *conf) {
	unsigned int r;
	int urandom = open("/dev/urandom", O_RDONLY);
	read(urandom, &r, sizeof(unsigned int));
	close(urandom);
	conf->seed = r;
}

// getopt parsing -> configuration object
void configureWorksheet(int argc, char* argv[], struct config* conf) {
	// Initialize the config object
	assert(conf != NULL);

	conf->width = 10;
	conf->height = 10;
	conf->showAnswers = 0;

	// range of numbers for problems
	conf->low = 0;
	conf->high = 20;

	// ensure that subtraction problems result in non-negative solutions
	conf->negDiff = 0;

	// PRNG seed value
	conf->seed = UINT_MAX;

	// Don't do division or multiplication by default
	// TODO: fix division (print as a float, etc.)
	conf->numOps = 2;
	conf->operations[0] = '+';
	conf->operations[1] = '-';
	conf->operations[2] = '*';
	conf->operations[3] = '/';

	int c, fatality = 0;

	opterr = 0;
	while ((c = getopt(argc, argv, "ad:o:nhvr:s:")) != -1) {
		switch (c) {

			// whether to display the solutions
			case 'a':
				conf->showAnswers = 1;
				break;

			// dimensions of worksheet
			// TODO - should have a better error message here when stuff goes south
			case 'd':
				conf->width = atoi(strtok(optarg, "x"));
				conf->height = atoi(strtok(NULL, "x"));
				break;

			// which arithmetic operations to allow
			case 'o':
				conf->numOps = 0;
				if (strchr(optarg, '+'))
					conf->operations[conf->numOps++] = '+';
				if (strchr(optarg, '-'))
					conf->operations[conf->numOps++] = '-';
				if (strchr(optarg, '*'))
					conf->operations[conf->numOps++] = '*';
				if (strchr(optarg, '/'))
					conf->operations[conf->numOps++] = '/';
				break;

			case 'h':
				printf("Usage: TBD\n");
				fatality++;
				break;

			case 'v':
				printf("Version: " VERSION "\n");
				fatality++;
				break;

			case 'r':
				// TODO - should have a better error message here when stuff goes south
				conf->low  = atoi(strtok(optarg, ":"));
				conf->high = atoi(strtok(NULL, ":"));
				if (conf->low > conf->high) {
					conf->low  ^= conf->high;
					conf->high  = conf->low ^ conf->high;
					conf->low   = conf->low ^ conf->high;
				}
				break;

			case 'n':
				conf->negDiff = 1;
				break;

			case 's':
				{
					char *endptr;
					conf->seed = (unsigned int)strtol(optarg, &endptr, 16);
				}
		}
	}

	if (fatality)
		exit(1);

	// if there are command-line arguments after we're done with getopt,
	// then try to parse the next argument as a KEY
	if (optind < argc) {
		char *endptr;
		unsigned long int key = strtoul(argv[optind], &endptr, 16);
		unformKey(key, conf);
	}

	// come up with a seed value if one wasn't specified
	if (conf->seed == UINT_MAX)
		getRandomUint(conf);

	srandom(conf->seed);
}
