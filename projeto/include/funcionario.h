#ifndef FUNCIONARIO_H_INCLUDED
#define FUNCIONARIO_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>

//Constante para o numero de funcionarios
#define NFUNC 100
int is_unique;
int id[NFUNC];

typedef struct Funcionario {
    int cod;
    char nome[50];
    double salario;
} TFunc;

//id unico
void generate_unique_id();

// Imprime funcionario
void imprime(TFunc *func);

// Cria funcionario. Lembrar de usar free(funcionario)
TFunc *funcionario(int cod, char *nome, double salario);

// Salva funcionario no arquivo out, na posicao atual do cursor
void salva(TFunc *func, FILE *out);

// Le um funcionario do arquivo in na posicao atual do cursor
// Retorna um ponteiro para funcionario lido do arquivo
TFunc *le(FILE *in);

// Retorna tamanho do funcionario em bytes
int tamanho_registro();

// Retorna o tamanho do arquivo em bytes
int tamanho_arquivo(FILE *arq) ;


#endif // FUNCIONARIO_H_INCLUDED
