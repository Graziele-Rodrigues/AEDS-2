#ifndef ARQ_H
#define ARQ_H

#include "arvore.h"
#include "arqProfissional.h"
#include "menu.h"
#include "profissional.h"

#define arquivoArvore "arvore.bin"

struct Cab {
  int raiz;
  int livre;
  int topo;
} typedef Cab;

// Lê o cabeçalho do arquivo e o armazena na memória
// Pré-condição: ponteiro válido para o arquivo
// Pós-condição: retorna o ponteiro do cabeçalho armazenado
Cab* leCab(FILE* arq);

// Escreve o cabeçalho no início do arquivo
// Pré-condição: ponteiro válido para o arquivo e para o cabeçalho
// Pós-condição: cabeçalho é escrito no arquivo
void escreveCab(FILE* arq, Cab* cabecalho);

// Lê um nó da arvore nesta posição do arquivo
// Pré-condição: ponteiro válido para o arquivo, e posição existente
// Pós-condição: retorna o nó x
No* leNoArquivo(FILE* arq, int pos);

// Escreve um nó da arvore em determinada posição do arquivo
// Pré-condição: Nó, arquivo e posição válidos
// Pós-condição: nó é escrito no arquivo
void escrevenoArq(FILE* arq, No* no, int pos);

// Insere um novo nó no arquivo
// Pré-condição: ponteiro válido para o arquivo e para o nó
// Pós-condição: retorna a posição do nó criado
int insereNoArq(FILE* arq, No* no);

// Remove um nó do arquivo (deixa ele livre para novos dados)
// Pré-condição: Ponteiro válido para o arquivo e para o nó, e posição existente
// Pós-condição: nó é "removido" do arquivo e sua posição adicionada como livre
void removeNoArq(FILE* arq, No* no, int pos);

#endif //ARQ_H