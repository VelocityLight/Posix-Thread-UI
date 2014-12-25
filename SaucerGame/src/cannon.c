#include 	<curses.h>
#include 	<stdio.h>
#include	<string.h>

#include 	"cannon.h"
#include	"locks.h"

#define TUNIT 20000

static struct cannon	cn;
static struct cannon	cn2;

/*
 * initializes the cannon
 */
void setupCannon(int players) {
	int col;
	if (players == 1) {
		col = ((0 + COLS-1)/2);
	} else {
		col = ((0 + COLS-1)/4);

		strncpy(cn2.message, CANNON, CANNON_LEN);
		cn2.length = CANNON_LEN;
		cn2.col = col;
		cn2.hit = 0;
		cn2.delay = 1;

		col *= 3;
	}

	strncpy(cn.message, CANNON, CANNON_LEN);
	cn.length = CANNON_LEN;
	cn.col = col;
	cn.hit = 0;
	cn.delay = 1;
}

/*
 * returns the players cannons column
 */
int getCannonCol(int player) {
	if (player == 1) {
		return cn.col;
	} else {
		return cn2.col;
	}
}

/* 
 * returns if player's cannon has been hit
 */
int getCannonHit(int player) {
	if (player == 1) {
		return cn.hit;
	} else {
		return cn2.hit;
	}
}

/*
 * sets the player's cannon to hit
 */
void setCannonHit(int player) {
	if (player == 1) {
		cn.hit = 1;
	} else {
		cn2.hit = 1;
	}
}

/*
 * moves the specific player's cannon in the direction given
 */
void moveCannon(int dir, int player) {
	if (player == 1) {
		cn.col += dir;
		if (cn.col >= COLS || cn.col <= 0)
			cn.col -= dir;
	} else {
		cn2.col += dir;
		if (cn2.col >= COLS || cn2.col <= 0)
			cn2.col -= dir;
	}	

	displayCannon(player);
}

/*
 * displays the cannon on the screen
 */
void displayCannon(int player) {
	usleep(cn.delay*TUNIT);
	pthread_mutex_lock(&mx);
	
	if (player == 1) {
		move(LINES-2, cn.col-1);
		addch(' ');
		addstr(cn.message);
		addch(' ');
	} else {
		move(LINES-2, cn2.col-1);
		addch(' ');
		addstr(cn2.message);
		addch(' ');
	}

	move(LINES-1, COLS-1);
	refresh();
	pthread_mutex_unlock(&mx);
}
