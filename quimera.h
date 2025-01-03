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

#define	u8_t			unsigned char
#define	u16_t			unsigned short
#define	u32_t			unsigned int
#define	u64_t			unsigned long

#define	i8_t			signed char
#define	i16_t			signed short
#define	i32_t			signed int
#define	i64_t			signed long

#define	bool_t			unsigned char
#define	True			1
#define	False			0

#define	success_t		unsigned char
#define	Succeed			1
#define	Failed			0

#define	FD_SIN			0
#define	FD_SOUT			1
#define	FD_SERR			2

#define	DEBUG_P(s, ...)	do { fprintf(stderr, s, __VA_ARGS__); fflush(stderr); } while (0)

struct Sheet
{
	char	*name;
};

struct Quimera
{
	struct	Sheet *currSheet;
	u16_t	row_p, col_p;
	char	*docName;
	bool_t	readOnly;
};

#endif
