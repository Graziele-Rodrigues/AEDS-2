#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>

#define TAM 1001
int id[TAM];
int is_unique;

//estrutura com dados a serem preenchidos do funcionario
typedef struct Funcionario {
    int cod;
    char nome[50];
    double salario;
    struct Funcionario *prox;
} TFuncionario;


typedef TFuncionario *Hash;

int hash(int mat, int tam) {
    return mat % tam;
}

// Retorna tamanho do funcionario em bytes
int tamanho_registro() {
    return sizeof(int)  //cod
           + sizeof(char) * 50 //nome
           + sizeof(double); //salario
}

void inicializa(Hash *tab, int m) {
    int i;
    for (i = 0; i < m; i++) {
        tab[i] = NULL;
    }
}

TFuncionario *le(FILE *in) {
    TFuncionario *func = (TFuncionario *) malloc(sizeof(TFuncionario));
    if (0 >= fread(&func->cod, sizeof(int), 1, in)) {
        free(func);
        return NULL;
    }
    fread(func->nome, sizeof(char), sizeof(func->nome), in);
    fread(&func->salario, sizeof(double), 1, in);
    return func;
}

TFuncionario *busca(Hash *tab, int m, int mat) {
    int h = hash(mat, m);
    TFuncionario *p = tab[h];
    while ((p != NULL) && (p->cod != mat)) {
        p = p->prox;
    }
    return p;
}

TFuncionario *aloca(int mat, char nome[], double salario) {
    TFuncionario *novo = (TFuncionario *) malloc(sizeof(TFuncionario));
    novo->cod = mat;
    strcpy(novo->nome, nome);
    novo->salario = salario;
    novo->prox = NULL;
    return novo;
}

void insereFuncionarioArvore(Hash *tab, int m, char * nome_arquivo_entrada) {
    printf("\nInserindo funcionarios\n");
    FILE *arq = fopen(nome_arquivo_entrada, "rb");
    TFuncionario *func;
    while ((func = le(arq)) != NULL) {
        int h = hash(func->cod, m);
        TFuncionario *p = tab[h];
        TFuncionario *ant = NULL;
        while ((p != NULL) && (p->cod != func->cod)) {
            ant = p;
            p = p->prox;
        }
        if (p != NULL) {
            strcpy(p->nome, func->nome);
            p->salario = func->salario;
        } else {
            TFuncionario *novo = aloca(func->cod, func->nome, func->salario);
            if (!ant) {
                tab[h] = novo;
            } else {
                ant->prox = novo;
            }
        }
    }
    printf("\nFuncionarios adicionados com sucesso!\n\n");
}

void libera(Hash *tab, int m) {
    int i;
    for (i = 0; i < m; i++)
        if (tab[i]) {
            TFuncionario *p = tab[i];
            TFuncionario *q;
            while (p) {
                q = p;
                p = p->prox;
                free(q);
            }
        }
}

float exclui(Hash *tab, int m, int mat) {
    int h = hash(mat, m);
    if (tab[h] == NULL) {
        return -1;
    }
    TFuncionario *p = tab[h];
    TFuncionario *ant = NULL;
    float salario = -1;
    while ((p != NULL) && (p->cod != mat)) {
        ant = p;
        p = p->prox;
    }
    if (p == NULL) {
        return salario;
    }
    if (ant == NULL) {
        tab[h] = p->prox;
    } else {
        ant->prox = p->prox;
    }
    salario = p->salario;
    free(p);
    return salario;
}

void imprime(Hash *tab, int m) {
    for (int i = 0; i < m; i++) {
        printf("%d: ", i);
        if (tab[i]) {
            TFuncionario *p = tab[i];
            printf("\n");
            while (p) {
                printf("\t%d\t%s\t%.2f\t%p\n", p->cod,p->nome, p->salario, p->prox);
                p = p->prox;
            }
        } else printf("NULL\n");
    }
}

//Salva funcionario no arquivo out, na posicao atual do cursor
void salva(TFuncionario *func, FILE *out) {
    fwrite(&func->cod, sizeof(int), 1, out);
    //func->nome ao invasao de &func->nome, pois string 
    fwrite(func->nome, sizeof(char), sizeof(func->nome), out);
    fwrite(&func->salario, sizeof(double), 1, out);
}

void generate_unique_id() {
    for (int i = 0; i < TAM; i++) {
        do {
            is_unique = 1;
            id[i] = rand() % TAM;
            for (int j = 0; j < i; j++) {
                if (id[i] == id[j]) {
                    is_unique = 0;
                    break;
                }
            }
        } while (!is_unique);
    }
}

void insere_funcionarios(FILE *out){
    // Gerar nomes e salários aleatórios
    char *nome[] = {"Maria", "Jose", "Carlos", "Ana", "Paulo", "Lucia"};
    int num_names = sizeof(nome) / sizeof(nome[0]);
    double salario;
    generate_unique_id();
    for (int i = 0; i < TAM; i++) {
        int random_index = rand() % num_names;
        salario = (double)rand() / RAND_MAX * (10000 - 1000) + 1000;
        TFuncionario *funcionario = aloca(id[i], nome[random_index], salario);
        salva(funcionario, out);
        free(funcionario);
    }
}

int main(void) {
    FILE *out;
    //abre arquivo
    if ((out = fopen("funcionario.dat", "w+b")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }
    else {
        int m;
        printf("Digite m (tamanho da tabela hash)... ");
        scanf("%d", &m);

        if ((TAM <= 1) || (m <= 1)) return 0;

        Hash *tab[m];
        inicializa(tab, m);
        insere_funcionarios(out);
        insereFuncionarioArvore(tab, m, "funcionario.dat");
        imprime(tab, m);


        int option = -2;
        while (option != -1){
            printf("1-> Remover funcionario\n");             
            printf("2-> Ver informacoes de um funcionario\n");
            scanf("%d", &option);
            switch(option){
            char resp;
            case 1: {
                do {
                    printf("Digite o codigo a ser excluida... ");
                    int mat;
                    float salario;
                    scanf("%d", &mat);
                    salario = exclui(tab, m, mat);
                    if (salario != -1){ 
                        printf("%d\t%.2f foi excluido\n", mat, salario);
                        imprime(tab, m);
                    }else{
                        printf("Elemento nao encontrado!\n");
                    }
                    printf("Quer continuar? ");
                    scanf(" %c", &resp);
                } while ((resp != 'N') && (resp != 'n'));
                break;
            }
            case 2:{
                do {
                    printf("Digite o codigo a ser procurada... ");
                    int mat;
                    float salario;
                    scanf("%d", &mat);
                    TFuncionario *p = busca(tab, m, mat);
                    if (!p) printf("Elemento nao encontrado!\n");
                    else printf("%d\t%s\t%.2f\n", p->cod, p->nome, p->salario);
                    printf("Quer continuar? ");
                    scanf(" %c", &resp);
                } while ((resp != 'N') && (resp != 'n'));
                libera(tab, m);
                break;
            }
            default:{
            break;
            }
        }
        }
    }
    return 0;
}