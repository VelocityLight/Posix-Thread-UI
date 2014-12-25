#include	<curses.h>
#include 	<stdio.h>

#include	"locks.h"
#include 	"scores.h"

#define	BUFFLEN 256

static void 	eraseInfo();

void displayInfo () {
	char info[BUFFLEN];
	sprintf(info, "Press 'Q' to quit | ESCAPED: %d | ROCKETS: %d "
		"| SCORE: %d", noEscaped, rocketsLeft, score);

	pthread_mutex_lock(&mx);

	eraseInfo();
	move(LINES - 1, 0);
	addstr(info);
	move(LINES-1, COLS-1);
	refresh();
	pthread_mutex_unlock(&mx);
}

void eraseInfo() {
	int i;
	move(LINES-1, 0);
	for (i=0; i <= COLS-1; i++) {
		addch(' ');
	}
	move(LINES-1, COLS-1);
	refresh();
}
