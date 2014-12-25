#ifndef _DESTROYER_H_ 
#define _DESTROYER_H_
#include <stdlib.h>

#define DESTROYER_TOP "  -------^-------  "
#define DESTROYER_BOT "<[_______________]>"
#define DESTROYER_LEN 19
#define	DESTROYER_HEALTH 5 
#define MAX_DESTROYERS 1

struct	destroyer {
	char	topMessage[DESTROYER_LEN];	/* the  top of the destroyer */
	char	botMessage[DESTROYER_LEN];	/* the bottom of the destroyer */
	int 	length;				/* the saucer length */
	int	row;				/* the row     */
	int	col;				/* current col */
	int	delay;  			/* delay in time units */
	int	dir;
	int	hit;				/* how many times it has been hit */
	int	isAlive;			/* +1 or 0 	*/
};

void 	*sendDestroyer(void *); 
#endif
