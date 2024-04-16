#include <stdio.h>
#include <stdlib.h>
#include "csv.h"
#include "estatistica.h"
#include "io.h"

void limpa_buffer () {
	int c;
	do c = getchar();
	while (c != EOF && c != '\n');
}

int conta_linhas_arquivo (const char *nome_arquivo) {
	FILE *f;
	int linhas = 0;
	int c;
	
	if (!nome_arquivo)  return -1;

	f = fopen(nome_arquivo, "r");
	if (!f)	return -2;
	
	c = fgetc(f);
	while (c != EOF) {
		if (c == '\n')	linhas++;
		c = fgetc(f);
	}
	fclose(f);
	f = NULL;

	return linhas;
}

int conta_colunas_arquivo (const char *nome_arquivo) {
	FILE *f;
	int colunas = 1;
	int c;

	if (!nome_arquivo)	return -1;

	f = fopen(nome_arquivo,"r");
	if (!f)	return -2;

	c = fgetc(f);
	while (c != EOF && c != '\n') {
		if (c == ',')	colunas++;	
		c = fgetc(f);
	}
	fclose(f);
	f = NULL;
		
	return colunas;
}

int conta_caracteres_arquivo (const char *nome_arquivo) {
	FILE *f;
	int caracteres = 0;
	int c;

	if (!nome_arquivo)	return -1;

	f = fopen (nome_arquivo, "r");
	if (!f)	return -2;

	c = fgetc(f);
	while(c != EOF)	{
		caracteres++;
		c = fgetc(f);
	}
	fclose(f);
	f = NULL;

	return caracteres;
}

char *le_arquivo (const char *nome_arquivo) {
	FILE *f;
	int c;
	int caracteres = conta_caracteres_arquivo (nome_arquivo);

	if (caracteres <= 0)	return NULL;
	
	char *str = (char*) malloc((caracteres + 1) * sizeof(char));

	if (!str)	return NULL;

	f = fopen(nome_arquivo, "r");
	if (!f)	{
		free(str);
		str = NULL;
		return NULL;
	}

	int i = 0;
	c = fgetc(f);
	while (c != EOF) {
		str[i++] = c;
		c = fgetc(f);
	}
	str[i] = '\0';
	fclose(f);
	f = NULL;

	return str;
}

void cria_arquivo_csv (tabela_csv *t, char *nome_arquivo) {
	if (!t || !nome_arquivo)	return;

	FILE *f = fopen (nome_arquivo, "w");
	if (!f)	return;		

	int i, j;
	int ultima_linha;
	for (i = t->linhas-1; i >=0 && !linha_habilitada(t,i); i--);
	ultima_linha = i;
	int ultima_coluna;
	for (i = t->colunas-1; i>=0 && !coluna_habilitada(t,i); i--);
	ultima_coluna = i;

	for (i = 0; i <= ultima_coluna; i++) {
		if (coluna_habilitada(t,i)) fprintf(f,"%s",t->cabecalho[i].nome);
		if (i != ultima_coluna)	fprintf(f, ",");
	}
	fprintf(f,"\n");	
		
	for (i = 0; i <= ultima_linha; i++) {
		if (linha_habilitada(t,i)) {
			for (j = 0; j <= ultima_coluna; j++) {
				if (coluna_habilitada (t,j)) {
					if (t->tabela[i][j])	fprintf(f,"%s",t->tabela[i][j]);
					if (j != ultima_coluna)	fprintf(f, ",");
				}
			}
			if (i != ultima_linha)	fprintf(f,"\n");
		}
	}

	fclose(f);
	f = NULL;
}
