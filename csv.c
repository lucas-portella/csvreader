#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv.h"
#include "io.h"

char *destroi_str (char *str) {
	if (str) free(str);
	return NULL;
}

// retorna o indice para primeira posicao 
// da string apos o cabecalho
int quebra_cabecalho (char *conteudo_arquivo) {
	if (!conteudo_arquivo)	return -1;
	
	int i = 0;
	while (conteudo_arquivo[i] != '\0' &&
	conteudo_arquivo[i] != '\n')
		i++;

	if (conteudo_arquivo[i] == '\n') {
		conteudo_arquivo[i] = '\0';
		return i+1;
	}
	else
		return -2;
}

// troca '\n' da string por ','
void trata_string (char *str) {
	if (!str) return;

	int i = 0;
	while (str[i] != '\0') {
		if (str[i] == '\n')
			str[i] = ',';
		i++;
	}
}

rotulo *destroi_cabecalho (rotulo *cab, int colunas) {
	if(!cab) return NULL;
	
	for (int i = 0; i < colunas; i++)
		cab[i].nome = destroi_str(cab[i].nome);
	free(cab);
	return NULL;
}

// recebe uma string apenas com o conteudo dos rotulos
rotulo *le_cabecalho (char *str_rot, int colunas) {

	if (colunas <= 0 || !str_rot)	return NULL;
	rotulo *cab = (rotulo *) malloc(colunas*sizeof(rotulo));

	if (cab) {
		trata_string(str_rot);
		int i = 0;
		char *tok = strtok(str_rot, ",");
		while (tok && i < colunas) {
			cab[i].nome = strdup(tok);
			cab[i].tipo = STR;
			cab[i].enable = EN;
			tok = strtok(NULL, ",");
			i++;
		}
	}

	return cab;
}

char *inicializa_vet_enable (int linhas) {
	if (linhas <= 0) return NULL;

	char *enable = (char*) malloc (linhas * sizeof(char));

	if (enable) for (int i = 0; i < linhas; enable[i++] = EN);

	return enable;
}

char *destroi_vet_enable (char *enable) {
	return destroi_str(enable);	
}

char ***aloca_tabela (int linhas, int colunas) {
	if (linhas <= 0 || colunas <= 0) return NULL;

	char ***tab;
	tab = (char***) malloc(linhas*sizeof(char**));
	if (tab) {
		for (int i = 0; i < linhas; i++)
			tab[i] = (char**) malloc(colunas * sizeof (char*));
	}

	//inicializando dados com NULL
	for (int i = 0; i < linhas; i++) {
		for (int j = 0; j < colunas; j++)
			tab[i][j] = NULL;
	}
	
	return tab;
}

char ***destroi_tabela (char ***tab, int linhas, int colunas) {
	if (!tab || linhas <= 0 || colunas <= 0) return NULL;

	for (int i = 0; i < linhas; i++) {
		for (int j = 0; j < colunas; j++) 
			tab[i][j] = destroi_str (tab[i][j]);
		free(tab[i]);
		tab[i] = NULL;
	}	
	free(tab);
	return NULL;
}

char ***inicializa_tabela (char *str_dados, int linhas, int colunas) {
	if (!str_dados || linhas <= 0 || colunas <=0)
		return NULL;

	char ***tab = aloca_tabela (linhas, colunas);
	if (!tab)	return NULL;

	trata_string(str_dados);

	int i = 0, j = 0, k = 0;
	char *tok;
	while (i < linhas) {
		while (j < colunas) {

			int w;
			for (w = k; str_dados[w] != '\0' && str_dados[w] != ','; w++);
			if (str_dados[w] == ',') {
				str_dados[w] = '\0';
				tok = &str_dados[k];
				k = w+1;
			} else tok = NULL;
			if (tok)	tab[i][j] = strdup(tok);

			j++;
		}
		i++;
		j = 0;
	}

	return tab;
}

tabela_csv *aloca_tabela_csv () {
	return (tabela_csv*) malloc (sizeof(tabela_csv));
}

tabela_csv *inicializa_tabela_csv (const char *nome_arquivo) {
	if (!nome_arquivo)	return NULL;

	tabela_csv *tab = aloca_tabela_csv();
	if (!tab)	return NULL;

	int linhas = conta_linhas_arquivo (nome_arquivo) - 1;
	int colunas = conta_colunas_arquivo (nome_arquivo);
	char *cabecalho = le_arquivo (nome_arquivo);
	int inicio_dados = quebra_cabecalho (cabecalho);
	char *dados = &cabecalho[inicio_dados];

	tab->cabecalho = le_cabecalho (cabecalho, colunas);
	tab->tabela = inicializa_tabela (dados, linhas, colunas);
	tab->enable = inicializa_vet_enable (linhas);
	tab->linhas = linhas;
	tab->colunas = colunas;

	cabecalho = destroi_str (cabecalho);

	return tab;
}

tabela_csv *destroi_tabela_csv (tabela_csv *tab) {
	if (!tab)	return NULL;
	
	tab->cabecalho = destroi_cabecalho (tab->cabecalho, tab->colunas);
	tab->tabela = destroi_tabela (tab->tabela, tab->linhas,
	tab->colunas);
	tab->enable = destroi_vet_enable(tab->enable);
	free(tab);
	return NULL;
}
