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

extern struct destroyerRocket destRocket;

static void	animateDestRocket();
static void 	initDestRocket(struct destroyer *);

/*
 * creates a destroyer rocket at random times, as long as the destroyer is
 * alive. 
 */
void *shootDestRocket(void *args) {
	struct destroyer *ship = args;
	srand(getpid());

	while (1) {
		if (!destRocket.isAlive && destRocket.hit < 1) {
			sleep(1 + (rand()%3));
				
			initDestRocket(ship);
			animateDestRocket();
		}
	}
}	

/*
 * initializes the destroyer rocket with the appropriate attributes
 */
void initDestRocket(struct destroyer *ship) {
	strncpy(destRocket.message, DESTROYER_ROCKET, DESTROYER_ROCKET_LEN);
	destRocket.length = DESTROYER_ROCKET_LEN;
	destRocket.row = ship->row + 2;
	destRocket.col = ship->col + 2;
	destRocket.dir = 1;
	destRocket.delay = 2;
	destRocket.hit = 0;
	destRocket.isAlive = 1;
}

/*
 * animates the destroyer rocket with ncurses, until the rocket leaves the
 * screen
 */
void animateDestRocket() {
	/*
	 * draw initial rocket
	 */
	pthread_mutex_lock(&mx);
	move(destRocket.row, destRocket.col);
	addstr(destRocket.message);
	move(LINES-1, COLS-1);
	refresh();
	pthread_mutex_unlock(&mx);

	while (destRocket.isAlive) {
		usleep(destRocket.delay * TUNIT);
		pthread_mutex_lock(&mx);
		move(destRocket.row, destRocket.col);
		addnstr("                   ", DESTROYER_ROCKET_LEN);
		destRocket.row += destRocket.dir;
		move(destRocket.row, destRocket.col);
		addnstr(destRocket.message, DESTROYER_ROCKET_LEN);
		move(LINES-1, COLS-1);
		refresh();
		pthread_mutex_unlock(&mx);

		if (destRocket.row >= LINES-2) {
			eraseDestroyerRocket();
			destRocket.isAlive = 0;
		} else if (destRocket.hit > 0) {
			eraseDestroyerRocket();
			destRocket.isAlive = 0;
		}
	}
}

/*
 * erases the destroyerRocket
 */
void eraseDestroyerRocket() {
	pthread_mutex_lock(&mx);
	move(destRocket.row, destRocket.col);
	addnstr("                   ", 
		DESTROYER_ROCKET_LEN);
	move(LINES-1, COLS-1);
	refresh();
	pthread_mutex_unlock(&mx);
}
