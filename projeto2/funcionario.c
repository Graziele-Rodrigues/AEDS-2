#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/funcionario.h"
#include "include/arqFuncionario.h"


// Cria um funcionario
// Pré-condição: nenhuma
// Pós-condição: é alocado uma struct de funcionario e retornado o ponteiro deste
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

// Imprime os dados de um determinado funcionario a partir de um ponteiro
// Pré-condição: ter um ponteiro para um funcionario
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

// Imprime os dados de um determinado funcionario a partir de uma posição no arquivo
// Pré-condição: ter uma posição no arquivo
// Pós-condição: os dados serão impressos
void mostrarFuncionarioPorPos(int pos){
  FILE *arq = fopen(arquivoFuncionario, "rb+");
	Funcionario* funcionario = leFuncionarioArq(arq, pos);
	mostrarFuncionario(funcionario);
	free(funcionario);
	fclose(arq);
}

// Remove um funcionario que esteja em uma determinada posição do arquivo
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

// Será incluido no arquivo um funcionario que está na memória primária 
// Pré-condição: ter um ponteiro para funcionario
// Pós-condição: será escrito no arquivo de dados o funcionario e retorna a posição em que foi escrito 
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