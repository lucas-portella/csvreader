CC=gcc
FLAGS=-Wall -Wextra

csvreader: csvreader.c io.o csv.o
		$(CC) $(FLAGS) io.o csv.o csvreader.c -o csvreader

io.o: io.c io.h csv.h
	$(CC) $(FLAGS) -c io.c

csv.o: csv.h csv.c
	$(CC) $(FLAGS) -c csv.c

clean:
	rm -f *.o

purge:
	rm -f csvreader
purge: clean
