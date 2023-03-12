#ifndef ARVORE_H
#define ARVORE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//define a ordem da árvore e em qual arquivo será salva
#define arquivoArvore "arvore.bin" 
#define ORDEM 5

typedef struct no {
  int posDado[ORDEM]; // posicao do profissional no aquivo de profissionais
  int chave[ORDEM]; 
  int filho[ORDEM +1]; 
  int numChaves; 
} No;

typedef struct auxNo {
	int codigo; //codigo do profissional
  int posDado; //posicao do dado no arquivo
} AuxNo;

// Verifica se existe um arquivo de árvore, caso não exista cria e adiciona o cabeçalho
// Pré-condição: nenhuma
// Pós-condição: caso não exista o arquivo será criado
void iniciarArvore();

// Verifica se o nó está com mais filhos que o máximo
// Pré-condição: ponteriro para um nó
// Pós-condição: retorna um caso estejá com overflow ou zero caso contrário
int overflow(No* r);

// Verifica se o nó tem o mínimo de chaves necessário
// Pré-condição: ponteriro para um nó
// Pós-condição: retorna um caso falte chaves ou zero caso contrário
int faltaChave(No* r);

// Verifica se o nó é uma folha
// Pré-condição: ponteriro para um nó
// Pós-condição: retorna um caso o nó seja uma folha ou zero caso contrário
int ehFolha(No* r);

// Cria um novo nó na memória
// Pré-condição: nenhuma
// Pós-condição: retorna o ponteiro para o nó criado
No* criaNo();

// Cria um novo nó e escreve nele metade dos dados do nó anterior
// Pré-condição: ponteiro para o arquivo de indices, ponteiro para um nó e um ponteiro para um auxiliar de nó
// Pós-condição: retorna a posição no arquivo onde o novo nó foi armazenado
int split(FILE* arq, No* no, AuxNo* auxNo);

// Insere um código na árvore
// Pré-condição: um ponteiro para arquivo, a posição que será inserido e um ponteiro para um auxiliar de no
// Pós-condição: é inserido o codigo na árvore e retorna a psoição
int insereCodigo(FILE* arq, int pos, AuxNo* auxNo);

// Imprime os dados de todos os profissionais daquela árvore
// Pré-condição: um ponteiro para arquivo e a posição da raiz
// Pós-condição: nenhuma
void mostraFuncionarios(FILE* arq, int pos);

// Busca a posição de um código na sub-árvore
// Pré-condição: um ponteiro para arquivo, a posição da raiz e o código a ser buscado
// Pós-condição: retorna a posição no arquivo
int posDado(FILE* arq, int pos, int codigo);

// Busca o maior código da sub-árvore
// Pré-condição: um ponteiro para arquivo e a posição da raiz
// Pós-condição: retorna o maior código naquela sub-árvore
AuxNo* maiorCodigo(FILE* arq, int pos);

// Remove um profissional da árvore
// Pré-condição: ponteiro para arquivo, posição do profissional na árvore, código do profissional a ser removido, posição do pai na árvore e um valor que determina se há mais nós a direita
// Pós-condição: será removido da árvore o código do profissional e a árvore será ajustada para que continue balanceada
int removeCodigo(FILE* arq, int pos, int codigo, int posPai,int desceDireita);

// Verifica a altura da árvore
// Pré-condição: ponteriro para o arquivo de indices e posição da raiz
// Pós-condição: retorna a altura da árvore
int alturaDaArvore(FILE* arq, int pos);

// Imprime os códigos de um determinado nível da árvore
// Pré-condição: ponteriro para o arquivo de indices, posição da raiz, nível que se deseja imprimir e o nível atual para a recursão
// Pós-condição: será impresso os códigos do nível 
void mostraNivel(FILE* arq, int pos, int nivel, int nivelAtual);

// Imprime todos os códigos da árvore por nível
// Pré-condição: nenhuma
// Pós-condição: será impresso os códigos por nível
void arvorePorNivel();

#endif //ARVORE_H