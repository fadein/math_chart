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

// Given a configuration structure, form a key number
static unsigned long long int formKey(struct config* conf) {
	//seed, rangeLo, rangeHi, width, height, ops
	unsigned char o = 0;

	if (memchr(conf->operations, '+', conf->numOps))
		o |= 1 << 0;
	if (memchr(conf->operations, '-', conf->numOps))
		o |= 1 << 1;
	if (memchr(conf->operations, 'x', conf->numOps))
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

	unsigned long long int key = ((unsigned long long int )(bits & 0xFFffFFff) << 32)
		| conf->seed;
	return key;
}

// Given a key number, form a configuration structure
static void unformKey(unsigned long long int key, struct config* conf) {
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
	// table of the number of bits per number [0 - 15]
	// (except where I say that 0 has 1 bit I can't have numOps == 0
	// since I use it as a divisor later on in the code)
	int bpn[] = { 1, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4 };
	conf->numOps = bpn[ops.uc & 0xF];
}

// Print the key as an easy-to-read hex string
void printKey(struct config* conf) {
	unsigned long long k = formKey(conf);
	printf("Math Worksheet v" VERSION " [%X %X %X %X]\n\n",
			(k & 0xFFFF000000000000) >> 48,
			(k & 0x0000FFFF00000000) >> 32,
			(k & 0x00000000FFFF0000) >> 16,
			(k & 0x000000000000FFFF));
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
	conf->operations[2] = 'x';
	conf->operations[3] = '/';

	int c, fatality = 0;

	opterr = 0;
	while ((c = getopt(argc, argv, "ad:o:nhvr:s:")) != -1) {
		switch (c) {

			// Display usage info
			case 'h':
				printf( "Math worksheet generator " VERSION "\n"
						"Usage: worksheet [options] [KEY]\n\n"
						"\tProduce a random 10x10 arithmetic worksheet along with its unique key.\n"
						"\tWhen this program is given the key value, the exact same sheet is recreated.\n"
						"\nThese options are for informational purposes only:\n"
						"\t-h        Display this help message and terminate\n"
						"\t-v        Display the version number and terminate\n"
						"\nThis option works when a KEY is specified:\n"
						"\t-a        Display the answers to each problem\n"
						"\nThese options are ignored when a KEY is given, as these settings are encoded by the KEY:\n"
						"\t-n        Allow differences to be negative\n"
						"\t          (subtraction problems give non-negative answers by defalut)\n"
						"\t-d RxC    Specify the number of Rows and Columns of problems on the worksheet\n"
						"\t          (maximum value for each is 16)\n"
						"\t-o [+-x/] Choose the operations to perform\n"
						"\t          (+ and - are enabled by default)\n"
						"\t-r L:H    Specify the range of numbers to quiz\n"
						"\t          (default value is 0:20, allowed range is [-255,255]\n");
				fatality++;
				break;

			// Display version number
			case 'v':
				printf("Version: " VERSION "\n");
				fatality++;
				break;


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
				if (strchr(optarg, 'x'))
					conf->operations[conf->numOps++] = 'x';
				if (strchr(optarg, '/'))
					conf->operations[conf->numOps++] = '/';
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
		char keyStr[17] = { '\0' };
		int n = 0,
			c = optind;

		// join up to 16 space-separated chars on cmdline into a key
		for (; c < argc && n <= 16; ++c) {
			if (strlen(argv[c]) <= 16 - n) {
				n += strlen(argv[c]);
				strncat(keyStr, argv[c], n);
			}
			else {
				// this argument is long enough to overflow keyStr, and is longer than
				// a valid key. We'll just pick off enough bytes to complete our key
				strncat(keyStr, argv[c], 16 - n);
				break;
			}
		}

		unsigned long long int key = strtoull(keyStr, NULL, 16);
		unformKey(key, conf);
	}

	// come up with a seed value if one wasn't specified
	if (conf->seed == UINT_MAX)
		getRandomUint(conf);

	srandom(conf->seed);
}
