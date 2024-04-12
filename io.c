#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"

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
