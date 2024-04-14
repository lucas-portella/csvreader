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

int linha_habilitada (tabela_csv* tab, int linha) {
	if (!tab)	return 0;

	if (linha >= tab->linhas)	return 0;
	if (tab->enable[linha] == EN)	return 1;
	return 0;
}

int coluna_habilitada (tabela_csv* tab, int coluna) {
	if (!tab)	return 0;

	if (coluna >= tab->colunas)	return 0;
	if (tab->cabecalho[coluna].enable == EN)	return 1;
	else	return 0;
}

int elemento_habilitado (tabela_csv *tab, int linha, int coluna) {
	if (!tab)	return 0;

	if (linha >= tab->linhas || coluna >= tab->colunas)	return 0;
	if ((tab->cabecalho[coluna].enable) == EN) { 
		if (tab->enable[linha] == EN)	return 1;
	}	
	return 0;
}

void imprime_elemento (tabela_csv *tab, int linha, int coluna, int largura) {
	char *nan_str = "NaN";
	if (tab->tabela[linha][coluna]) printf("%*s", largura+2, tab->tabela[linha][coluna]);
	else	printf("%*s", largura+2, nan_str);
}

void imprime_pontilhado (int *v_tam, int colunas) {
	for (int i = 0; i < colunas; i++) 
		printf("%*s", v_tam[i]+2, "...");
	printf("\n");
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

char *backup_vet_enable (tabela_csv *t) {
	if (!t)	return NULL;

	char *backup = (char *) malloc (t->linhas * sizeof(char));

	if (backup) 
		for (int i = 0; i < t->linhas; i++)	backup[i] = t->enable[i];
	
	return backup;
}

void restaura_vet_enable (tabela_csv *t, char *backup) {
	if (!t || !backup)	return;

	for (int i = 0; i < t->linhas; i++)	t->enable[i] = backup[i];
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
			if (tok[0] != '\0')	tab[i][j] = strdup(tok);

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

void encontra_tipos (tabela_csv *tab) {
	if (!tab)	return;

	for (int i = 0; i < tab->linhas; i++) {
		for (int j = 0; j < tab->colunas; j++) {
			if (tab->tabela[i][j]) {
				if (atof(tab->tabela[i][j]))
					tab->cabecalho[j].tipo = NUM;
			}
		}
	}
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
	encontra_tipos(tab);
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

void sumario (tabela_csv *tab) {
	char c;

	for (int i = 0; i < tab->colunas; i++) {
		printf("%s ", tab->cabecalho[i].nome);
		if (tab->cabecalho[i].tipo == STR)
			printf("[S]\n");
		else printf("[N]\n");
	}

	printf("\n%d variaveis encontradas\n", tab->colunas);
	printf("\nPressione ENTER para continuar\n");
	scanf("%c", &c);
}

void imprime (tabela_csv *tab) {
	if (!tab)	return;

	int linhas = tab->linhas;
	int colunas = tab->colunas;
	int *v_tamanhos = (int*) malloc (colunas * sizeof(int));
	if (!v_tamanhos)	return;

	for (int i = 0; i < colunas; i++) 
		v_tamanhos[i] = strlen (tab->cabecalho[i].nome);

	for (int i = 0; i < linhas; i++) {
		for (int j = 0; j < colunas; j++) {
			if (tab->tabela[i][j]) {
				int tam = strlen (tab->tabela[i][j]);
				if (tam > v_tamanhos[j])	v_tamanhos[j] = tam;
				}
			}
		}

		int digitos = 0;
		int n_linha = linhas - 1;
		while (n_linha > 0) {
			n_linha = n_linha / 10;
			digitos++;
		} 

		printf("%-*s", digitos+2, "");
		for (int i = 0; i < colunas; i++) {
			if (coluna_habilitada (tab, i))
				printf("%*s",v_tamanhos[i]+2, tab->cabecalho[i].nome);
		}
		printf("\n");

		for (int i = 0; i < 5 && i < linhas; i++) {
			printf ("%-*d", digitos+2, i);
			for (int j = 0; j < colunas; j++) {
				if (elemento_habilitado(tab,i,j))
					imprime_elemento(tab,i,j,v_tamanhos[j]);
			}
			printf("\n");
		}

		printf("%-*s", digitos+2, "...");
		imprime_pontilhado (v_tamanhos, colunas);
		for (int i = linhas - 6; i > 4 && i < linhas; i++) {
			printf ("%-*d", digitos+2, i);
			for (int j = 0; j < colunas; j++) {
				if (elemento_habilitado(tab,i,j))
					imprime_elemento(tab,i,j,v_tamanhos[j]);
			}
			printf("\n");
		}
	
	char c;
	int linhas_habilitadas = 0, colunas_habilitadas = 0;
	for (int i = 0; i < linhas; i++) 
		if (tab->enable[i] == EN)	linhas_habilitadas++;
	for (int j = 0; j < colunas; j++)
		if (tab->cabecalho[j].enable == EN)	colunas_habilitadas ++;
	
	printf("\n[%d rows x %d collumns]\n", linhas_habilitadas,
	colunas_habilitadas);
	printf("\nPressione ENTER para continuar\n");
	scanf("%c", &c);

	free(v_tamanhos);
}

int igual (tabela_csv *t, int linha, int coluna, char* valor) {
	if (!t)	return -1;
	if (t->tabela[linha][coluna]) {
		if (t->cabecalho[coluna].tipo == STR)
			return strcmp (t->tabela[linha][coluna], valor) == 0;
		return atof (t->tabela[linha][coluna]) == atof(valor);
	}	else return 0;
}

int maior (tabela_csv *t, int linha, int coluna, char* valor) {
	if (!t)	return -1;
	if (t->tabela[linha][coluna]) {
		if (t->cabecalho[coluna].tipo == STR)
			return strcmp (t->tabela[linha][coluna], valor) > 0;
		return atof (t->tabela[linha][coluna]) > atof(valor);
	}	else return 0;
}

int maior_igual (tabela_csv *t, int linha, int coluna, char* valor) {
	if (!t)	return -1;
	if (t->tabela[linha][coluna]) {
		if (t->cabecalho[coluna].tipo == STR) 
			return strcmp (t->tabela[linha][coluna], valor) >= 0;
		return atof (t->tabela[linha][coluna]) >= atof(valor);
	}	else return 0;
}

int menor (tabela_csv *t, int linha, int coluna, char* valor) {
	if (!t)	return -1;
	if (t->tabela[linha][coluna]) {
		if (t->cabecalho[coluna].tipo == STR)
			return strcmp (t->tabela[linha][coluna], valor) < 0;
		return atof (t->tabela[linha][coluna]) < atof(valor);
	}	else return 0;
}

int menor_igual (tabela_csv *t, int linha, int coluna, char* valor) {
	if (!t)	return -1;
	if (t->tabela[linha][coluna]) {
		if (t->cabecalho[coluna].tipo == STR)
			return strcmp (t->tabela[linha][coluna], valor) <= 0;
		return atof (t->tabela[linha][coluna]) <= atof(valor);
	}	else return 0;
}

int diferente (tabela_csv *t, int linha, int coluna, char* valor) {
	if (!t)	return -1;
	if (t->tabela[linha][coluna]) {
		if (t->cabecalho[coluna].tipo == STR)
			return strcmp (t->tabela[linha][coluna], valor) != 0;
		return atof (t->tabela[linha][coluna]) != atof(valor);
	}	else return 0;
}

int identifica_filtro (char *filtro) {
	if (strcmp (filtro, "==") == 0)	return 0;
	if (strcmp (filtro, ">") == 0)	return 1;
	if (strcmp (filtro, ">=") == 0)	return 2;
	if (strcmp (filtro, "<") == 0)	return 3;
	if (strcmp (filtro, "<=") == 0)	return 4;
	else	return 5;
}

int identifica_variavel (tabela_csv *t, char *variavel) {
	if (!t)	return -1;
	for (int i = 0; i < t->colunas; i++) {
		if (coluna_habilitada(t, i)) {
			fprintf(stderr, "%s", variavel);
			if (strcmp (variavel, t->cabecalho[i].nome) == 0)
				return i;
		}
	}

	return -2;
}

void filtra (tabela_csv *tab) {
	char variavel[20];
	char filtro[3];
	char valor[20];
	char *backup = backup_vet_enable (tab);

	int(*compara[])(tabela_csv *, int , int, char*) = {
		igual,
		maior,
		maior_igual,
		menor,
		menor_igual,
		diferente
	};

	getchar();
	printf("Entre com a variavel: "); 
	scanf("%s", variavel);
	getchar();
	printf("Escolha um filtro ( == > >= < <= != ): ");
	scanf("%s", filtro);
	getchar();
	printf("Digite um valor: ");
	scanf("%s", valor);
	
	int i_var = identifica_variavel (tab, variavel);
	int i_filtro = identifica_filtro (filtro);
	int linhas = tab->linhas;
	
	for (int i = 0; i < linhas; i++) {
		if (linha_habilitada (tab, i)) {
			if (!compara[i_filtro] (tab, i, i_var, valor))
				tab->enable[i] = N_EN;
		}
	}

	imprime (tab);

	char op;
	getchar();
	printf("Deseja gravar um arquivo com os dados filtrados? [S|N]: ");
	scanf("%c", &op);
	if (op == 'S') {
		char nome_arq[20];
		getchar();
		printf("Entre com o nome do arquivo: ");
		scanf("%s", nome_arq);
		cria_arquivo_csv (tab,nome_arq);
		printf("Arquivo criado com sucesso\n");
	}
	getchar();
	printf("Deseja guardar os dados originais? [S|N]: ");
	scanf("%c", &op);
	if (op == 'S') restaura_vet_enable(tab, backup);
	backup = destroi_str (backup);
	getchar();
	printf("Digite ENTER para continuar");
	scanf("%c", &op);
}
