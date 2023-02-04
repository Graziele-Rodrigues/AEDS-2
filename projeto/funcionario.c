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

void generate_unique_id() {
    for (int i = 0; i < NUM_EMPLOYEES; i++) {
        do {
            is_unique = 1;
            id[i] = rand() % MAX_ID;
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

void busca_funcionarios(FILE *out){
    FILE *busca;
    busca = fopen("busca.dat", "w+b");
    TFunc *f;
    int chave=0;
    for(int i=0; i<50;i++){
        chave = rand() % MAX_ID;
        f = busca_sequencial(out, chave);
        if(f!= NULL){
            salva(f, busca);
            imprime(f);
            printf("\nQuantidade de comparacoes: %d\n", comparisons);
            printf("Tempo gasto: %.2f segundos\n", (double)(end - start) / CLOCKS_PER_SEC);
        }
        else{
            printf("%d, nao encontrado\n", chave);
        }
        free(f);
    }
    le_funcionarios(busca);
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
