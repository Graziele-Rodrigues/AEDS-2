#include <math.h>
#include <time.h>


typedef struct Funcionario {
    int cod;
    char nome[50];
    double salario;
} TFunc;

#define NUM_EMPLOYEES 5000
#define MAX_ID 5000
int id[NUM_EMPLOYEES];
int is_unique;
int comparisons;
clock_t start, end;

void generate_unique_id();
TFunc *funcionario(int cod, char *nome, double salario);
void salva(TFunc *func, FILE *out);
TFunc *le(FILE *in);
void le_funcionarios(FILE *in);
void imprime(TFunc *func);
void insere_funcionarios(FILE *out);
int tamanho_registro();
int tamanho_arquivo(FILE *arq);
void busca_funcionarios(FILE *out);
TFunc *busca_sequencial(FILE *busca, int chave);
