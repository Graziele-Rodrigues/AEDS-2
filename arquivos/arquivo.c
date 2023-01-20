//#include <stdio.h>
//#include <stdlib.h>
#include "arquivo.h"

void le_arquivo_caracteres(char* nomeArq){
    FILE *arq; //declara ponteiro para arquivo
    arq = fopen(nomeArq, "r"); //abre arquivo
    if (arq != NULL) {// checa se nao deu erro na abertura do arquivo
        char c;
        while ((c = fgetc(arq)) != EOF) {//le caractere do arquivo e testa se chegou ao final do arquivo
            printf("%c", c); //imprime caractere lido no monitor
        }
        fclose(arq); //fecha arquivo
    } else {
        printf("Erro ao abrir arquivo\n");
    }
}

void le_arquivo_strings(char* nomeArq) {
    FILE *arq; //declara ponteiro para arquivo
    arq = fopen(nomeArq, "r"); //abre arquivo
    if (arq != NULL) {// checa se n�o deu erro na abertura do arquivo
        char s[12];
        fscanf(arq, "%s", s);
        while (!feof(arq)) {//testa se chegou ao final do arquivo
            printf("%s\n", s);
            fscanf(arq, "%s", s);
        }
        fclose(arq); //fecha arquivo
    } else {
        printf("Erro ao abrir arquivo\n");
    }
}

void le_arquivo_strings_numeros(char* nomeArq) {

    FILE *arq; //declara ponteiro para arquivo

    //abre arquivo
    arq = fopen(nomeArq, "r");

    if (arq != NULL) {// checa se não deu erro na abertura do arquivo
        int n;
        fscanf(arq, "%d", &n);
        while (!feof(arq)) {//testa se chegou ao final do arquivo
            printf("%d\n", n);
            fscanf(arq, "%d", &n);
        }
        fclose(arq); //fecha arquivo
    } else {
        printf("Erro ao abrir arquivo\n");
    }
}

void grava_arquivo_strings(char *nomeArq, int inicio, int fim, int incremento) {

    FILE *arq; //declara ponteiro para arquivo

    //abre arquivo para gravação
    arq = fopen(nomeArq, "w");
    if (arq != NULL) {// checa se não deu erro na abertura do arquivo
        for (int i = inicio; i < fim; i += incremento) {
            fprintf(arq, "%d\n", i); //grava no arquivo
        }
        fclose(arq);
    } else {
        printf("Erro ao abrir arquivo\n");
    }
}
