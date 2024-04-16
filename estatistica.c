#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "csv.h"
#include "estatistica.h"

char *media (tabela_csv *t, int coluna) {
	if (!t)	return NULL;

	int linhas = t->linhas;
	float media = 0;
	for (int i = 0; i < linhas; i++) {
		if (linha_habilitada(t,i)) {
			char *str = retorna_elemento (t, i, coluna);
			if (str)	media += atof(str);
		}	
	}
	
	int l_hab = linhas_habilitadas(t);
	media = media / l_hab;
	char *media_str;
	asprintf(&media_str, "%f", media);
	return media_str;
}

char *desvio (tabela_csv *t, char *media, int coluna) {
	if (!t || !media)	return NULL;

	int linhas = t->linhas;
	int l_hab = linhas_habilitadas (t); 
	float media_f = atof (media);
	float sum = 0;
	for (int i = 0; i < linhas; i++) {
		if (linha_habilitada (t, i)) {
			char *str = retorna_elemento (t, i, coluna);
			if (str)	{
				float x = atof (str);
				sum += pow(x-media_f, 2);
			}
		}
	}

	sum = sum / l_hab;
	char *desvio;
	asprintf(&desvio, "%f", sum);
	return desvio;
}

/*	assume que os valores estao ordenados */
char *mediana (tabela_csv *t, int coluna) {
	if (!t)	return NULL;
	
	int ultima_pos;
	for (ultima_pos = 0; linha_habilitada(t,ultima_pos); ultima_pos++);
	int p_mediana = ultima_pos/2;	
	char *elem = retorna_elemento (t, p_mediana, coluna);
	return strdup(elem);
}

char **vet_elementos_unicos (tabela_csv *t, int coluna) {
	if (!t)	return NULL;
	int linhas = t->linhas;
	char **v = (char**) calloc (0, linhas);
	if (!v)	return NULL;
	
	int ultima_linha = 0;
	for (int i = 0; i < linhas; i++) {
		char *elem = retorna_elemento (t, i, coluna);
		if (elem) {
			int inserido = 0;
			for (int j = 0; j < ultima_linha && !inserido; j++) {
				if (strcmp (elem, v[j]) == 0)
					inserido = 1;
			}
			if (!inserido) v[ultima_linha++] = elem;
		}
	}
	
	return v;
}

/* recebe um vetor de elementos unicos */
t_moda *moda (tabela_csv *t, char **elem, int coluna) {
	if (!t || !elem)	return NULL;

	/*	o vetor de elementos acaba quando elem[i] == NULL	*/
	int n_elem = 0;
	while (elem[n_elem] != NULL)	n_elem++; 

	/*	alocando vetor com a contagem de aparicoes	*/
	int *aparicoes = (int*) calloc (0, n_elem * sizeof(int));
	if (!aparicoes)	return NULL;
	t_moda *m = (t_moda*) malloc(sizeof(t_moda));	

	/*	contando as aparicoes	*/
	int linhas = t->linhas;
	for (int i = 0; i < linhas; i++) {
		if (linha_habilitada(t,i)) {
				if (coluna_habilitada(t,coluna)) {
					char *str = retorna_elemento(t,i,coluna);
					int p = 0;
					while (strcmp (elem[p], str) != 0)	p++;
					aparicoes[p]++;
				}
			}
		}

	/*	encontrando a moda	*/
	int maior = 0;
	for (int i = 1; i < n_elem; i++) {
		if (aparicoes[i] > aparicoes[maior])
			maior = i;
	}
	
	m->elem = strdup(elem[maior]);
	char *ap;
	asprintf(&ap,"%d", aparicoes[maior]);
	m->aparicoes = ap;
	return m;
}

t_moda *destroi_t_moda (t_moda *m) {
	if (m)	free(m);
	return NULL;
}

char *minimo (tabela_csv *t, int coluna) {
	if (!t)	return NULL;
	char *str = retorna_elemento (t, 0, coluna);
	return strdup (str);
}

char *maximo (tabela_csv *t, int coluna) {
	if (!t)	return NULL;
	int i = 0;
	char *str;
	while ((str = retorna_elemento(t, i, coluna)) != NULL)
		i++;
	
	str = retorna_elemento(t,i,coluna);
	return strdup(str);
}

char *contador (tabela_csv *t, int coluna) {
	if (!t)	return NULL;

	char *str;
	int i = 0;
	while ((str = retorna_elemento(t, i, coluna)) != NULL)
		i++;
	
	asprintf(&str, "%d", i+1);
	return str;
}

info_estat *cria_info_estat () {
	return (info_estat*) calloc (0, sizeof(sizeof(info_estat)));
}

info_estat *destroi_info_estat (info_estat *i) {
	if (i) {
		if (i->contador)	free(i->contador);
		i->contador = NULL;
		if (i->min)	free(i->min);
		i->min = NULL;
		if (i->max)	free(i->max);
		i->max = NULL;
		if (i->media)	free(i->media);
		i->media = NULL;
		if (i->mediana)	free(i->mediana);
		i->mediana = NULL;
		if (i->desvio)	free(i->desvio);
		i->desvio = NULL;
		if (i->moda)	i->moda = destroi_t_moda (i->moda);
		if (i->elementos_unicos)	free(i->elementos_unicos);
		i->elementos_unicos = NULL;
		free(i);
		i = NULL;
	}
	return i;
}

info_estat *gera_estatistica_numerica (tabela_csv *t, int coluna) {
	if (!t)	return NULL;

	info_estat *info = cria_info_estat ();
	info->contador = contador(t,coluna);
	info->min = minimo(t,coluna);
	info->max = maximo(t,coluna);
	info->media = media(t,coluna);
	info->mediana = mediana(t,coluna);
	info->desvio = desvio(t,info->media,coluna);
	info->elementos_unicos = vet_elementos_unicos(t,coluna);
	info->moda = moda (t, info->elementos_unicos, coluna);

	return info;
}

info_estat *gera_estatistica_string (tabela_csv *t, int coluna) {
	if (!t)	return NULL;

	info_estat *info = cria_info_estat ();
	info->contador = contador(t,coluna);
	info->elementos_unicos = vet_elementos_unicos(t,coluna);
	info->moda = moda (t, info->elementos_unicos, coluna);

	return info;
}

info_estat *gera_estatistica (tabela_csv *t, int coluna) {
	if (!t)	return NULL;
	if (t->cabecalho[coluna].tipo == STR)
		return gera_estatistica_string(t,coluna);
	else	return gera_estatistica_numerica(t,coluna);
}

void imprime_estatisticas (info_estat* i) {
	if (!i)	return;

	if (i->contador)	printf("Contador: %s\n", i->contador);
	if (i->media)	printf("Media: %s\n", i->media);
	if (i->desvio)	printf("Desvio: %s\n", i->desvio);
	if (i->mediana)	printf("Mediana: %s\n", i->mediana);
	if (i->moda)	
		printf("Moda: %s %s vezes\n",i->moda->elem, 
		i->moda->aparicoes);
	if (i->min)	printf("Min.: %s\n", i->min);
	if (i->max)	printf("Max.: %s\n", i->max);
	if (i->elementos_unicos && i->elementos_unicos[0]) {
		printf("Valores unicos: [%s",i->elementos_unicos[0]);
		for (int j= 0; i->elementos_unicos[j]; j++)
			printf(", %s", i->elementos_unicos[j]);
		printf("]\n");
	}
}
