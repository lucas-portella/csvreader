#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv.h"
#include "estatistica.h"
#include "io.h"

int main (int argc, char **argv) {
	if (argc != 2)	return 1;
	char *arquivo = argv[1];
 
	tabela_csv *tab = inicializa_tabela_csv (arquivo);
	printf(" ==== sumario ==== \n");
	sumario(tab);
	printf(" ==== impressao ==== \n");
	imprime(tab);
	printf(" ==== filtro ====\n");
	filtra(tab);
	printf(" ==== selecao ====\n");
	selecao(tab);
	printf (" ==== ordenacao ====\n");
	ordena(tab);
	printf ("==== descricao =====\n");
	descricao(tab);
	tab = destroi_tabela_csv (tab);
	return 0;
}
