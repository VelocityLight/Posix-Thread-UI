CC := cc
FLAGS := -lcurses -lpthread

all:		scores.o saucer.o rockets.o destroyer.o destroyerrocket.o cannon.o main.o
	$(CC) main.o cannon.o destroyer.o destroyerrocket.o rockets.o saucer.o scores.o $(FLAGS) -o game

destroyer.o:	locks.h scores.h destroyer.h destroyerrocket.h destroyer.c
	$(CC) -c destroyer.c $(FLAGS) -o destroyer.o

destroyerrocket.o:	locks.h scores.h destroyer.h destroyerrocket.h destroyerrocket.c
	$(CC) -c destroyerrocket.c $(FLAGS) -o destroyerrocket.o

scores.o:	locks.h scores.h scores.c
	$(CC) -c scores.c $(FLAGS) -o scores.o

rockets.o:	locks.h rockets.h scores.h rockets.c
	$(CC) -c rockets.c $(FLAGS) -o rockets.o

cannon.o:	locks.h cannon.h cannon.c
	$(CC) -c cannon.c $(FLAGS) -o cannon.o

saucer.o:	locks.h saucer.h scores.h saucer.c
	$(CC) -c saucer.c $(FLAGS) -o saucer.o

main.o: 	destroyer.h destroyerrocket.h rockets.h locks.h cannon.h saucer.h scores.h main.c
	$(CC) -c main.c $(FLAGS) -o main.o	

clean:
	rm -rf *.o game
