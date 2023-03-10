#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/profissional.h"

// Cria um profissional
// Pré-condição: nenhuma
// Pós-condição: é alocado uma struct de profissional e retornado o ponteiro deste
Profissional* criarProfissional(int codigo, char nome[], char cpf[], char registro[], char endereco[], char telefone[]){
  Profissional* novoProfissional = (Profissional *)malloc(sizeof(Profissional));

  novoProfissional->codigo = codigo;
  strcpy(novoProfissional->nome, nome);
	strcpy(novoProfissional->cpf, cpf);
	strcpy(novoProfissional->registro, registro);
	strcpy(novoProfissional->endereco, endereco);
  strcpy(novoProfissional->telefone, telefone);

  return novoProfissional;
}

// Imprime os dados de um determinado profissional a partir de um ponteiro
// Pré-condição: ter um ponteiro para um profissional
// Pós-condição: os dados serão impressos
void mostrarProfissional(Profissional* profissional){
  printf("\ncodigo: %d\n", profissional->codigo);
  printf("nome: %s\n", profissional->nome);
  printf("cpf: %s\n", profissional->cpf);
	printf("registro: %s\n", profissional->registro);
  printf("endereco: %s\n", profissional->endereco);
  printf("telefone: %s\n\n", profissional->telefone); 
}

// Imprime os dados de um determinado profissional a partir de uma posição no arquivo
// Pré-condição: ter uma posição no arquivo
// Pós-condição: os dados serão impressos
void mostrarProfissionalPorPos(int pos){
  FILE *arq = fopen(arquivoProfissional, "rb+");
	Profissional* profissional = leProfissionalArq(arq, pos);
	mostrarProfissional(profissional);
	free(profissional);
	fclose(arq);
}

// Edita o endereço do profissional que esteja em uma determinada posição do arquivo
// Pré-condição: ter uma posição do arquivo e uma string do novo endereço
// Pós-condição: o endereço do profissional do arquivo de dados será alterado
void editarEndereco(int pos, char novoEndereco[]){
  FILE *arq = fopen(arquivoProfissional, "rb+");
  Profissional* profissional = leProfissionalArq(arq, pos);

  strcpy(profissional->endereco, novoEndereco);
  escreveProfissionalArq(arq, profissional, pos);
	free(profissional);
  fclose(arq);
}

// Edita o telefone do profissional que esteja em uma determinada posição do arquivo
// Pré-condição: ter uma posição do arquivo e uma string do novo telefone
// Pós-condição: o telefone do profissional do arquivo de dados será alterado
void editarTelefone(int pos, char novoTelefone[]){
  FILE *arq = fopen(arquivoProfissional, "rb+");
  Profissional* profissional = leProfissionalArq(arq, pos);

  strcpy(profissional->telefone, novoTelefone);
  escreveProfissionalArq(arq, profissional, pos);
	free(profissional);
  fclose(arq);
}

// Remove um profissional que esteja em uma determinada posição do arquivo
// Pré-condição: ter uma posição no arquivo
// Pós-condição: a posição será considerada como livre e será alterado o cabeçalho e/ou alguma outra posição que também estava livre anteriormente
void removerProfissional(int pos){
  FILE *arq = fopen(arquivoProfissional, "rb+");
  Profissional *profissionalDeletado = leProfissionalArq(arq, pos);
  CabProfissional *cabecalho = leCabProfissional(arq);

  profissionalDeletado->codigo = cabecalho->posLivre;
  cabecalho->posLivre = pos;

  escreveCabProfissional(arq, cabecalho);
  escreveProfissionalArq(arq, profissionalDeletado, pos);
	free(profissionalDeletado);
	free(cabecalho);
  fclose(arq);
}

// Será incluido no arquivo um profissional que está na memória primária 
// Pré-condição: ter um ponteiro para profissional
// Pós-condição: será escrito no arquivo de dados o profissional e retorna a posição em que foi escrito 
int inserirProfissional(Profissional* profissional){
  FILE *arq = fopen(arquivoProfissional, "rb+");
  CabProfissional *cab = leCabProfissional(arq);
  int pos;

  if(cab->posLivre == -1){
    escreveProfissionalArq(arq, profissional, cab->posTopo);
    pos = cab->posTopo;
    cab->posTopo++;
  }
	else {
    Profissional *profissionalDeletado = leProfissionalArq(arq,cab->posLivre);
		escreveProfissionalArq(arq,profissional,cab->posLivre);
    pos = cab->posLivre;
    cab->posLivre = profissionalDeletado->codigo;
		free(profissionalDeletado);
  }

  escreveCabProfissional(arq,cab);
	free(cab);
  fclose(arq);
  return pos;
}