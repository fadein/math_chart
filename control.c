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

// get a random uint from /dev/urandom
static void getRandomUint(struct config *conf) {
	unsigned int r;
	int urandom = open("/dev/urandom", O_RDONLY);
	read(urandom, &r, sizeof(unsigned int));
	close(urandom);
	conf->seed = r;
}

// getopt parsing -> configuration object
void argParsing(int argc, char* argv[], struct config* conf) {
	// Initialize the config object
	assert(conf != NULL);

	conf->width = 10;
	conf->height = 10;
	conf->showAnswers = 0;

	// range of numbers for problems
	conf->low = 0;
	conf->high = 20;

	// PRNG seed value
	conf->seed = UINT_MAX;

	// Don't do division by default
	// TODO: fix division (print as a float, etc.)
	conf->numOps = 3;
	conf->operations[0] = '+';
	conf->operations[1] = '-';
	conf->operations[2] = '*';
	conf->operations[3] = '/';

	int c, fatality = 0;

	opterr = 0;
	while ((c = getopt(argc, argv, "ad:o:hvr:s:")) != -1) {
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
				printf("Version: 1.0\n");
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

			case 's':
				{
					char *endptr;
					conf->seed = (unsigned int)strtol(optarg, &endptr, 16);
				}
		}
	}

	if (fatality)
		exit(1);

	// come up with a seed value if one wasn't specified
	if (conf->seed == UINT_MAX)
		getRandomUint(conf);

	srandom(conf->seed);
}

void argPrinting(struct config* conf) {
	printf("Math Worksheet (-s0x%x -r%d:%d -d%dx%d -o'%s')\n\n",
			conf->seed,
			conf->low, conf->high,
			conf->width, conf->height,
			strndup(conf->operations, conf->numOps));
}
