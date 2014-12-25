#include <string.h>
#include <stdio.h>
#include <curses.h>
#include <pthread.h>
#include <time.h>
#include "locks.h"
#include "destroyer.h"
#include "destroyerrocket.h"
#include "scores.h"

#define TUNIT 20000		/* time units in microseconds */


static void 	initDestroyer(struct destroyer *, int, int);
static void 	animateDestroyer(pthread_t, struct destroyer *);

/*
 * sends a new destroyer at random intervals, if the destroyer is not already
 * on the screen
 *
 * limited to one destroyer at a time.
 */
void *sendDestroyer(void *arg) {
	struct destroyer *ship = arg;
	pthread_t destRocketThread;
	srand(getpid());

	while(1) {
		if (!ship->isAlive) {
			sleep(10 + (rand()%20));
		
			// initialize the destroyer
			initDestroyer(ship, rand()%15, 1+(rand()%10));
			pthread_create(&destRocketThread, NULL, 
			    shootDestRocket, ship);
			animateDestroyer(destRocketThread, ship);
		}	
	}
}

/*
 * initialize the destroyer with the appropriate starting attributes
 */
void initDestroyer(struct destroyer *ship, int row, int delay) {
	strncpy(ship->topMessage, DESTROYER_TOP, DESTROYER_LEN);
	strncpy(ship->botMessage, DESTROYER_BOT, DESTROYER_LEN);
	ship->length = DESTROYER_LEN;
	ship->row = row;
	ship->col = 0;
	ship->dir = 1;
	ship->delay = delay;
	ship->hit = 0;
	ship->isAlive = 1;
}

/*
 * animate the destroyer until it is hit by a rocket
 */ 
void animateDestroyer(pthread_t thread, struct destroyer *ship) {
	int i;

	while (1) {
		usleep(ship->delay*TUNIT);
		
		pthread_mutex_lock(&mx);
		move(ship->row, ship->col);
		addch(' ');
		addnstr(ship->topMessage, DESTROYER_LEN);
		addch(' ');
		move(LINES-1, COLS-1);
		refresh();

		move(ship->row + 1, ship->col);
		addch(' ');
		addnstr(ship->botMessage, DESTROYER_LEN);
		addch(' ');
		move(LINES-1, COLS-1);
		refresh();

		pthread_mutex_unlock(&mx);
		
		/*
		 * perform checks;
		 */
		if ((ship->col + ship->length + 2) >= COLS && ship->dir == 1)
			ship->dir = -1;

		if (ship->col <= 0 && ship->dir == -1) 
			ship->dir = 1;
		
		if (ship->hit > 100) {
			pthread_mutex_lock(&mx);
			move(ship->row, ship->col);
			for (i=0; i <= ship->length; i++)
				addch(' ');
			move(ship->row + 1, ship->col);
			for (i=0; i <= ship->length; i++)
				addch(' ');
			move(LINES-1, COLS-1);
			refresh();
			pthread_mutex_unlock(&mx);
			break;
		}
		ship->col += ship->dir;
	}

	ship->isAlive = 0;
	eraseDestroyerRocket();		
	pthread_cancel(thread);
}
