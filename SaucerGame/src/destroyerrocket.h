#ifndef _DESTROYERROCKET_H_
#define _DESTROYERROCKET_H_
#include <stdlib.h>

#define DESTROYER_ROCKET "  vvvvvvvvvvvvvvv  "
#define DESTROYER_ROCKET_LEN 19

struct destroyerRocket {
	char	message[DESTROYER_ROCKET_LEN];
	int	length;
	int	row;
	int	col;
	int 	dir;
	int	delay;
	int 	hit;
	int 	isAlive;
};

void eraseDestroyerRocket();
void *shootDestRocket(void *);

#endif

