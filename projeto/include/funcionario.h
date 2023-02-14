#include <math.h>
#include <time.h>
#include "lista.h"

typedef struct Funcionario {
    int cod;
    char nome[50];
    double salario;
} TFunc;


#define NUM_EMPLOYEES 100
int id[NUM_EMPLOYEES];
int is_unique;
int comparisons;

clock_t start, end;

void generate_unique_id();
TFunc *funcionario(int cod, char *nome, double salario);
void salva(TFunc *func, FILE *out);
TFunc *le(FILE *in);
void le_funcionarios(FILE *in);
void imprime_arquivo(FILE *arq);
void imprime(TFunc *func);
void insere_funcionarios(FILE *out);
int tamanho_registro();
int tamanho_arquivo(FILE *arq);
void busca_funcionarios_sequencial(FILE *out);
TFunc *busca_sequencial(FILE *busca, int chave);
void busca_funcionarios_binario(FILE *out);
TFunc *busca_binaria(int chave, FILE *in, int inicio, int fim);
void insertion_sort_disco(FILE *arq, int tam);
void ordenacao_por_substituicao(char *nome_arquivo_entrada, Lista *nome_arquivos_saida, int M);