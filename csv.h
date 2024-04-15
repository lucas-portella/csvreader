#ifndef __CSV__
#define __CSV__
#define N_EN '0'
#define EN '1'
#define STR '0'
#define NUM '1'

typedef struct{
	char *nome;
	char tipo;
	char enable;
} rotulo;

typedef struct{
	rotulo *cabecalho;
	char ***tabela;
	char *enable;
	int linhas;
	int colunas;
} tabela_csv;

int linha_habilitada (tabela_csv *tab, int linha);

int coluna_habilitada (tabela_csv *tab, int coluna);

int elemento_habilitado (tabela_csv *tab, int linha, int coluna);

char *destroi_str (char *);

tabela_csv *inicializa_tabela_csv (const char *nome_arquivo); 

tabela_csv *destroi_tabela_csv (tabela_csv *tab); 

void sumario (tabela_csv *tab);

void imprime (tabela_csv *tab);

void filtra (tabela_csv *tab);

void selecao (tabela_csv *tab);

void ordena(tabela_csv *tab);
#endif
