#ifndef __ESTATISTICA__
#define __ESTATISTICA__

typedef struct {
	char* elem;
	char* aparicoes;
} t_moda;

typedef struct {
	char *contador;
	char *min;
	char *max;
	char *media;
	char *mediana;
	char *desvio;
	t_moda *moda;
	char **elementos_unicos;
} info_estat ;

info_estat *gera_estatistica (tabela_csv *t, int coluna);
info_estat *destroi_info_estat (info_estat *i); 
void imprime_estatisticas (info_estat* i); 
#endif
