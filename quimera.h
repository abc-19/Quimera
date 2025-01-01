/*              ___
 *           .="   "=._.---.		Sheet - program's structure.
 *         ."         c ' Y'`p		abc-19
 *        /   ,       `.  w_/
 *    jgs |   '-.   /     / 
 *  _,..._|      )_-\ \_=.\
 * `-....-'`------)))`=-'"`'"		Jan 01 2025
 */
#ifndef	QUIM_H
#define	QUIM_H

#include <stdio.h>

#define	uint8_t		unsigned char
#define	uint16_t	unsigned short
#define	uint32_t	unsigned int
#define	uint64_t	unsigned long

#define	int8_t		signed char
#define	int16_t		signed short
#define	int32_t		signed int
#define	int64_t		signed long

#define	bool_t		unsigned char
#define	True		1
#define	False		0

#define	success_t	unsigned char
#define	Succeed		1
#define	Failed		0

struct Sheet
{
	struct	Sheet *next, *prev;
	char	*name;
};

struct Quimera
{
	struct	*currSheet;
	char	*docName;
};

#endif
