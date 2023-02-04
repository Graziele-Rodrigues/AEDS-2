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
        //le_funcionarios(out);
        int entrada=0;
        while(entrada!=5){
            printf("\n######## MENU ##########\n");
            printf("[1] - busca sequencial\n[2] - busca binaria\n[3] - insertionSortMemoria\n[4] - InsertionSortDisco\n[5] - Sair\n");
            scanf("%d", &entrada);
            if(entrada==1){
                busca_funcionarios(out);
            }else if(entrada==5){
                printf("Saindo....\n");
            }else{
                printf("OPCAO INVALIDA\n");
            } 
        }
        //fecha arquivo
        fclose(out);
    }
}

