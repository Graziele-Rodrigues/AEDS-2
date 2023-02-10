#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcionario.c"


/* ############################ BUSCA BINARIA ############################ */

//Arquivos devem estar ordenados


/* ############################ MAIN ############################ */

int main(int argc, char** argv) {
    //declara ponteiro para arquivo
    FILE *out;
    //abre arquivo
    if ((out = fopen("funcionario.dat", "w+b")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }
    else {
        insere_funcionarios(out);
        le_funcionarios(out);
        int entrada=0;
        while(entrada!=5){
            printf("\n######## MENU ##########\n");
            printf("[1] - busca sequencial\n[2] - insertion sort disco\n[3] - busca binaria\n[4] - ordenacao externa\n[5] - Sair\n");
            scanf("%d", &entrada);
            if(entrada==1){
                fflush(out);
                busca_funcionarios_sequencial(out);
            }else if(entrada==2){
                fflush(out);
                imprime_arquivo(out);
                insertion_sort_disco(out, tamanho_arquivo(out));
                printf("\n\nArquivo apos ordenacao...\n\n");
                imprime_arquivo(out);
            }else if(entrada==4){
                printf("SUBSTITUIÇÃO");
            }else if(entrada==3){
                printf("LEMBRE DE ESCOLHER UM METODO DE ORDENACAO ANTES\n");
                busca_funcionarios_binario(out);
            }
            else if(entrada==5){
                printf("Saindo....\n");
            }else{
                printf("OPCAO INVALIDA\n");
            } 
        }
        //fecha arquivo
        fclose(out);
    }
}

