CC=gcc
FLAGS=-Wall -Wextra

teste_leitura: teste.c io.o csv.o teste_leitura.sh
	$(CC) $(FLAGS) io.o csv.o teste.c -o teste_leitura; ./teste_leitura.sh

io.o: io.c io.h
	$(CC) $(FLAGS) -c io.c

csv.o: csv.h csv.c
	$(CC) $(FLAGS) -c csv.c

clean:
	rm -f *.o

purge:
	rm -f teste_leitura
purge: clean
