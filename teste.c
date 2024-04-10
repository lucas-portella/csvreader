#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv.h"
#include "io.h"

int main () {
	char *arquivo = "teste.csv";
	int colunas = conta_colunas_arquivo (arquivo);
	rotulo *cab = le_cabecalho (arquivo);

	int i = 0;
	while (i < colunas) {
		printf("%s %c %c\n", cab[i].nome, cab[i].tipo, cab[i].enable);	
		i++;
	}
	return 0;
}
