/*              ___
 *           .="   "=._.---.		Sheet - main file.
 *         ."         c ' Y'`p		abc-19
 *        /   ,       `.  w_/
 *    jgs |   '-.   /     / 
 *  _,..._|      )_-\ \_=.\
 * `-....-'`------)))`=-'"`'"		Jan 01 2025
 */
#include "quimera.h"
#include "tui.h"

#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

static void parseArguments (const int, char**, struct Quimera *const);
static void printUsage (const char *const);

int main (int argc, char **argv)
{
	struct Quimera quim;
	memset(&quim, 0, sizeof(quim));

	parseArguments(argc, argv, &quim);
	tui__drawLayout();

	scanf("%d", &argc);
	return 0;
}

static void parseArguments (const int nargs, char **vargs, struct Quimera *const quim) 
{
	i32_t op;
	while ((op = getopt(nargs, vargs, ":s:Ru")) != -1) {
		switch (op) {
			case 's': quim->docName = optarg; break;
			case 'R': quim->readOnly = True; break;
			case 'u': printUsage(*vargs); break;
			case ':': errx(-1, "[args]: The `-%c` needs an additional argument.", optopt); break;
			case '?': errx(-1, "[args]: The '-%c' is not a quimera argument.", optopt); break;
		}
	}
}

static void printUsage (const char *const as)
{
	static const char *const usage =
		"Quimera 02 Jan 2025 - %s %s\n"
		"usage: %s [arguments]\n"
		"   or: %s -s <sheet> [arguments]\n"
		"arguments:\n"
		"   \x1b[38;5;92m-\x1b[0ms	*		Load <*> document\n"
		"   \x1b[38;5;92m-\x1b[0mR			Read only\n"
		"   \x1b[38;5;92m-\x1b[0mu			Familiar?\n";

	fprintf(stderr, usage, __DATE__, __TIME__, as, as);
	exit(0);
}
