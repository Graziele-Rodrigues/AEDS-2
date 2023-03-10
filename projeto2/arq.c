#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/arq.h"

// Lê o cabeçalho do arquivo e o armazena na memória
// Pré-condição: ponteiro válido para o arquivo
// Pós-condição: retorna o ponteiro do cabeçalho armazenado
Cab* leCab(FILE* arq){
  Cab* cab = (Cab*) malloc(sizeof(Cab));
	fseek(arq, 0, SEEK_SET);	// posiciona no in ́ıcio do arquivo
	fread(cab, sizeof(Cab), 1, arq);
	return cab;
}

// Escreve o cabeçalho no início do arquivo
// Pré-condição: ponteiro válido para o arquivo e para o cabeçalho
// Pós-condição: cabeçalho é escrito no arquivo
void escreveCab(FILE* arq, Cab* cabecalho){
  fseek(arq, 0, SEEK_SET); //posiciona no in ́ıcio do arquivo
  fwrite(cabecalho, sizeof(Cab), 1, arq); 
}

// Lê um nó da arvore nesta posição do arquivo
// Pré-condição: ponteiro válido para o arquivo, e posição existente
// Pós-condição: retorna o nó x
No* leNoArquivo(FILE* arq, int pos){
  No* x = (No*) malloc(sizeof(No));
	fseek(arq, 0, SEEK_SET);	// posiciona no in ́ıcio do arquivo
	fseek(arq, sizeof(Cab) + pos* sizeof(No), SEEK_SET);
	fread(x, sizeof(No), 1, arq);
	return x;
}

// Escreve um nó da arvore em determinada posição do arquivo
// Pré-condição: Nó, arquivo e posição válidos
// Pós-condição: nó é escrito no arquivo
void escrevenoArq(FILE* arq, No* no, int pos){
  fseek(arq, 0, SEEK_SET); //posiciona no in ́ıcio do arquivo
  fseek(arq, sizeof(Cab) + pos * sizeof(No), SEEK_SET);
  fwrite(no, sizeof(No), 1, arq);
}


// Insere um novo nó no arquivo
// Pré-condição: ponteiro válido para o arquivo e para o nó
// Pós-condição: retorna a posição do nó criado
int insereNoArq(FILE* arq, No* no){
  Cab* cab = leCab(arq);
  int pos = -1;
	
  if(cab->livre == -1){
    escrevenoArq(arq,no,cab->topo);
    pos = cab->topo;
    cab->topo = cab->topo +1;
	}
	else{ 
    No* listaVazia = leNoArquivo(arq,cab->livre);
		pos = cab->livre;
		escrevenoArq(arq,no,cab->livre);
    cab->livre = listaVazia->numChaves;   
  }
	
	escreveCab(arq,cab);
	return pos;
}

// Remove um nó do arquivo (deixa ele livre para novos dados)
// Pré-condição: Ponteiro válido para o arquivo e para o nó, e posição existente
// Pós-condição: nó é "removido" do arquivo e sua posição adicionada como livre
void removeNoArq(FILE* arq, No* no, int pos){
  Cab* cab = leCab(arq);
	
  no->numChaves = cab->livre;
  cab->livre = pos;
	
  escrevenoArq(arq,no,pos);
  escreveCab(arq,cab);
}
