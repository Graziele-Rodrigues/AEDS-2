#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcionario.c"

/* ############################ BUSCA SEQUENCIAL ############################ */

TFunc *busca_sequencial(FILE *busca, int chave){
    TFunc *func = (TFunc *) malloc(sizeof(TFunc));
    int encontrado=0;
    for(int i=0; i<1000;i++){
        fseek(busca, i *sizeof(TFunc), SEEK_SET);
        fread(func, sizeof(TFunc),1,busca);
        if(func->cod==chave){
            encontrado = 1;
            return func;
        }
    }
    if(encontrado!=1){
        return NULL;
    }
}

void busca_funcionarios(FILE *out){
    FILE *busca;
    busca = fopen("busca.dat", "w+b");
    TFunc *f;
    int chave = 0;
    for(int i=0; i<5;i++){
        printf("\nEntre com um id:\n");
        scanf("%d", &chave);
        f = busca_sequencial(out, chave);
        if(f!= NULL){
            //imprime(f);
            salva(f, busca);
        }
        else{
            printf("Nao encontrado\n");
        }
        free(f);
    }
    le_funcionarios(busca);
    fclose(busca);
}


void buscaSequencial(){
    //declara ponteiro para arquivo
    FILE *out;
    //abre arquivo
    if ((out = fopen("funcionario.dat", "w+b")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }
    else {
        //insere funcionários
        printf("Inserindo funcionarios no arquivo...");
        for(int i=1; i<=1000; i++){
        insere_funcionarios_2(out, i);
        }
        //volta ao início do arquivo e lê os funcionários inseridos
        //le_funcionarios(out);
        //busca 50 funcionarios e salva em busca.dat
        busca_funcionarios(out);
        //fecha arquivo
        fclose(out);
    }
}

/* ############################ BUSCA BINARIA ############################ */

TFunc *busca_binaria(int chave, FILE *in, int inicio, int fim) {
    TFunc *f = NULL;
    int cod = -1;
    while (inicio <= fim && cod != chave) {
        int meio = trunc((inicio + fim) / 2);
        printf("Inicio: %d; Fim: %d; Meio: %d\n", inicio, fim, meio);
        fseek(in, (meio -1) * tamanho_registro(), SEEK_SET);
        f = le_funcionario(in);
        cod = f->cod;
        if (f) {
            if (cod > chave) {
                fim = meio - 1;
            } else {
                inicio = meio + 1;
            }
        }
    }
    if (cod == chave) {
        return f;
    }
    else return NULL;
}

void buscaBinaria(){
    //declara ponteiro para arquivo
    FILE *arq;
    int chave;
    TFunc *f;
    //abre arquivo para leitura e escrita
    if ((arq = fopen("funcionario.dat", "wb+")) == NULL) {
        printf("Erro ao abrir arquivo\n");
    } else {
        //insere funcionÃƒÂ¡rios
        insere_funcionarios_3(arq);
        //forÃƒÂ§a flush para salvar funcionÃƒÂ¡rios
        fflush(arq);
        imprime_arquivo(arq);
        printf("\n\nDigite a chave a ser buscada: ");
        scanf("%d", &chave);
        f = busca_binaria(chave, arq, 0, tamanho_arquivo(arq)-1);
        imprime(f);
        fclose(arq);
    }
}

/* ############################ INSERTION SORT MEMORIA ############################ */

void insertion_sort_memoria(FILE *arq, int tam) {
    TFunc *v[tam - 1];
    //le o arquivo e coloca no vetor
    rewind(arq); //posiciona cursor no inicio do arquivo
    TFunc *f = le(arq);
    int i = 0;
    while (!feof(arq)) {
        v[i] = f;
        f = le(arq);
        i++;
    }
    //faz o insertion sort
    for (int j = 1; j < tam; j++) {
        TFunc *f = v[j];
        i = j - 1;
        while ((i >= 0) && (v[i]->cod > f->cod)) {
            v[i + 1] = v[i];
            i = i - 1;
        }
        v[i+1] = f;
    }
    //grava vetor no arquivo, por cima do conteÃºdo anterior
    rewind(arq);
    for (int i = 0; i < tam; i++) {
        salva(v[i], arq);
    }
    //descarrega o buffer para ter certeza que dados foram gravados
    fflush(arq);
}

void insertionSortMemoria() {
    //declara ponteiro para arquivo
    FILE *arq;
    //abre arquivo para leitura e escrita
    if ((arq = fopen("funcionario.dat", "wb+")) == NULL) {
        printf("Erro ao abrir arquivo\n");
    } else {
        //insere funcionÃ¡rios
        insere_funcionarios(arq);
        //forÃ§a flush para salvar funcionÃ¡rios
        fflush(arq);
        imprime_arquivo(arq);
        insertion_sort_memoria(arq, tamanho_arquivo(arq));
        printf("\n\nArquivo apos ordenacao...\n\n");
        imprime_arquivo(arq);
        fclose(arq);
    }
}

/* ############################ INSERTION SORT DISCO ############################ */

void insertion_sort_disco(FILE *arq, int tam) {
    int i;
    //faz o insertion sort
    for (int j = 2; j <= tam; j++) {
        //posiciona o arquivo no registro j
        fseek(arq, (j-1) * tamanho_registro(), SEEK_SET);
        TFunc *fj = le(arq);
        printf("\n********* Funcionario atual: %d\n", fj->cod);
        i = j - 1;
        //posiciona o cursor no registro i
        fseek(arq, (i-1) * tamanho_registro(), SEEK_SET);
        TFunc *fi = le(arq);
        printf("fi = %d\n", fi->cod);
        while ((i > 0) && (fi->cod > fj->cod)) {
            //posiciona o cursor no registro i+1
            fseek(arq, i * tamanho_registro(), SEEK_SET);
            printf("Salvando funcionario %d na posicao %d\n", fi->cod, i+1);
            salva(fi, arq);
            i = i - 1;
            //lÃª registro i
            fseek(arq, (i-1) * tamanho_registro(), SEEK_SET);
            fi = le(arq);
            printf("fi = %d; i = %d\n", fi->cod, i);
        }
        //posiciona cursor no registro i + 1
        fseek(arq, (i) * tamanho_registro(), SEEK_SET);
        printf("*** Salvando funcionario %d na posicao %d\n", fj->cod, i+1);
        //salva registro j na posiÃ§Ã£o i
        salva(fj, arq);
    }
    //descarrega o buffer para ter certeza que dados foram gravados
    fflush(arq);
}

void insertionSortDisco() {
    //declara ponteiro para arquivo
    FILE *arq;
    //abre arquivo para leitura e escrita
    if ((arq = fopen("funcionario.dat", "wb+")) == NULL) {
        printf("Erro ao abrir arquivo\n");
    } else {
        //insere funcionÃ¡rios
        insere_funcionarios(arq);
        //forÃ§a flush para salvar funcionÃ¡rios
        fflush(arq);
        imprime_arquivo(arq);
        insertion_sort_disco(arq, tamanho_arquivo(arq));
        printf("\n\nArquivo apos ordenacao...\n\n");
        imprime_arquivo(arq);
        fclose(arq);
    }
}

/* ############################ MAIN ############################ */

int main() {
    int entrada=0;
    while(entrada!=5){
        printf("\n######## MENU ##########\n");
        printf("[1] - busca sequencial\n[2] - busca binaria\n[3] - insertionSortMemoria\n[4] - InsertionSortDisco\n[5] - Sair\n");
        scanf("%d", &entrada);
        if(entrada==1){
            buscaSequencial();
        }else if(entrada==2){
            buscaBinaria();
        }else if(entrada==3){
            insertionSortMemoria();
        }else if(entrada==4){
            insertionSortDisco();
        }else if(entrada==5){
            printf("Saindo....\n");
        }else{
            printf("OPCAO INVALIDA\n");
        } 
        }
    }

