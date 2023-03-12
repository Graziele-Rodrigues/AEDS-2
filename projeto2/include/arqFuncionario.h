#ifndef ARQFUNCIONARIO_H
#define ARQFUNCIONARIO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "funcionario.h"


//define em qual arquivo será salva os dados
#define arquivoFuncionario "funcionario.bin"

typedef struct CabecalhoFuncionario{
    int posTopo;
    int posLivre;
} CabFuncionario;

// Verifica se existe um arquivo de dados, caso não exista cria e adiciona o cabeçalho
// Pré-condição: nenhuma
// Pós-condição: caso não exista o arquivo será criado
void iniciaArquivo();

// Escreve os dados de um profissional em determinada posição do arquivo
// Pré-condição: ter um arquivo aberto, uma posição e um profissional 
// Pós-condição: os dados do profissional serão escritos no arquivo
void escreveFuncionarioArq(FILE* arq, Funcionario* funcionario, int pos);

// Lê os dados de um profissional em determinada posição do arquivo
// Pré-condição: ter um arquivo aberto, uma posição
// Pós-condição: retornará um profissional com os dados lidos do arquivo
Funcionario* leFuncionarioArq(FILE* arq, int pos);

// Lê o cabeçalho do arquivo de dados
// Pré-condição: ter um arquivo aberto
// Pós-condição: retornará o cabecalho do arquivo de dados
CabFuncionario* leCabFuncionario(FILE* arq);

// Escreve o cabeçalho do arquivo de dados
// Pré-condição: ter um arquivo aberto e os dados do cabeçalho
// Pós-condição: os dados do cabeçalho serão escritos no arquivo
void escreveCabFuncionario(FILE* arq, CabFuncionario* cabecalho);


#endif 