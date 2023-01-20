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
    printf("\n\nLendo funcionários do arquivo...\n\n");
    rewind(in);
    TFunc *f;
    while ((f = le(in)) != NULL) {
        imprime(f);
        free(f);
    }
}


TFunc *le_funcionario(FILE *in) {
    TFunc *func = (TFunc *) malloc(sizeof(TFunc));
    if (0 >= fread(&func->cod, sizeof(int), 1, in)) {
        free(func);
        return NULL;
    }
    fread(func->nome, sizeof(char), sizeof(func->nome), in);
    fread(&func->salario, sizeof(double), 1, in);
    return func;
}



//Imprime arquivo
void imprime_arquivo(FILE *arq) {
    //le o arquivo e coloca no vetor
    rewind(arq); //posiciona cursor no inicio do arquivo
    TFunc *f = le(arq);
    while (!feof(arq)){
        imprime(f);
        f = le(arq);
    }
}

void insere_funcionarios(FILE *out) {
    TFunc *f;
    f = funcionario(143, "Ana Bueno", 1500);
    salva(f, out);
    f = funcionario(254, "Arnaldo Souza", 4300);
    salva(f, out);
    f = funcionario(123, "Carlos Albuquerque", 1500);
    salva(f, out);
    f = funcionario(102, "Joao Silva", 1000);
    salva(f, out);
    f = funcionario(410, "Guilherme Santos", 2000);
    salva(f, out);
    f = funcionario(305, "Marisa Clara", 5000);
    salva(f, out);
    f = funcionario(502, "Tatiana Andrade", 2500);
    salva(f, out);
    f = funcionario(200, "Caio GusmÃ£o", 4000);
    salva(f, out);
    f = funcionario(239, "Bianca Amarilo", 3000);
    salva(f, out);
    f = funcionario(403, "Bruno SimÃ£o", 4500);
    salva(f, out);
    free(f);
}

void insere_funcionarios_2(FILE *out, int i) {
    TFunc *f = funcionario(i, "XXXX", 0000);
    salva(f, out);
    free(f);
}

//para usar busca binaria o arquivo já tem que estar ordenado. Aqui estamos colocando em ordem, mas o correto é utilizar o insertSort
void insere_funcionarios_3(FILE *out) {
    TFunc *f;
    f = funcionario(100, "Ana Bueno", 1500);
    salva(f, out);
    f = funcionario(102, "Arnaldo Souza", 4300);
    salva(f, out);
    f = funcionario(130, "Carlos Albuquerque", 1500);
    salva(f, out);
    f = funcionario(133, "Joao Silva", 1000);
    salva(f, out);
    f = funcionario(135, "Guilherme Santos", 2000);
    salva(f, out);
    f = funcionario(142, "Marisa Clara", 5000);
    salva(f, out);
    f = funcionario(502, "Tatiana Andrade", 2500);
    salva(f, out);
    f = funcionario(509, "Caio GusmÃƒÂ£o", 4000);
    salva(f, out);
    f = funcionario(567, "Bianca Amarilo", 3000);
    salva(f, out);
    f = funcionario(876, "Bruno SimÃƒÂ£o", 4500);
    salva(f, out);
    f = funcionario(887, "Catarina Aguiar", 5000);
    salva(f, out);
    f = funcionario(899, "Carlos Silva", 1200);
    salva(f, out);
    f = funcionario(901, "Bruno Lucca", 1800);
    salva(f, out);
    f = funcionario(903, "Ana Maria dos Santos", 2300);
    salva(f, out);
    f = funcionario(923, "Armando Nogueira", 3000);
    salva(f, out);
    f = funcionario(976, "Carolina Dias", 2500);
    salva(f, out);
    f = funcionario(989, "Carina Matos", 4100);
    salva(f, out);
    free(f);
}

void le_segundo_funcionario(FILE *in) {
    printf("\n\nLendo segundo funcionário do arquivo...\n\n");
    //tamanho() indica quantos bytes vamos pular, o que aqui é igual ao tamanho de um registro
    //(vamos pular o primeiro e nos posicionar no início do segundo)
    //** ATENÇÃO: não usar sizeof(Funcionario), pois ele pode retornar valor maior que o tamanho ocupado em disco,
    //            devido a alinhamento automático (ver https://en.wikipedia.org/wiki/Data_structure_alignment))
    //SEEK_SET indica o início do arquivo
    //ao final, o cursor estará posicionado em 0 + tamanho() +1
    fseek(in, tamanho_registro(), SEEK_SET);
    TFunc *f = le(in);
    if (f != NULL) {
        imprime(f);
        free(f);
    }
}

void adiciona_funcionario(FILE *in) {
    printf("\n\nAdicionando funcionário no final do arquivo...\n\n");
    //pula 5 registros para posicionar no início do final do arquivo
    fseek(in, tamanho_registro() * 5, SEEK_SET);
    TFunc *f = funcionario(200, "Maria Carolina", 1500);
    salva(f, in);
    free(f);

    //lê funcionário que acabou de ser gravado
    //posiciona novamente o cursor no início desse registro
    fseek(in, tamanho_registro() * 5, SEEK_SET);
    TFunc *f6 = le(in);
    if (f6 != NULL) {
        imprime(f6);
        free(f6);
    }
}

void sobrescreve_quarto_funcionario(FILE *in) {
    printf("\n\nSobrescrevendo quarto funcionário do arquivo...\n\n");
    //pula primeiros 3 registros para posicionar no início do quarto registro
    fseek(in, tamanho_registro() * 3, SEEK_SET);
    TFunc *f4 = funcionario(143, "Ana Bueno", 1500);
    salva(f4, in);
    free(f4);

    //lê funcionário que acabou de ser gravado
    //posiciona novamente o cursor no início desse registro
    fseek(in, tamanho_registro() * 3, SEEK_SET);
    TFunc *f = le(in);
    if (f != NULL) {
        imprime(f);
        free(f);
    }
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
