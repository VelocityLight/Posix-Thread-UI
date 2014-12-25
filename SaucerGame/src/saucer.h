#ifndef _SAUCER_H_
#define _SAUCER_H_
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <pthread.h>
#include <time.h>

#define SAUCER "<--->"
#define SAUCER_LEN 5
#define MAX_SAUCERS 200

struct	saucer {
	char	message[SAUCER_LEN];	/* the message */
	int 	length;		/* the saucer length */
	int	row;			/* the row     */
	int	col;
	int	delay;  		/* delay in time units */
	int	hit;
	int	isAlive;		/* +1 or -1	*/
};

void initSaucer(struct saucer *, int, int);
void *animateSaucer(void *);

#endif
