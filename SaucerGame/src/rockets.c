#include <curses.h>
#include <stdio.h>
#include <string.h>

#include "locks.h"
#include "rockets.h"
#include "scores.h"


static void 	initRocket(struct rocket *);
static void 	animateRocket(struct rocket *);

/*
 * sets each rockets isAlive field to 0
 */
void setRocketsToDead(struct rocket * rockets) {
	int i;
	for (i = 0; i < MAX_ROCKETS; i++)
		rockets[i].isAlive = 0;
}

/*
 * makes the appropriate calls to initialize the rocket when it is being 
 * created and animate it
 */
void *setupRocket(void *arg) {
	struct rocket *r = arg;
		
	initRocket(r);
	animateRocket(r);
}

/*
 * initializes the rocket with the appropriate starting attributes
 */
void initRocket(struct rocket *r) {
	strncpy(r->message, ROCKET, ROCKET_LEN);
	r->length = ROCKET_LEN;
	r->row = LINES - 3;
	r->delay = 1;
	r->dir = -1; 		/* moving up! */
	r->hit = 0;
	r->isAlive = 1;
}

/*
 * animates the rocket until it dies
 */
void animateRocket(struct rocket *r) {
	int len = r->length;

	/*
	 * Draw the rocket initially
	 */
	pthread_mutex_lock(&mx);
	move(r->row, r->col);
	addstr(r->message);
	move(LINES-1, COLS-1);
	refresh();
	pthread_mutex_unlock(&mx);
	
	while (1) {
		/*
		 * Now erase the previous and draw the next
		 */
		usleep(r->delay*TUNIT);

		pthread_mutex_lock(&mx);
		move(r->row, r->col);
		addch(' ');	
		r->row += r->dir;
		move(r->row, r->col);
		addstr(r->message);
		move(LINES-1, COLS-1);
		refresh();
		pthread_mutex_unlock(&mx);

		if (r->hit == 1) {
			pthread_mutex_lock(&mx);
			move(r->row, r->col);
			addch(' ');
			move(LINES-1, COLS-1);	
			refresh();
			pthread_mutex_unlock(&mx);

			rocketsLeft++;
			score++;
			displayInfo();
			break;
		}

		if (r->row <= 0) {
			pthread_mutex_lock(&mx);
			move(r->row, r->col);
			addch(' ');	
			move(LINES-1, COLS-1);
			refresh();
			pthread_mutex_unlock(&mx);
			break;
		}
	}
	r->isAlive = 0;
	pthread_exit(NULL);
}
