#ifndef FUNCIONARIO_H
#define FUNCIONARIO_H


typedef struct Funcionario {
    int cod;
    char nome[50];
    double salario;
} Funcionario;


#define NUM_EMPLOYEES 100
int id[NUM_EMPLOYEES];
int is_unique;

typedef struct auxFuncionario{
  int chave;  //codigo do profissional
  int posDado; //posicao do profissional no arquivo
} AuxFuncionario;

// Cria um profissional
// Pré-condição: nenhuma
// Pós-condição: é alocado uma struct de profissional e retornado o ponteiro deste
Funcionario* criarFuncionario(int cod, char nome[], double salario);

// Imprime os dados de um determinado profissional a partir de um ponteiro
// Pré-condição: ter um ponteiro para um profissional
// Pós-condição: os dados serão impressos
void mostrarFuncionario(Funcionario* funcionario);

// Imprime os dados de um determinado profissional a partir de uma posição no arquivo
// Pré-condição: ter uma posição no arquivo
// Pós-condição: os dados serão impressos
void mostrarFuncionarioPorPos(int pos);

/*
// Edita o endereço do profissional que esteja em uma determinada posição do arquivo
// Pré-condição: ter uma posição do arquivo e uma string do novo endereço
// Pós-condição: o endereço do profissional do arquivo de dados será alterado
void editarEndereco(int pos, char novoEndereco[]);

// Edita o telefone do profissional que esteja em uma determinada posição do arquivo
// Pré-condição: ter uma posição do arquivo e uma string do novo telefone
// Pós-condição: o telefone do profissional do arquivo de dados será alterado
void editarTelefone(int pos, char novoTelefone[]); */

// Remove um profissional que esteja em uma determinada posição do arquivo
// Pré-condição: ter uma posição no arquivo
// Pós-condição: a posição será considerada como livre e será alterado o cabeçalho e/ou alguma outra posição que também estava livre anteriormente
void removerFuncionario(int pos);

// Será incluido no arquivo um funcionario que está na memória primária 
// Pré-condição: ter um ponteiro para profissional
// Pós-condição: será escrito no arquivo de dados o profissional e retorna a posição em que foi escrito 
int inserirFuncionario(Funcionario* funcionario);

#endif //PROFISSIONAL_H