#ifndef _CANNON_H_
#define _CANNON_H_
#include <stdlib.h>

#define CANNON "|"
#define CANNON_LEN 1

struct	cannon {
	char	message[CANNON_LEN];
	int	length;
	int	col;
	int	hit;
	int	delay;
};

void	setupCannon(int);
int 	getCannonCol(int);
int		getCannonHit(int);
void	setCannonHit(int);
void	moveCannon(int, int);
void	displayCannon(int);

#endif
