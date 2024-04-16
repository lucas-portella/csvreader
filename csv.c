#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv.h"
#include "io.h"
#include "estatistica.h"

char *destroi_str (char *str) {
	if (str) free(str);
	return NULL;
}

char *retorna_elemento (tabela_csv *tab, int linha, int coluna) {
	if (!tab)	return NULL;
	if (linha >= tab->linhas)	return NULL;
	if (coluna >= tab->colunas)	return NULL;
	return tab->tabela[linha][coluna];
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

void salvar_dados (tabela_csv *tab) {
		char nome_arq[20];
		getchar();
		printf("Entre com o nome do arquivo: ");
		scanf("%s", nome_arq);
		cria_arquivo_csv (tab,nome_arq);
		printf("Arquivo criado com sucesso\n");
}

/*	imprime uma linha da tabela, considerando-a valida
		v_tam: vetor com o tamanho das strings para formatacao	*/
void imprime_linha (tabela_csv *tab, int linha, int *v_tam) {
	if (!tab || !v_tam)	return;

	int colunas = tab->colunas;
	printf("%*d\t", v_tam[colunas], linha);
	for (int i = 0; i < colunas; i++) {
		if (coluna_habilitada(tab,i)) {
			char* str = retorna_elemento (tab, linha, i);
			if (str) {
				printf("%*s\t", v_tam[i], str);
			}	else printf("%*s\t", v_tam[i], "NaN");
		}
	}
	printf("\n");
}

/*	v_tam: vetor com o tamanho das strings para formatacao */
void imprime_pontilhado (tabela_csv *tab, int *v_tam) {
	char *pont = "...";
	int colunas = tab->colunas;
	printf("%-*s\t", v_tam[colunas], pont);
	for (int i = 0; i < colunas; i++)
		if (coluna_habilitada (tab, i))	printf("%*s\t", v_tam[i], pont);
	printf("\n");
}

void imprime_cabecalho (tabela_csv* tab, int *v_tam) {
	int colunas = tab->colunas;
	printf("%*s\t", v_tam[colunas], "");
	for (int i = 0; i < colunas; i++) {
		char *str = tab->cabecalho[i].nome;
		if (coluna_habilitada(tab,i))	printf("%*s\t", v_tam[i], str);
	}
	printf("\n");
}

int linhas_habilitadas (tabela_csv* tab) {
	if (!tab)	return -1;
	int linhas = tab->linhas;
	int l_hab = 0;
	for (int i = 0; i < linhas; i++)
		if (linha_habilitada(tab,i))	l_hab ++;
	return l_hab;
}

int colunas_habilitadas (tabela_csv* tab) {
	if (!tab)	return -1;
	int colunas = tab->colunas;
	int c_hab = 0;
	for (int i = 0; i < colunas; i++) 
		if (coluna_habilitada(tab,i))	c_hab++;
	return c_hab;
}

void imprime (tabela_csv *tab) {
	if (!tab)	return;

	int linhas = tab->linhas;
	int colunas = tab->colunas;
	/*	vetor de tamanhos para formatacao	*/
	int *v_tamanhos = (int*) malloc ((colunas + 1) * sizeof(int));
	if (!v_tamanhos)	return;

	/*	identificando tamanhos para formatacao */
	
	int digitos = 0;
	int n_linha = linhas - 1;
	while (n_linha > 0) {
		n_linha = n_linha / 10;
		digitos++;
	} 
	v_tamanhos[colunas] = digitos;

	for (int i = 0; i < colunas; i++) 
		v_tamanhos[i] = strlen (tab->cabecalho[i].nome);
	
	for (int i =0; i < linhas; i++) {
		for (int j = 0; j < colunas; j++) {
			char *str = retorna_elemento(tab,i,j);
			if (str) {
				int tam = strlen (str);
				if (tam > v_tamanhos[j])	v_tamanhos[j] = tam;
			}
		}
	}
	
	/*	impressao dos elementos */
	imprime_cabecalho (tab, v_tamanhos);
	int impressos = 0, i;
	for (i = 0; i < linhas && impressos < 5; i++) {
		if (linha_habilitada (tab, i)) {
			imprime_linha (tab, i, v_tamanhos);
			impressos++;
		}
	}
	imprime_pontilhado (tab, v_tamanhos);
	/*	encontrando as ultimas 5 linhas validas	*/	
	int j, validas = 0;
	for (j = linhas - 1; j > i && validas < 4; j--) {
		if (linha_habilitada (tab, j))	validas++;
	}
	for (; j < linhas && validas >= 0; j++) {
		if (linha_habilitada(tab,j)) {
			imprime_linha (tab, j, v_tamanhos);
			validas--;
		}
	}

	int l_hab = linhas_habilitadas (tab);
	int c_hab = colunas_habilitadas (tab);
	printf("\n[%d rows x %d collumns]\n", l_hab, c_hab);
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
		if (strcmp (variavel, t->cabecalho[i].nome) == 0)
			return i;
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

	limpa_buffer();
	printf("Entre com a variavel: "); 
	scanf("%s", variavel);
	limpa_buffer();
	printf("Escolha um filtro ( == > >= < <= != ): ");
	scanf("%s", filtro);
	limpa_buffer();
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
	limpa_buffer();
	printf("Deseja gravar um arquivo com os dados filtrados? [S|N]: ");
	scanf("%c", &op);
	if (op == 'S') salvar_dados(tab);

	limpa_buffer();
	printf("Deseja guardar os dados originais? [S|N]: ");
	scanf("%c", &op);
	if (op == 'S') restaura_vet_enable(tab, backup);
	backup = destroi_str (backup);
}

char *backup_en_colunas (tabela_csv *tab) {
	int colunas = tab->colunas;
	char *backup = (char*) malloc (colunas * sizeof (char));
	if (!backup)	return NULL;

	for (int i = 0; i < colunas; i++)	backup[i] = tab->cabecalho[i].enable;
	return backup;	
}

void restaura_en_colunas (tabela_csv *tab, char *backup) {
	if (!tab || !backup)	return;

	int colunas = tab->colunas;	
	for (int i = 0; i < colunas; i++)	tab->cabecalho[i].enable = backup[i];
}

void habilita_coluna (tabela_csv *tab, int indice) {
	if (!tab)	return;
	if (indice < 0 || indice >= tab->colunas)	return;
	tab->cabecalho[indice].enable = EN;
}

void desabilita_colunas (tabela_csv *tab) {
	if (!tab)	return;

	for (int i = 0; i < tab->colunas; i++)	
		tab->cabecalho[i].enable = N_EN;
}

void selecao (tabela_csv *tab) {
	if (!tab)	return;

	char variaveis[100];
	char *backup = backup_en_colunas (tab);
	desabilita_colunas (tab);
	limpa_buffer();
	printf("Entre com as variaveis que deseja selecionar (separadas por espaco) : ");
	scanf("%[^\n]", variaveis);

	char *tok;
	tok = strtok (variaveis, " ");
	while (tok) {
		int indice = identifica_variavel(tab, tok);
		habilita_coluna (tab, indice);	
		tok = strtok (NULL, " ");
	}

	imprime(tab);
	char op;
	limpa_buffer();
	printf("Deseja gravar um arquivo com as variaveis selecionadas? [S|N] : ");
	scanf("%c", &op);
	if (op == 'S')	salvar_dados (tab);

	limpa_buffer();
	printf("Deseja guardar os dados originais? [S|N]: ");
	scanf("%c", &op);
	if (op == 'S') restaura_en_colunas (tab, backup);
	backup = destroi_str (backup);
}

char ***posicao_original_dados (tabela_csv *tab) {
	if (!tab)	return NULL;

	int linhas = tab->linhas;
	char ***backup = (char ***) malloc (linhas * sizeof(char**));
	if (!backup)	return NULL;

	for (int i = 0; i < linhas; i++)
		backup[i] = tab->tabela[i];

	return backup;
}

void restaura_posicao_original_dados (tabela_csv *tab, char ***backup) {
	if (!tab || !backup)	return;

	for (int i = 0; i < tab->linhas; i++)
		tab->tabela[i] = backup[i];
}

void troca_linhas (tabela_csv *tab, int l1, int l2) {
	if (!tab)	return;

	char **tmp = tab->tabela[l1];
	tab->tabela[l1] = tab->tabela[l2];
	tab->tabela[l2] = tmp;
}

// retorna a ultima posicao com elemento valido
int move_invalidos (tabela_csv *tab, int coluna) {
	if (!tab) return -1;
	
	int ultima_pos = tab->linhas-1;
	int i = 0;
	while (i < ultima_pos) {
		if (!tab->tabela[i][coluna])
			troca_linhas(tab,i,ultima_pos--);
		i++;
	}

	if (tab->tabela[i][coluna])	return i;
	return i-1;
}

int particiona (tabela_csv *t, int(*compara)(tabela_csv *, int , int, char*),
int coluna, int ini, int fim) {
	int i = ini - 1;
	int j ;
	int p = fim;
	char *pivo = t->tabela[fim][coluna];

	for (j = ini; j < fim; j++) {
		if (compara(t,j,coluna,pivo)) {
			i++;
			troca_linhas (t, i, j);
		}
	}
	troca_linhas (t,i+1,p);
	return i+1;
}

void quick_sort (tabela_csv *t, int(*compara)(tabela_csv *, int , int, char*),
int coluna, int ini, int fim) {
	if (ini < fim) {
		int pos = ini + rand()%(fim - ini +1);
		troca_linhas(t,pos,fim);
		int p = particiona (t, compara, coluna, ini, fim);
		quick_sort (t, compara, coluna, ini, p-1);
		quick_sort (t, compara, coluna, p+1, fim);
	}
}

void ordena (tabela_csv *tab) {

	int(*compara[])(tabela_csv *, int, int, char*) = {
		menor,
		maior
	};

	char variavel[20], op;
	int id_func;
	limpa_buffer();
	printf("Entre com a variavel: ");
	scanf("%s", variavel);
	limpa_buffer();
	printf("Selecione uma opcao [A]scendente ou [D]ecrescente: ");
	scanf("%c", &op);
	if (op == 'A')	id_func = 0;
	else	id_func = 1;
	int coluna = identifica_variavel(tab, variavel);

	char ***backup = posicao_original_dados(tab);
	int fim = move_invalidos(tab, coluna);
	quick_sort(tab,compara[id_func],coluna, 0, fim);
	imprime(tab);
	limpa_buffer();
	printf("Deseja gravar um arquivo com os dados ordenados? [S|N] ");
	scanf("%c", &op);
	if (op == 'S')	salvar_dados(tab);
	limpa_buffer();
	printf("Deseja descartar os dados originais? [S|N] ");
	scanf("%c", &op);
	if (op == 'N')	restaura_posicao_original_dados(tab, backup);
	free(backup);
}

void descricao (tabela_csv *tab) {
	if (!tab)	return;

	char variavel[20];
	limpa_buffer();
	printf("Entre com a variavel :");
	scanf("%s", variavel);
	int coluna = identifica_variavel(tab, variavel);

	char ***backup = posicao_original_dados(tab);
	int fim = move_invalidos(tab, coluna);
	quick_sort(tab,&menor,coluna, 0, fim);
	info_estat *i = gera_estatistica (tab, coluna);
	imprime_estatisticas(i);
	restaura_posicao_original_dados(tab, backup);
	free(backup);
}
