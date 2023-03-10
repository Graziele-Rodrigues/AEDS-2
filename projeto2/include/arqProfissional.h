#ifndef ARQPROFISSIONAL_H
#define ARQPROFISSIONAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arvore.h"
#include "arq.h"
#include "menu.h"
#include "profissional.h"


//define em qual arquivo será salva os dados
#define arquivoProfissional "profissional.bin"

typedef struct CabecalhoProfissional{
    int posTopo;
    int posLivre;
} CabProfissional;

// Verifica se existe um arquivo de dados, caso não exista cria e adiciona o cabeçalho
// Pré-condição: nenhuma
// Pós-condição: caso não exista o arquivo será criado
void iniciaArquivo();

// Escreve os dados de um profissional em determinada posição do arquivo
// Pré-condição: ter um arquivo aberto, uma posição e um profissional 
// Pós-condição: os dados do profissional serão escritos no arquivo
void escreveProfissionalArq(FILE* arq, Profissional* profissional, int pos);

// Lê os dados de um profissional em determinada posição do arquivo
// Pré-condição: ter um arquivo aberto, uma posição
// Pós-condição: retornará um profissional com os dados lidos do arquivo
Profissional* leProfissionalArq(FILE* arq, int pos);

// Lê o cabeçalho do arquivo de dados
// Pré-condição: ter um arquivo aberto
// Pós-condição: retornará o cabecalho do arquivo de dados
CabProfissional* leCabProfissional(FILE* arq);

// Escreve o cabeçalho do arquivo de dados
// Pré-condição: ter um arquivo aberto e os dados do cabeçalho
// Pós-condição: os dados do cabeçalho serão escritos no arquivo
void escreveCabProfissional(FILE* arq, CabProfissional* cabecalho);


#endif //ARQPROFISSIONAL_H