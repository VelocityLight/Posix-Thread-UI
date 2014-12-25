#include	<stdio.h>
#include	<curses.h>
#include	<pthread.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<string.h>

#include	"cannon.h"
#include	"destroyer.h"
#include 	"locks.h"
#include	"rockets.h"
#include 	"saucer.h"
#include 	"scores.h"

#include 	"destroyerrocket.h"

#define ENDGAME 15		/* end game condition */
#define	TUNIT   20000		/* timeunits in microseconds */

/*
 * Static functions
 */
static void 	setup();
static void	shootRocket(int);
static void 	*collisionDetection();
static void 	compareCoords(int i, int j); 
static void 	printEndGameMessage(char *);
static void 	printStartMessage();
static void 	*checkEndConditions();
static void 	*checkDestRocketCollision();

/*
 * mutex lock that is used to monitor curses operations
 */
pthread_mutex_t mx = PTHREAD_MUTEX_INITIALIZER;

/*
 * rocketThreads is an arry of pthreads used for each rocket
 * rockets is an array of structures used for each rocket 
 */
pthread_t	rocketThreads[MAX_ROCKETS];
struct rocket 	rockets[MAX_ROCKETS];

/*
 * sThread is an array of pthreads used for each saucer
 * saucers is an array of structuresused for each saucer
 */
pthread_t	sThread[MAX_SAUCERS];
struct saucer saucers[MAX_SAUCERS];

/*
 * destroyerThread is a single thread used for the destroyer saucer
 *
 * destShip is a structure that is used for the destroyer
 * destRocket is a structure that is used for the destroyers rocket
 */
pthread_t	destroyerThread;
struct destroyer destShip;
struct destroyerRocket destRocket;

/*
 * saucerSetup is a pthread used for generating saucers at random intervals
 */
pthread_t	saucerSetup;

/*
 * Global flags used for end-game conditions
 */
int 	noEscaped = 0;
int 	endGame = 0;
int	rocketsLeft = MAX_ROCKETS;
int	score = 0;

/*
 * The main function does all the game setup which includes creating all of
 * the threads.
 * The main functions main use is to receive input until the game has ended.
 */
int main(int ac, char *av[])
{
	int	       	i, c, quitFlag, twoPlayer;		
	pthread_t	collisionThread;
	pthread_t	destCollisionThread;
	pthread_t	gameMonitor;
	void	      	*animate();	
	
	quitFlag = 0;

	setup();
	
	/*
	 * Display start screen, have the user press 'S' to start
	 * the game.
	 */
	printStartMessage();
	while (1) {
		c = getch();
		if (c == '1') {
			twoPlayer = 0;
			clear();
			refresh();
			break;
		} else if (c == '2') {
			twoPlayer = 1;
			clear();
			refresh();
			break;
		} else if (c == 'Q') {
			quitFlag = 1;
			break;
		}
	}	
		

	if (twoPlayer) {
		setupCannon(2);	
		displayCannon(2);
	} else {
		setupCannon(1);
	}
	
	setRocketsToDead(rockets);
	displayCannon(1);
	displayInfo();

	if (pthread_create(&saucerSetup, NULL, setupSaucer, NULL)) {
		endwin();
		exit(0);
	} 	
	pthread_create(&destroyerThread, NULL, sendDestroyer, &destShip); 
	pthread_create(&collisionThread, NULL, collisionDetection, NULL);
	pthread_create(&destCollisionThread, NULL, checkDestRocketCollision, NULL);
	pthread_create(&gameMonitor, NULL, checkEndConditions, NULL);

	/* process user input */
	while(!quitFlag) {
		c = getch();

		if ( c == 'Q' || quitFlag){
			 quitFlag = 1;
			 break;
		} else if (c == ',' && !endGame) {
			moveCannon(-1, 1);
		} else if (c == '.' && !endGame) {
			moveCannon(1, 1);
		} else if (c == 'x' && !endGame) {
			moveCannon(-1, 2);
		} else if (c == 'c' && !endGame) {
			moveCannon(1, 2);
		} else if (c == 'z' && !endGame) {
			shootRocket(2);
		} else if (c == ' ' && !endGame) {
			shootRocket(1);	
		}	
	}

	/* cancel all the threads */
	pthread_mutex_lock(&mx);
	for (i=0; i < MAX_ROCKETS; i++) {
		pthread_cancel(rocketThreads[i]);
	}
	pthread_cancel(collisionThread);
	pthread_cancel(destCollisionThread);
	pthread_cancel(gameMonitor);
	endwin();
	return 0;
}

/*
 * checkEndConditions runs on a separate thread, called gameMonitor, and 
 * it's main purpose is to constantly check. If the game has reached a 
 * state which is deemed game-ending
 */
void *checkEndConditions() {
	int i, liveRockets;

	while (1) {
		if (noEscaped >= ENDGAME) {
			endGame = 1;
			printEndGameMessage("TOO MANY SAUCERS ESCAPED!");
			break;
		} else if (getCannonHit(1) || getCannonHit(2)) {
			endGame = 1;
			printEndGameMessage("YOUR DEFENSES WERE DESTROYED!");
			break;
		} else if (rocketsLeft <= 0) {
			liveRockets = 0;
			for (i = 0; i < MAX_ROCKETS; i++) {
				if (rockets[i].isAlive == 1) {
					liveRockets = 1;
				}
			}
			if (liveRockets != 1) {
				endGame = 1;
				printEndGameMessage("RAN OUT OF ROCKETS!");
				break;
			}
		}
	}

	for (i = 0; i < MAX_SAUCERS; i++) {
		pthread_cancel(sThread[i]);
	}
	pthread_cancel(destroyerThread);
	pthread_cancel(saucerSetup);
}

/*
 * prints the starting menu
 */
void printStartMessage() {
	char *titleMsg = "---SAUCER INVADERS!---";
	char *playerOne = "PLAYER ONE CONTROLS:";
	char *rightMoveMsg = "Press ',' to move right";
	char *leftMoveMsg = "Press '.' to move left";
	char *shootMsg = "Press 'SPACE' to shoot a rocket";
	char *playerTwo = "PLAYER TWO CONTROLS:";
	char *pTwoRightMoveMsg = "Press 'c' to move right";
	char *pTwoLeftMoveMsg = "Press 'x' to move left";
	char *pTwoShootMsg = "Press 'z' to shoot a rocket";
	char *quitMsg = "Press 'Q' to quit";
	char *header = "--------------------------";
	char *startMsg = "Press '1' to start single player game";
	char *pTwoStartMsg = "Press '2' to start two player game";

	pthread_mutex_lock(&mx);
	move(((LINES-1)/4), (((COLS-1)/2)-20));
	addstr(titleMsg);
	move((((LINES-1)/4)+1), (((COLS-1)/2)-20));
	addstr(playerOne);
	move((((LINES-1)/4)+2), (((COLS-1)/2)-20));
	addstr(rightMoveMsg);
	move((((LINES-1)/4)+3), (((COLS-1)/2)-20));
	addstr(leftMoveMsg);
	move((((LINES-1)/4)+4), (((COLS-1)/2)-20));
	addstr(shootMsg);
	move((((LINES-1)/4)+5), (((COLS-1)/2)-20));
	addstr(playerTwo);
	move((((LINES-1)/4)+6), (((COLS-1)/2)-20));
	addstr(pTwoRightMoveMsg);
	move((((LINES-1)/4)+7), (((COLS-1)/2)-20));
	addstr(pTwoLeftMoveMsg);
	move((((LINES-1)/4)+8), (((COLS-1)/2)-20));
	addstr(pTwoShootMsg);
	move((((LINES-1)/4)+9), (((COLS-1)/2)-20));
	addstr(quitMsg);
	move((((LINES-1)/4)+10), (((COLS-1)/2)-20));
	addstr(header);
	move((((LINES-1)/4)+11), (((COLS-1)/2)-20));
	addstr(startMsg);
	move((((LINES-1)/4)+12), (((COLS-1)/2)-20));
	addstr(pTwoStartMsg);

	move(LINES-1, COLS-1);	
	refresh();
	pthread_mutex_unlock(&mx);
}

/*
 * prints the ending message, if you've let too many saucers escape
 */
void printEndGameMessage(char *Message) {
	char *gameOver = "GAME OVER!!!";
	char *quitMessage = "PRESS Q TO QUIT";
	pthread_mutex_lock(&mx);
	move(((LINES-1)/2), (((COLS-1)/2)-10));
	addstr(gameOver);
	move((((LINES-1)/2)+1), (((COLS-1)/2)-10));
	addstr(Message);
	move((((LINES-1)/2)+2), (((COLS-1)/2)-10));
	addstr(quitMessage);
	move(LINES-1, COLS-1);	
	refresh();
	pthread_mutex_unlock(&mx);
}

/*
 * starts a rocket thread, and decrements the rockets left
 */
void shootRocket(int player) {
	int 	i;
	int 	col = getCannonCol(player);
		
	if (rocketsLeft > 0) {	
		rocketsLeft -= 1;
		displayInfo();
		for (i = 0; i < MAX_ROCKETS; i++) {
			if (rockets[i].isAlive == 0) {
				rockets[i].col = col;
				pthread_create(&rocketThreads[i], 
				    NULL, setupRocket, &rockets[i]);	
				break;
			}
		}
	}
			
}

/*
 * setupSaucer runs on a seperate thread and creates new saucers at
 * randome intervals
 */
void *setupSaucer() {
	int num_msg = 1;
	int i;

	/* assign rows and velocities to each string */
	srand(getpid());

	/*
	 * initliaze the saucers
	 */
	for (i = 0; i < MAX_SAUCERS; i++) {
		saucers[i].isAlive = 0;
	}

	while (1) {

		for(i=0 ; i<MAX_SAUCERS; i++){
			if (saucers[i].isAlive == 0) {
				initSaucer(&saucers[i], rand()%15, 1+(rand()%15));
				pthread_create(&sThread[i], NULL, animateSaucer, &saucers[i]);
				break;
			} 
		}

		sleep(1+(rand()%7));
	}
}


/*
 * collisionDetection runs on a separate thread and makes calls to comparCoords
 * to check if any live saucers have collided with any live rockets
 */
void *collisionDetection() {
	int i, j;

	while (1) {
		for (i = 0; i < MAX_SAUCERS; i++) {
			for (j = 0; j < MAX_ROCKETS; j++) {
				compareCoords(i, j);	
			}
		} 
		
	}
}

/*
 * checks if saucer[i] and rocket[j] have collided
 */
void compareCoords(int i, int j) {
	/*
	 * Check here if any coords match
	 */ 
	if (saucers[i].isAlive == 1 
		&& rockets[j].isAlive == 1) {
		if ((saucers[i].row == rockets[j].row 
		    || (saucers[i].row + 1) == rockets[j].row)
		    && rockets[j].col > saucers[i].col
		    && rockets[j].col <= (saucers[i].col + saucers[i].length)) {
			/*
			 * hit
		 	 */
			saucers[i].hit = 1;
			rockets[j].hit = 1;
		}
	}
	
	/*
	 * Check if the rocket has hit the destroyer (only one at a time)
	 */
	if (rockets[j].isAlive == 1 
		&& destShip.isAlive == 1
		&& rockets[j].col >= destShip.col
		&& rockets[j].col <= (destShip.col + destShip.length)
		&& (rockets[j].row == (destShip.row + 1)
		|| rockets[j].row == (destShip.row + 2))) {

                rockets[j].hit = 1;
		destShip.hit += 1;
	}
}

/*
 * checks if a live destroyer rocket has collided with one of the users
 * cannons
 */
void *checkDestRocketCollision() {
	int col;
	while (1) {
		col = getCannonCol(1);

		if (destRocket.isAlive == 1
			&& destRocket.col <= col
			&& (destRocket.col + destRocket.length) >= col
			&& destRocket.row >= LINES-3) {
			
			/*
			 * HIT
			 */
			destRocket.hit = 1;
			setCannonHit(1);			
		}
		
		col = getCannonCol(2);

		if (destRocket.isAlive == 1
			&& destRocket.col <= col
			&& (destRocket.col + destRocket.length) >= col
			&& destRocket.row >= LINES-3) {
			
			/*
			 * HIT
			 */
			destRocket.hit = 1;
			setCannonHit(2);			
		}
	}

}

void setup()
{
	/* set up curses */
	initscr();
	crmode();
	noecho();
	clear();
}
