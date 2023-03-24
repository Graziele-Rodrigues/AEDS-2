#ifndef FUNCIONARIO_H
#define FUNCIONARIO_H


//estrutura com dados a serem preenchidos do funcionario
typedef struct Funcionario {
    int cod;
    char nome[50];
    double salario;
} Funcionario;


#define NUM_EMPLOYEES 1000
int id[NUM_EMPLOYEES];
int is_unique;5

typedef struct auxFuncionario{
  int chave;  //codigo do funcionario
  int posDado; //posicao do funcionario no arquivo
} AuxFuncionario;

// Cria um funcionario
// Pré-condição: nenhuma
// Pós-condição: é alocado uma struct de um funcionario e retornado o ponteiro deste
Funcionario* criarFuncionario(int cod, char nome[], double salario);

// Imprime os dados de um determinado funcionario a partir de um ponteiro
// Pré-condição: ter um ponteiro para um funcionario
// Pós-condição: os dados serão impressos
void mostrarFuncionario(Funcionario* funcionario);

// Imprime os dados de um determinado funcionario a partir de uma posição no arquivo
// Pré-condição: ter uma posição no arquivo
// Pós-condição: os dados serão impressos
void mostrarFuncionarioPorPos(int pos);

// Remove um funcionario que esteja em uma determinada posição do arquivo
// Pré-condição: ter uma posição no arquivo
// Pós-condição: a posição será considerada como livre e será alterado o cabeçalho e/ou alguma outra posição que também estava livre anteriormente
void removerFuncionario(int pos);

// Será incluido no arquivo um funcionario que está na memória primária 
// Pré-condição: ter um ponteiro para funcionario
// Pós-condição: será escrito no arquivo de dados o funcionario e retorna a posição em que foi escrito 
int inserirFuncionario(Funcionario* funcionario);

#endif //PROFISSIONAL_H