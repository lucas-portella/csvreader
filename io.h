#ifndef __IO__
#define __IO__
#include "csv.h"

void limpa_buffer ();

int conta_linhas_arquivo (const char *nome_arquivo);

int conta_colunas_arquivo (const char *nome_arquivo);

int conta_caracteres_arquivo (const char *nome_arquivo);

char *le_arquivo (const char *nome_arquivo);

void cria_arquivo_csv (tabela_csv *tab, char *nome_arquivo);

#endif
