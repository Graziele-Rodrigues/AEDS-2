#ifndef MENU_H
#define MENU_H

#include "funcionario.h"


// Chama uma função que imprime as opções, lê a entra da do usuário e chama a função escolhida pelo usuário
// Pré-condição: Nenhuma
// Pós-condição: Chamará a função desejada pelo usuário
void menuEntrada();

// Insere um profissional na árvore
// Pré-condição: ter uma struct de funcionario com os dados que o código não existir na árvore
// Pós-condição: o funcionario será adicionado a árvore
void insereFuncionario(Funcionario* funcionario);

// Remove um funcionario na árvore a partir de um código que esteja presente na árvore
// Pré-condição: ter o código de um funcionario que exista na árvore
// Pós-condição: o funcionario será removido da árvore
void removeFuncionario(int codigo);

// Lê os dados e insere um funcionario
// Pré-condição: o código não existir na árvore
// Pós-condição: o funcionario será adicionado a árvore
void inserir();

// Remove um funcionario da árvore
// Pré-condição: o código existir na árvore
// Pós-condição: o funcionario será removido da árvore
void remover();


// Imprime os dados de um funcionario
// Pré-condição: o funcionario estar na árvore
// Pós-condição: nenhuma
void mostrar();

// Imprime os dados de todos os funcionario presentes na árvore
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