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
