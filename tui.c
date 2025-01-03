/*              ___
 *           .="   "=._.---.		Sheet - Terminal user interface.
 *         ."         c ' Y'`p		abc-19
 *        /   ,       `.  w_/
 *    jgs |   '-.   /     / 
 *  _,..._|      )_-\ \_=.\
 * `-....-'`------)))`=-'"`'"		Jan 02 2025
 */
#include "tui.h"
#include <sys/ioctl.h>

/* 
 * +---------------------------------+
 * A0	fx: some text				 +	row used (1)
 * cmd:			 					 +	row used (2)
 * 		A		B		C		D	 +	row used (3)
 * 0	39							 +	This is the first row for cells (4th one).
 * 1								 +
 * 2								 +
 * 3								 +
 * "sheet-name"						 +	row used (4)
 * ----------------------------------+
 *
 * ROWSALRDUSED_: refers to the number of rows
 * where information is usted to display information
 * instead of cells
 *
 * LEFTMARGIN_: Refers to the number of bytes used to display
 * the column of row numbers plus 1, since the width of such
 * column is 5 bytes but we need to start writing from 6th.
 * */
#define	LEFTMARGIN_		6
#define	ROWSALRDUSED_	4

#define	COLWIDTH_		10

/* Number of vertical bytes (lines) and number
 * of horizonta byrtes (columns) available in
 * the current view. */
static u16_t WVBytes_, WHBytes_;

/* Number of columns and rows available for
 * the user to see and go throught. */
static u16_t nRows_, nCols_;

static void getWinSize (void);
static void updtRowNumsView (const u16_t);

static void updtColNamesView (const u16_t);
static void compareCurrVsOld (const u16_t, const u16_t, const u16_t, i16_t*, i16_t*, void (*updater)(const u16_t));

static void updtPosTxt (const u16_t, const u16_t);

void tui__drawLayout ()
{
	printf("\x1b[H\x1b[J\x1b[?25l");
	getWinSize();

	// TODO: adjust to personalizated column width
	nRows_ = WVBytes_ - ROWSALRDUSED_;
	nCols_ = (WHBytes_ - LEFTMARGIN_) / COLWIDTH_;

	updtRowNumsView(0);
	updtColNamesView(0);

	tui__moveCursor(0, 0, "          ");
}

void tui__moveCursor (const u16_t rowPos, const u16_t colPos, const char *const src)
{
	static u16_t putxat_old, putyat_old, oldCol = 0, oldRow = 0;
	static i16_t relCol = 0, updtCol = 0, relRow = 0, updtRow = 0;

	static char *src_old = NULL;
	updtPosTxt(rowPos, colPos);

	compareCurrVsOld(rowPos, oldRow, nRows_, &relRow, &updtRow, updtRowNumsView);
	compareCurrVsOld(colPos, oldCol, nCols_, &relCol, &updtCol, updtColNamesView);

	const u16_t putxat = LEFTMARGIN_ + relCol * COLWIDTH_;
	const u16_t putyat = ROWSALRDUSED_ + relRow;

	if (src_old)
	{ printf("\x1b[%d;%dH\x1b[0m%-*s", putyat_old, putxat_old, COLWIDTH_, src_old); }

	printf("\x1b[%d;%dH\x1b[48;5;106m%-*s\x1b[0m", putyat, putxat, COLWIDTH_, src);
	fflush(stdout);

	putyat_old = putyat;
	putxat_old = putxat;
	oldCol     = colPos;
	oldRow     = rowPos;
	src_old    = (char*) src;
}

void tui__updateSheetName (const char *name)
{
	printf("\x1b[%d;0H[%-10.5s]", WVBytes_, name);
	fflush(stdout);
}

static void getWinSize (void)
{
	struct winsize wsz;
	ioctl(FD_SIN, TIOCGWINSZ, &wsz);

	WVBytes_ = wsz.ws_row;
	WHBytes_ = wsz.ws_col;
}

static void updtRowNumsView (const u16_t from)
{
	static const char *const nums =
	" 0   \n"" 1   \n"" 2   \n"" 3   \n"" 4   \n"" 5   \n"" 6   \n"" 7   \n"
	" 8   \n"" 9   \n"" 10  \n"" 11  \n"" 12  \n"" 13  \n"" 14  \n"" 15  \n"
	" 16  \n"" 17  \n"" 18  \n"" 19  \n"" 20  \n"" 21  \n"" 22  \n"" 23  \n"
	" 24  \n"" 25  \n"" 26  \n"" 27  \n"" 28  \n"" 29  \n"" 30  \n"" 31  \n"
	" 32  \n"" 33  \n"" 34  \n"" 35  \n"" 36  \n"" 37  \n"" 38  \n"" 39  \n"
	" 40  \n"" 41  \n"" 42  \n"" 43  \n"" 44  \n"" 45  \n"" 46  \n"" 47  \n"
	" 48  \n"" 49  \n"" 50  \n"" 51  \n"" 52  \n"" 53  \n"" 54  \n"" 55  \n"
	" 56  \n"" 57  \n"" 58  \n"" 59  \n"" 60  \n"" 61  \n"" 62  \n"" 63  \n"
	" 64  \n"" 65  \n"" 66  \n"" 67  \n"" 68  \n"" 69  \n"" 70  \n"" 71  \n"
	" 72  \n"" 73  \n"" 74  \n"" 75  \n"" 76  \n"" 77  \n"" 78  \n"" 79  \n"
	" 80  \n"" 81  \n"" 82  \n"" 83  \n"" 84  \n"" 85  \n"" 86  \n"" 87  \n"
	" 88  \n"" 89  \n"" 90  \n"" 91  \n"" 92  \n"" 93  \n"" 94  \n"" 95  \n"
	" 96  \n"" 97  \n"" 98  \n"" 99  \n"" 100 \n"" 101 \n"" 102 \n"" 103 \n"
	" 104 \n"" 105 \n"" 106 \n"" 107 \n"" 108 \n"" 109 \n"" 110 \n"" 111 \n"
	" 112 \n"" 113 \n"" 114 \n"" 115 \n"" 116 \n"" 117 \n"" 118 \n"" 119 \n"
	" 120 \n"" 121 \n"" 122 \n"" 123 \n";

	const u16_t nrows2print = LEFTMARGIN_ * (WVBytes_ - ROWSALRDUSED_);
	const char *rows = nums + (LEFTMARGIN_ * from);

	printf("\x1b[4;0H%.*s", nrows2print, rows);
	fflush(stdout);
}

static void updtColNamesView (const u16_t from)
{
	static const char *const names =
	"    A         B         C         D         E         F         G     "
	"    H         I         J         K         L         M         N     "
	"    O         P         Q         R         S         T         U     "
	"    V         W         X         Y         Z         AA        AB    "
	"    AC        AD        AE        AF        AG        AH        AI    "
	"    AJ        AK        AL        AM        AN        AO        AP    "
	"    AQ        AR        AS        AT        AU        AV        AW    "
	"    AX        AY        AZ        BA        BB        BC        BD    "
	"    BE        BF        BG        BH        BI        BJ        BK    "
	"    BL        BM        BN        BO        BP        BQ        BR    "
	"    BS        BT        BU        BV        BW        BX        BY        BZ    ...";

	const u16_t ncols2print = WHBytes_ - LEFTMARGIN_;
	const char *cols = names + (COLWIDTH_ * from);

	printf("\x1b[3;0H     %.*s\x1b[0K", ncols2print, cols);
	fflush(stdout);
}

static void compareCurrVsOld (const u16_t curr, const u16_t old, const u16_t lim, i16_t *rel, i16_t *updtn, void (*updater)(const u16_t))
{
	if (curr > old) *rel += 1;
	if (curr < old) *rel -= 1;

	if (*rel == lim) {
		*updtn += 1;
		updater(*updtn);
		*rel = lim - 1;
	}
	if (*rel == -1) {
		*updtn -= 1;
		updater(*updtn);
		*rel = 0;
	}
}

static void updtPosTxt (const u16_t row, const u16_t col)
{
	char colName[3] = {0, 0, 0};

	if (col < 26)		{ colName[0] = 'A' + col; }
	else if (col < 52)	{ colName[0] = 'A'; colName[1] = 'A' + (col - 26); }
	else if (col < 78)	{ colName[0] = 'B'; colName[1] = 'A' + (col - 52); }

	printf("\x1b[0;0H\x1b[0K%s%d", colName, row);
}
