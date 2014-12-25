#include 	"locks.h"
#include 	"saucer.h"
#include 	"scores.h"

#define TUNIT 20000		/* time units in microseconds */

pthread_mutex_t	escaped = PTHREAD_MUTEX_INITIALIZER; 

/**
 * initialize all the saucers with 
 * - message
 * - length
 * - speed
 * - isAlive = 0 (dead)
 */
void initSaucer(struct saucer *ship, int row, int delay) {
	strncpy(ship->message, SAUCER, SAUCER_LEN);
	ship->length = SAUCER_LEN;
	ship->row = row;
	ship->col = 0;
	ship->delay = delay;
	ship->hit = 0;
	ship->isAlive = 1;
}

/*
 * animates the saucer until it is shot or leaves the scream
 */
void *animateSaucer(void *arg) {
	struct saucer *ship = arg;
	int len = ship->length+2;
	//int col = 0;	/* might need to review this */

	while (1) {
		usleep(ship->delay*TUNIT);
	
		pthread_mutex_lock(&mx);	
		move(ship->row, ship->col);
		addch(' ');
		addstr(ship->message);
		addch(' ');
		move(LINES-1, COLS-1);
		refresh();	
		pthread_mutex_unlock(&mx);

		ship->col += 1;

		/*
		 * CODE REFERENCED FROM JAMES HODGSON
		 */
		if (ship->col + len >= COLS) {
			ship->message[len-1] = '\0';
			len = len-1;
		}

		if (ship->hit) {
			pthread_mutex_lock(&mx);
			move(ship->row, ship->col-1);
			addch(' ');
			addch(' ');
			addch(' ');
			addch(' ');
			addch(' ');
			addch(' ');
			move(LINES-1, COLS-1);
			refresh();
			pthread_mutex_unlock(&mx);
			break;
		}
		if (ship->col >= COLS) {
			pthread_mutex_lock(&escaped);
			noEscaped++;	
			pthread_mutex_unlock(&escaped);
			displayInfo();
			break;
		}
	}
	
	ship->isAlive = 0;
	pthread_exit(NULL);
}
