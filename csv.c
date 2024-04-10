#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv.h"
#include "io.h"

char *destroi_str (char *str) {
	if (str) free(str);
	return NULL;
}

rotulo *le_cabecalho (char *conteudo_arquivo) {
	int colunas = conta_colunas_arquivo (conteudo_arquivo);
	
	if (colunas <= 0)	return NULL;
	rotulo *cab = (rotulo *) malloc(colunas*sizeof(rotulo));

	if (cab) {
		//copiando o cabecalho em uma nova string
		char *inicio_cab = conteudo_arquivo;
		char *final_cab = strchr (conteudo_arquivo, '\n');			
		int tamanho = final_cab - inicio_cab;
		char *cab_str = (char*) malloc((tamanho+1) * sizeof(char));
		strncpy(cab_str, conteudo_arquivo, tamanho);
		cab_str[tamanho] = '\0';

		int i = 0;
		char *tok = strtok(cab_str, ",");
		while (tok && i < colunas) {
			cab[i].nome = strdup(tok);
			cab[i].tipo = STR;
			cab[i].enable = EN;
			tok = strtok(NULL, ",");
		}

		cab_str = destroi_str (cab_str);
	}

	return cab;
}
