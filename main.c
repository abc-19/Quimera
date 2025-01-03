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

#define	BUFINP_			4
#define	MAXCOLS_		78
#define	MAXROWS_		124

enum TurnxMode
{
	TurnOn	= 1,
	TurnOff	= 2
};

static void parseExecArgs (const int, char**, struct Quimera *const);
static void showUsage (const char *const);

static void setUpSignals (void);
static void signalHandler (const i32_t);

static void startInteractions (struct Quimera *const);
static void turnxxCanonical (const enum TurnxMode);

static void checkMotion (const char, u16_t*, u16_t*);

int main (int argc, char **argv)
{
	struct Quimera quim;
	memset(&quim, 0, sizeof(quim));

	setUpSignals();

	parseExecArgs(argc, argv, &quim);
	tui__drawLayout(quim.rowPos, quim.colPos);

	tui__updateSheetName("NoName...............");
	startInteractions(&quim);

	return 0;
}

static void parseExecArgs (const int nargs, char **vargs, struct Quimera *const quim) 
{
	i32_t op;
	while ((op = getopt(nargs, vargs, ":s:Ru")) != -1) {
		switch (op) {
			case 's': quim->docName = optarg; break;
			case 'R': quim->readOnly = True; break;
			case 'u': showUsage(*vargs); break;
			case ':': errx(-1, "[args]: The `-%c` needs an additional argument.", optopt); break;
			case '?': errx(-1, "[args]: The '-%c' is not a quimera argument.", optopt); break;
		}
	}
}

static void showUsage (const char *const as)
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
			fprintf(stderr, "No supported yet\n");
			break;
	}
}

static void startInteractions (struct Quimera *const quim)
{
	turnxxCanonical(TurnOff);
	char buff[BUFINP_] = {0};


	bool_t leave = False;

	while (!leave) {
		read(FD_SIN, buff, BUFINP_);

		switch (*buff) {
			case 'h': case 'j': case 'k': case 'l': {
				checkMotion(*buff, &quim->rowPos, &quim->colPos);
				tui__moveCursor(quim->rowPos, quim->colPos, "          ");
			}
			case ':': break;
			case 'q': leave = True; break;
		}
	}

	turnxxCanonical(TurnOn);
	printf("\x1b[H\x1b[J\x1b[?25h");
}

static void turnxxCanonical (const enum TurnxMode mode)
{
	struct termios stts;
	tcgetattr(FD_SIN, &stts);

	switch (mode) {
		case TurnOff: stts.c_lflag &= ~(ICANON | ECHO); break;
		case TurnOn : stts.c_lflag |= (ICANON  | ECHO); break;
	}

	tcsetattr(FD_SIN, TCSANOW, &stts);
}

static void checkMotion (const char towards, u16_t *row, u16_t *col)
{
	// TODO: implement warning messages
	switch (towards) {
		case 'k': { *row -= (*row) ? 1 : 0; break; }
		case 'h': { *col -= (*col) ? 1 : 0; break; }
		case 'j': { *row += ((*row + 1) == MAXROWS_) ? 0 : 1; break; }
		case 'l': { *col += ((*col + 1) == MAXCOLS_) ? 0 : 1; break; }
	}
}
