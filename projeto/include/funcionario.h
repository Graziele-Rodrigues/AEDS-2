#include <math.h>


typedef struct Funcionario {
    int cod;
    char nome[50];
    double salario;
} TFunc;

void imprime(TFunc *func);
TFunc *funcionario(int cod, char *nome, double salario);
void salva(TFunc *func, FILE *out);
TFunc *le(FILE *in);
void le_funcionarios(FILE *in);
TFunc *le_funcionario(FILE *in);
void imprime_arquivo(FILE *arq);
void insere_funcionarios(FILE *out);
void insere_funcionarios_2(FILE *out, int i);
void insere_funcionarios_3(FILE *out);
void le_segundo_funcionario(FILE *in);
void adiciona_funcionario(FILE *in);
void sobrescreve_quarto_funcionario(FILE *in);
int tamanho_registro();
int tamanho_arquivo(FILE *arq);