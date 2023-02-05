#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "include/funcionario.h"

// Imprime funcionario
void imprime(TFunc *func) {
    printf("**********************************************");
    printf("\nFuncionario de codigo: ");
    printf("%d", func->cod);
    printf("\nNome: ");
    printf("%s", func->nome);
    printf("\nSalario: ");
    printf("%4.2f", func->salario);
    printf("\n**********************************************");
}

//Cria funcionario
TFunc *funcionario(int cod, char *nome, double salario) {
    TFunc *func = (TFunc *) malloc(sizeof(TFunc));
    //inicializa espaÃƒÂ§o de memÃƒÂ³ria com ZEROS
    if (func) memset(func, 0, sizeof(TFunc));
    //copia valores para os campos de func
    func->cod = cod;
    strcpy(func->nome, nome);
    func->salario = salario;
    return func;
}

//Salva funcionario no arquivo out, na posicao atual do cursor
void salva(TFunc *func, FILE *out) {
    fwrite(&func->cod, sizeof(int), 1, out);
    //func->nome ao invasao de &func->nome, pois string 
    fwrite(func->nome, sizeof(char), sizeof(func->nome), out);
    fwrite(&func->salario, sizeof(double), 1, out);
}


/*
 * Le um funcionario do arquivo in na posicao atual do cursor
 * Retorna um ponteiro para funcionario lido do arquivo
 */
TFunc *le(FILE *in) {
    TFunc *func = (TFunc *) malloc(sizeof(TFunc));
    if (0 >= fread(&func->cod, sizeof(int), 1, in)) {
        free(func);
        return NULL;
    }
    fread(func->nome, sizeof(char), sizeof(func->nome), in);
    fread(&func->salario, sizeof(double), 1, in);
    return func;
}



void le_funcionarios(FILE *in) {
    printf("\n\nLendo funcionarios do arquivo...\n\n");
    rewind(in);
    TFunc *f;
    while ((f = le(in)) != NULL) {
        imprime(f);
        free(f);
    }
}

void imprime_arquivo(FILE *arq) {
    //le o arquivo e coloca no vetor
    rewind(arq); //posiciona cursor no inicio do arquivo
    TFunc *f = le(arq);
    int i = 0;
    while (!feof(arq)) {
        imprime(f);
        f = le(arq);
    }
}


void generate_unique_id() {
    for (int i = 0; i < NUM_EMPLOYEES; i++) {
        do {
            is_unique = 1;
            id[i] = rand() % NUM_EMPLOYEES;
            for (int j = 0; j < i; j++) {
                if (id[i] == id[j]) {
                    is_unique = 0;
                    break;
                }
            }
        } while (!is_unique);
    }
}

void insere_funcionarios(FILE *out) {
    printf("Inserindo funcionarios no arquivo...");
    // Gerar nomes e salários aleatórios
    char *nome[] = {"Maria", "João", "Carlos", "Ana", "Paulo", "Lucia"};
    int num_names = sizeof(nome) / sizeof(nome[0]);
    double salario;
    generate_unique_id();
    for (int i = 0; i < NUM_EMPLOYEES; i++) {
        int random_index = rand() % num_names;
        salario = (double)rand() / RAND_MAX * (10000 - 1000) + 1000;
        TFunc *f = funcionario(id[i], nome[random_index], salario);
        salva(f, out);
        free(f);
    }
}

/* ############################ BUSCA SEQUENCIAL ############################ */
TFunc *busca_sequencial(FILE *busca, int chave){
    comparisons=0;
    TFunc *func = (TFunc *) malloc(sizeof(TFunc));
    int encontrado=0;
    start = clock();
    for(int i=0; i<NUM_EMPLOYEES;i++){
        comparisons++;
        fseek(busca, i *sizeof(TFunc), SEEK_SET);
        fread(func, sizeof(TFunc),1,busca);
        if(func->cod==chave){
            encontrado = 1;
            end = clock();
            return func;
        }
    }
    if(encontrado!=1){
        return NULL;
    }
}

void busca_funcionarios_sequencial(FILE *out){
    FILE *busca;
    busca = fopen("busca.dat", "w+b");
    TFunc *f;
    int chave=0;
    for(int i=0; i<50;i++){
        chave = rand() % NUM_EMPLOYEES;
        f = busca_sequencial(out, chave);
        if(f!= NULL){
            imprime(f);
            printf("\nQuantidade de comparacoes: %d\n", comparisons);
            printf("Tempo gasto: %.2f segundos\n", (double)(end - start) / CLOCKS_PER_SEC);
            salva(f, busca);        }
        else{
            printf("%d, nao encontrado\n", chave);
        }
        free(f);
    }
    imprime_arquivo(busca);
    fclose(busca);
}

/*#################################### INSERTION SORT DISCO ####################################*/
void insertion_sort_disco(FILE *arq, int tam) {
    int i;
    //faz o insertion sort
    for (int j = 2; j <= tam; j++) {
        //posiciona o arquivo no registro j
        fseek(arq, (j-1) * tamanho_registro(), SEEK_SET);
        TFunc *fj = le(arq);
        i = j - 1;
        //posiciona o cursor no registro i
        fseek(arq, (i-1) * tamanho_registro(), SEEK_SET);
        TFunc *fi = le(arq);
        while ((i > 0) && (fi->cod > fj->cod)) {
            //posiciona o cursor no registro i+1
            fseek(arq, i * tamanho_registro(), SEEK_SET);
            salva(fi, arq);
            i = i - 1;
            //le registro i
            fseek(arq, (i-1) * tamanho_registro(), SEEK_SET);
            fi = le(arq);
        }
        //posiciona cursor no registro i + 1
        fseek(arq, (i) * tamanho_registro(), SEEK_SET);
        //salva registro j na posicao i
        salva(fj, arq);
    }
    //descarrega o buffer para ter certeza que dados foram gravados
    fflush(arq);
}

/*#################################### BUSCA BINARIA ####################################*/

TFunc *busca_binaria(int chave, FILE *in, int inicio, int fim) {
    comparisons=0;
    TFunc *f = NULL;
    int cod = -1;
    start = clock();
    while (inicio <= fim && cod != chave) {
        comparisons++;
        int meio = trunc((inicio + fim) / 2);
        fseek(in, (meio -1) * tamanho_registro(), SEEK_SET);
        f = le(in);
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
        end = clock();
        return f;
    }
    else return NULL;
}

void busca_funcionarios_binario(FILE *out){
    FILE *busca;
    busca = fopen("busca.dat", "w+b");
    TFunc *f;
    int chave=0;
    for(int i=0; i<50;i++){
        chave = rand() % NUM_EMPLOYEES;
        f = busca_binaria(chave, out, 0, tamanho_arquivo(out)-1);
        if(f!= NULL){
            imprime(f);
            printf("\nQuantidade de comparacoes: %d\n", comparisons);
            printf("Tempo gasto: %.2f segundos\n", (double)(end - start) / CLOCKS_PER_SEC);
            salva(f, busca);        }
        else{
            printf("%d, nao encontrado\n", chave);
        }
        free(f);
    }
    imprime_arquivo(busca);
    fclose(busca);
}

// Retorna tamanho do funcionario em bytes
int tamanho_registro() {
    return sizeof(int)  //cod
           + sizeof(char) * 50 //nome
           + sizeof(double); //salario
}


int tamanho_arquivo(FILE *arq) {
    fseek(arq, 0, SEEK_END);
    int tam = trunc(ftell(arq) / tamanho_registro());
    return tam;
}
