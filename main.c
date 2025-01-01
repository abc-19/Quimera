/*              ___
 *           .="   "=._.---.		Sheet - main file.
 *         ."         c ' Y'`p		abc-19
 *        /   ,       `.  w_/
 *    jgs |   '-.   /     / 
 *  _,..._|      )_-\ \_=.\
 * `-....-'`------)))`=-'"`'"		Jan 01 2025
 */
#include "quimera.h"

#include <getopt.h>
#include <string.h>

static void parseArguments (const int, const char const**, struct Quimera *const);

int main (int argc, char **argv)
{
	struct Quimera quim;
	memset(&quim, 0, sizeof(quim));

	return 0;
}

static void parseArguments (const int nargs, const char const **vargs, struct Quimera *const quim) 
{
	// TODO, should we work with long options?
}
