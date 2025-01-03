/*              ___
 *           .="   "=._.---.		Sheet - Terminal user interface.
 *         ."         c ' Y'`p		abc-19
 *        /   ,       `.  w_/
 *    jgs |   '-.   /     / 
 *  _,..._|      )_-\ \_=.\
 * `-....-'`------)))`=-'"`'"		Jan 02 2025
 */
#ifndef	QUIM_TUI_H
#define	QUIM_TUI_H
#include "quimera.h"

void tui__drawLayout ();
void tui__moveCursor (const u16_t, const u16_t, const char *const);

void tui__updateSheetName (const char*);

#endif
