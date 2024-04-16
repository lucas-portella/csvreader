CC=gcc
FLAGS=-Wall -Wextra

csvreader: csvreader.c io.o csv.o estatistica.o
		$(CC) $(FLAGS) io.o csv.o estatistica.o csvreader.c -o csvreader

io.o: io.c io.h csv.h
	$(CC) $(FLAGS) -c io.c

csv.o: csv.h csv.c estatistica.h
	$(CC) $(FLAGS) -c csv.c

estatistica.o: estatistica.h estatistica.c csv.h
	$(CC) $(FLAGS) -c estatistica.c

clean:
	rm -f *.o

purge:
	rm -f csvreader
purge: clean
