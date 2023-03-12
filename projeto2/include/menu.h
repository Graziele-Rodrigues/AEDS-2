#ifndef MENU_H
#define MENU_H

#include "funcionario.h"


// Chama uma função que imprime as opções, lê a entra da do usuário e chama a função escolhida pelo usuário
// Pré-condição: Nenhuma
// Pós-condição: Chamará a função desejada pelo usuário
void menuEntrada();

// Insere um profissional na árvore
// Pré-condição: ter uma struct de profissional com os dados que o código não existir na árvore
// Pós-condição: o profissional será adicionado a árvore
void insereFuncionario(Funcionario* funcionario);

// Remove um profissional na árvore a partir de um código que esteja presente na árvore
// Pré-condição: ter o código de um profissional que exista na árvore
// Pós-condição: o profissional será removido da árvore
void removeFuncionario(int codigo);

// Altera o endereço e/ou telefone de um determinado profissional
// Pré-condição: ter o código de um profissional que exista na árvore, o novo endereço e/ou telefone e o que será alterado endereço e/ou telefone
// Pós-condição: será alterado o endereço e/ou telefone do profissional
//void alteraFuncionario(int cod, char nome[], double salario);

// Lê os dados e insere um profissional
// Pré-condição: o código não existir na árvore
// Pós-condição: o profissional será adicionado a árvore
void inserir();

// Remove um profissional da árvore
// Pré-condição: o código existir na árvore
// Pós-condição: o profissional será removido da árvore
void remover();


/*
// Altera o endereço de um determinado profissional
// Pré-condição: o código existir na árvore
// Pós-condição: será alterado o endereço do profissional
void alterarEndereco();

// Altera o telefone de um determinado profissional
// Pré-condição: o código existir na árvore
// Pós-condição: será alterado o telefone do profissional
void alterarTelefone();


// Executa as operações de inserção, remoção e alteração lidas de um arquivo de texto
// Pré-condição: ter um arquivo de texto válido
// Pós-condição: será executado o que foi determinado pelo arquivo
void carregarArq();

*/

// Imprime os dados de um profissional
// Pré-condição: o profissional estar na árvore
// Pós-condição: nenhuma
void mostrar();

// Imprime os dados de todos os profissional presentes na árvore
// Pré-condição: nenhuma
// Pós-condição: nenhuma 
void listarCodigos();

// Imprime todos os códigos da forma como eles estariam na árvore
// Pré-condição: nenhuma
// Pós-condição: nenhuma
void imprimirNivel();

// Imprime todas as posições livre no arquivo de dados
// Pré-condição: nenhuma
// Pós-condição: nenhuma
void mostrarDadosLivres();

// Imprime todas as posições livre no arquivo de indices
// Pré-condição: nenhuma
// Pós-condição: nenhuma
void mostrarIndicesLivres();

#endif //MENU_H