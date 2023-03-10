#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "include/arqProfissional.h"

// Verifica se existe um arquivo de dados, caso não exista cria e adiciona o cabeçalho
// Pré-condição: nenhuma
// Pós-condição: caso não exista o arquivo será criado
void iniciaArquivo(){
  FILE* arq = fopen(arquivoProfissional, "ab+");
	CabProfissional* cabecalho = (CabProfissional*) malloc(sizeof(CabProfissional));
	fseek(arq, 0, SEEK_SET);
	if(fread(cabecalho, sizeof(CabProfissional), 1, arq)){
		free(cabecalho);	
		fclose(arq);
		return;
	}
	cabecalho->posLivre = -1;
	cabecalho->posTopo = 0;
	escreveCabProfissional(arq,cabecalho);
	free(cabecalho);	
	fclose(arq);
}

// Escreve os dados de um profissional em determinada posição do arquivo
// Pré-condição: ter um arquivo aberto, uma posição e um profissional 
// Pós-condição: os dados do profissional serão escritos no arquivo
void escreveProfissionalArq(FILE* arq, Profissional* profissional, int pos) {
	fseek(arq, 0, SEEK_SET); //posiciona no in ́ıcio do arquivo
  fseek(arq, sizeof(CabProfissional) + pos * sizeof(Profissional), SEEK_SET);
  fwrite(profissional, sizeof(Profissional), 1, arq);
}

// Lê os dados de um profissional em determinada posição do arquivo
// Pré-condição: ter um arquivo aberto, uma posição
// Pós-condição: retornará um profissional com os dados lidos do arquivo
Profissional* leProfissionalArq(FILE* arq, int pos){
	Profissional* x = malloc(sizeof(Profissional));
	fseek(arq, 0, SEEK_SET);	
	fseek(arq, sizeof(CabProfissional) + pos * sizeof(Profissional), SEEK_SET);
	fread(x, sizeof(Profissional), 1, arq);
	return x;
}

// Lê o cabeçalho do arquivo de dados
// Pré-condição: ter um arquivo aberto
// Pós-condição: retornará o cabecalho do arquivo de dados
CabProfissional* leCabProfissional(FILE* arq){
	CabProfissional* cab = (CabProfissional*) malloc(sizeof(CabProfissional));
	fseek(arq, 0, SEEK_SET);
	fread(cab, sizeof(CabProfissional), 1, arq);
	return cab;
}

// Escreve o cabeçalho do arquivo de dados
// Pré-condição: ter um arquivo aberto e os dados do cabeçalho
// Pós-condição: os dados do cabeçalho serão escritos no arquivo
void escreveCabProfissional(FILE* arq, CabProfissional* cabecalho) {
  fseek(arq, 0, SEEK_SET);
  fwrite(cabecalho, sizeof(CabProfissional), 1, arq); 
}