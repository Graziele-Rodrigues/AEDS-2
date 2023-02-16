#ifndef NOMES_H
#define NOMES_H


typedef struct lista {
    char *nome;
    struct lista *prox;
} Lista;


Lista *lista_nomes_arquivos_saida;

void inicializa_lista(Lista *lista, char *nomes[], int num_arquivos);
int num_elementos_lista(Lista *lista);
void percorre_lista(Lista *lista, void (*funcao)(char *));

#endif