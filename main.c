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

#include <err.h>
#include <getopt.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#define	T_ON	1
#define	T_OFF	0

#define	BUFSZ	4

static void parseArguments (const int, char**, struct Quimera *const);
static void printUsage (const char *const);

static void setUpSignals (void);
static void signalHandler (const i32_t);

static void start (struct Quimera *const);
static void turnxxCanonical (const u8_t);

static void moveTowards (const char, u16_t*, u16_t*);

int main (int argc, char **argv)
{
	struct Quimera quim;
	memset(&quim, 0, sizeof(quim));

	setUpSignals();

	parseArguments(argc, argv, &quim);
	tui__drawLayout();

	start(&quim);

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

static void setUpSignals (void)
{
	struct sigaction sa;
	sa.sa_handler = signalHandler;
	sa.sa_flags = 0;

	sigaction(SIGINT,	&sa, NULL);
	sigaction(SIGQUIT,	&sa, NULL);
	sigaction(SIGTERM,	&sa, NULL);

	sigaction(SIGCONT,	&sa, NULL);
	sigaction(SIGTSTP,	&sa, NULL);

	sigaction(SIGHUP,	&sa, NULL);
	sigaction(SIGWINCH,	&sa, NULL);
}

static void signalHandler (const i32_t sig)
{
	switch (sig) {
		case SIGINT:
		case SIGQUIT:
		case SIGTERM:
		case SIGHUP:
			break;
		case SIGCONT:
			break;
		case SIGTSTP:
			break;
		case SIGWINCH:
			tui__drawLayout();
			break;
	}
}

static void start (struct Quimera *const quim)
{
	turnxxCanonical(T_OFF);
	char buff[BUFSZ] = {0};

	while (1) {
		read(FD_SIN, buff, BUFSZ);

		if (*buff == 0x1b && buff[1] == '[') {
			moveTowards(buff[2], &quim->row_p, &quim->col_p);
			tui__moveCursor(quim->row_p, quim->col_p, "here!");
			DEBUG_P("%d %d %d\n", quim->row_p, quim->col_p, buff[2]);
		}

		if (*buff == '1') break;
	}

	turnxxCanonical(T_ON);
	printf("\x1b[H\x1b[J\x1b[?25h");
}

static void turnxxCanonical (const u8_t mode)
{
	struct termios stts;
	tcgetattr(FD_SIN, &stts);

	switch (mode) {
		case T_OFF: stts.c_lflag &= ~(ICANON | ECHO); break;
		case T_ON : stts.c_lflag |= (ICANON  | ECHO); break;
	}

	tcsetattr(FD_SIN, TCSANOW, &stts);
}

static void moveTowards (const char towards, u16_t *row, u16_t *col)
{
	// TODO: implement warning messages
	switch (towards) {
		case 'A': { *row -= (*row) ? 1 : 0; break; }
		case 'D': { *col -= (*col) ? 1 : 0; break; }
		case 'B': { *row += ((*row + 1) == MaxRows) ? 0 : 1; break; }
		case 'C': { *col += ((*col + 1) == MaxCols) ? 0 : 1; break; }
	}
}
