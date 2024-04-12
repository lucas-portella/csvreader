#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv.h"
#include "io.h"

int main (int argc, char **argv) {
	if (argc != 2)	return 1;
	char *arquivo = argv[1];
 
	tabela_csv *tab = inicializa_tabela_csv (arquivo);

	int i, j;
	for (i = 0; i < tab->colunas-1; i++) 
		printf("%s,", tab->cabecalho[i].nome);
	printf("%s\n", tab->cabecalho[i].nome);

	for (i = 0; i < tab->linhas; i++) {
		for (j = 0; j < tab->colunas - 1; j++) 
			printf("%s,", tab->tabela[i][j]);
		printf("%s\n", tab->tabela[i][j]);
	}

	tab = destroi_tabela_csv (tab);
	return 0;
}
