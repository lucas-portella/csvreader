CC=gcc
FLAGS=-Wall -Wextra

teste: teste.c io.o csv.o
	$(CC) $(FLAGS) io.o csv.o teste.c -o teste

io.o: io.c io.h
	$(CC) $(FLAGS) -c io.c

csv.o: csv.h csv.c
	$(CC) $(FLAGS) -c csv.c

clean:
	rm -f *.o

purge:
	rm -f teste
purge: clean
