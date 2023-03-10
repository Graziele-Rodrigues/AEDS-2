#ifndef PROFISSIONAL_H
#define PROFISSIONAL_H

#include "arvore.h"
#include "arq.h"
#include "arqProfissional.h"
#include "menu.h"

typedef struct Profissional{   
  int codigo;						
  char nome[51];
  char cpf[12];
  char registro[31];
  char endereco[101];
	char telefone[16];
} Profissional;

typedef struct auxProfissional{
  int chave;  //codigo do profissional
  int posDado; //posicao do profissional no arquivo
} AuxProfissional;

// Cria um profissional
// Pré-condição: nenhuma
// Pós-condição: é alocado uma struct de profissional e retornado o ponteiro deste
Profissional* criarProfissional(int codigo, char nome[], char cpf[], char registro[], char endereco[], char telefone[]);

// Imprime os dados de um determinado profissional a partir de um ponteiro
// Pré-condição: ter um ponteiro para um profissional
// Pós-condição: os dados serão impressos
void mostrarProfissional(Profissional* profissional);

// Imprime os dados de um determinado profissional a partir de uma posição no arquivo
// Pré-condição: ter uma posição no arquivo
// Pós-condição: os dados serão impressos
void mostrarProfissionalPorPos(int pos);

// Edita o endereço do profissional que esteja em uma determinada posição do arquivo
// Pré-condição: ter uma posição do arquivo e uma string do novo endereço
// Pós-condição: o endereço do profissional do arquivo de dados será alterado
void editarEndereco(int pos, char novoEndereco[]);

// Edita o telefone do profissional que esteja em uma determinada posição do arquivo
// Pré-condição: ter uma posição do arquivo e uma string do novo telefone
// Pós-condição: o telefone do profissional do arquivo de dados será alterado
void editarTelefone(int pos, char novoTelefone[]);

// Remove um profissional que esteja em uma determinada posição do arquivo
// Pré-condição: ter uma posição no arquivo
// Pós-condição: a posição será considerada como livre e será alterado o cabeçalho e/ou alguma outra posição que também estava livre anteriormente
void removerProfissional(int pos);


// Será incluido no arquivo um profissional que está na memória primária 
// Pré-condição: ter um ponteiro para profissional
// Pós-condição: será escrito no arquivo de dados o profissional e retorna a posição em que foi escrito 
int inserirProfissional(Profissional* profissional);

#endif //PROFISSIONAL_H