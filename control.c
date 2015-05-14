#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>

extern char *optarg;
extern int opterr;

#include "model.h"
#include "view.h"
#include "control.h"

// getopt parsing -> configuration object
void argParsing(int argc, char* argv[], struct config* conf) {

	// Initialize the config object
	assert(conf != NULL);

	conf->width = 10;
	conf->height = 10;
	conf->showAnswers = 0;
	conf->numOps = 4;
	conf->operations[0] = '+';
	conf->operations[1] = '-';
	conf->operations[2] = '*';
	conf->operations[3] = '/';

	int c, fatality = 0;

	opterr = 0;

	while ((c = getopt(argc, argv, "ad:o:hv")) != -1) {
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
		}
	}

	if (fatality)
		exit(1);
}

