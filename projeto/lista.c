#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "include/lista.h"



// Inicializa a lista com os nomes dos arquivos de saída
void inicializa_lista(Lista *lista, char *nomes[], int num_arquivos) {
    int i;
    Lista *atual = lista;

    for (i = 0; i < num_arquivos; i++) {
        atual->nome = nomes[i];
        if (i < num_arquivos - 1) {
            atual->prox = (Lista*) malloc(sizeof(Lista));
            atual = atual->prox;
        } else {
            atual->prox = NULL;
        }
    }
}



//conta num elemento da lista encadeada
int num_elementos_lista(Lista *lista) {
    int count = 0;
    Lista *atual = lista;
    while (atual != NULL) {
        count++;
        atual = atual->prox;
    }
    return count;
}

//percorre os elementos de uma lista encadeada e executa uma função para cada elemento:
void percorre_lista(Lista *lista, void (*funcao)(char *)) {
    Lista *atual = lista;
    while (atual != NULL) {
        funcao(atual->nome);
        atual = atual->prox;
    }
}


