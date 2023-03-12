#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/funcionario.h"
#include "include/arqFuncionario.h"


// Cria um profissional
// Pré-condição: nenhuma
// Pós-condição: é alocado uma struct de profissional e retornado o ponteiro deste
Funcionario* criarFuncionario(int cod, char nome[], double salario){
    Funcionario *func = (Funcionario*) malloc(sizeof(Funcionario));
    //inicializa espaÃƒÂ§o de memÃƒÂ³ria com ZEROS
    if (func) memset(func, 0, sizeof(Funcionario));
    //copia valores para os campos de func
    func->cod = cod;
    strcpy(func->nome, nome);
    func->salario = salario;
    return func;
}

// Imprime os dados de um determinado profissional a partir de um ponteiro
// Pré-condição: ter um ponteiro para um profissional
// Pós-condição: os dados serão impressos
void mostrarFuncionario(Funcionario *func){
    printf("**********************************************");
    printf("\nFuncionario de codigo: ");
    printf("%d", func->cod);
    printf("\nNome: ");
    printf("%s", func->nome);
    printf("\nSalario: ");
    printf("%4.2f", func->salario);
    printf("\n**********************************************");
}

// Imprime os dados de um determinado profissional a partir de uma posição no arquivo
// Pré-condição: ter uma posição no arquivo
// Pós-condição: os dados serão impressos
void mostrarFuncionarioPorPos(int pos){
  FILE *arq = fopen(arquivoFuncionario, "rb+");
	Funcionario* funcionario = leFuncionarioArq(arq, pos);
	mostrarFuncionario(funcionario);
	free(funcionario);
	fclose(arq);
}

/**
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
}*/

// Remove um profissional que esteja em uma determinada posição do arquivo
// Pré-condição: ter uma posição no arquivo
// Pós-condição: a posição será considerada como livre e será alterado o cabeçalho e/ou alguma outra posição que também estava livre anteriormente
void removerFuncionario(int pos){
  FILE *arq = fopen(arquivoFuncionario, "rb+");
  Funcionario *funcionarioDeletado = leFuncionarioArq(arq, pos);
  CabFuncionario *cabecalho = leCabFuncionario(arq);

  funcionarioDeletado->cod = cabecalho->posLivre;
  cabecalho->posLivre = pos;

  escreveCabFuncionario(arq, cabecalho);
  escreveFuncionarioArq(arq, funcionarioDeletado, pos);
	free(funcionarioDeletado);
	free(cabecalho);
  fclose(arq);
}

// Será incluido no arquivo um profissional que está na memória primária 
// Pré-condição: ter um ponteiro para profissional
// Pós-condição: será escrito no arquivo de dados o profissional e retorna a posição em que foi escrito 
int inserirFuncionario(Funcionario* funcionario){
  FILE *arq = fopen(arquivoFuncionario, "rb+");
  CabFuncionario *cab = leCabFuncionario(arq);
  int pos;

  if(cab->posLivre == -1){
    escreveFuncionarioArq(arq, funcionario, cab->posTopo);
    pos = cab->posTopo;
    cab->posTopo++;
  }
	else {
    Funcionario *funcionarioDeletado = leFuncionarioArq(arq,cab->posLivre);
		escreveFuncionarioArq(arq,funcionario,cab->posLivre);
    pos = cab->posLivre;
    cab->posLivre = funcionarioDeletado->cod;
		free(funcionarioDeletado);
  }

  escreveCabFuncionario(arq,cab);
	free(cab);
  fclose(arq);
  return pos;
}