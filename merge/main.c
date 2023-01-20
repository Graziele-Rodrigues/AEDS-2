#include <stdio.h>
#include <stdlib.h>

void merge(char *nomeArq1, char *nomeArq2, char *nomeArqMerge) {
    FILE *arq1, *arq2, *arq_merge;

    //abre arquivos de leitura
    arq1 = fopen(nomeArq1, "r");
    arq2 = fopen(nomeArq2, "r");
    //abre arquivo escrita
    arq_merge = fopen(nomeArqMerge, "w");

    //declara cursores
    int n1, n2;

    if (arq1 != NULL && arq2 != NULL && arq_merge != NULL) {
        //le primeiro numero dos dois arquivos
        fscanf(arq1, "%d", &n1);
        fscanf(arq2, "%d", &n2);
        while (!feof(arq1) && !feof(arq2)) {
            if (n1 < n2) {
                fprintf(arq_merge, "%d\n", n1);
                fscanf(arq1, "%d", &n1);
            } else {
                if (n2 < n1) {
                    fprintf(arq_merge, "%d\n", n2);
                    fscanf(arq2, "%d", &n2);
                } else {
                    //sao iguais - grava um e avança ambos
                    fprintf(arq_merge, "%d\n", n1);
                    fscanf(arq1, "%d", &n1);
                    fscanf(arq2, "%d", &n2);
                }
            }
        }
        //trata caso de um arquivo terminar antes do outro
        //grava arq1 até terminar
        while (!feof(arq1)) {
            fprintf(arq_merge, "%d\n", n1);
            fscanf(arq1, "%d", &n1);
        }
        //grava arq 2 até terminar
        while (!feof(arq2)) {
            fprintf(arq_merge, "%d\n", n2);
            fscanf(arq2, "%d", &n2);
        }
        fclose(arq1);
        fclose(arq2);
        fclose(arq_merge);
    } else printf("Erro ao abrir arquivos");
}

void grava_arquivo_strings(char *nomeArq, int inicio, int fim, int incremento) {
    FILE *arq; //declara ponteiro para arquivo

    //abre arquivo para gravação
    arq = fopen(nomeArq, "w");
    if (arq != NULL) {// checa se não deu erro na abertura do arquivo
        char *s[12];
        for (int i = inicio; i < fim; i += incremento) {
            sprintf(s, "%d", i); //transforma int em string
            fprintf(arq, "%s\n", &s); //grava no arquivo
        }
        fclose(arq);
    } else {
        printf("Erro ao abrir arquivo\n");
    }
}

int main(int argc, char **argv) {
    grava_arquivo_strings("numeros1.txt", 10, 30, 3);
    grava_arquivo_strings("numeros2.txt", 8, 20, 2);
    merge("numeros1.txt", "numeros2.txt", "merge.txt");
}

