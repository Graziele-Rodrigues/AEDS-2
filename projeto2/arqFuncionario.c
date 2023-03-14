#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "include/arqFuncionario.h"
#include "include/funcionario.h"


// Verifica se existe um arquivo de dados, caso não exista cria e adiciona o cabeçalho
// Pré-condição: nenhuma
// Pós-condição: caso não exista o arquivo será criado
void iniciaArquivo(){
  FILE* arq = fopen(arquivoFuncionario, "ab+");
	CabFuncionario* cabecalho = (CabFuncionario*) malloc(sizeof(CabFuncionario));
	fseek(arq, 0, SEEK_SET);
	if(fread(cabecalho, sizeof(CabFuncionario), 1, arq)){
		free(cabecalho);	
		fclose(arq);
		return;
	}
	cabecalho->posLivre = -1;
	cabecalho->posTopo = 0;
	escreveCabFuncionario(arq,cabecalho);
	free(cabecalho);	
	fclose(arq);
}

// Escreve os dados de um funcionario em determinada posição do arquivo
// Pré-condição: ter um arquivo aberto, uma posição e um funcionario 
// Pós-condição: os dados do funcionario serão escritos no arquivo
void escreveFuncionarioArq(FILE* arq, Funcionario* funcionario, int pos) {
	fseek(arq, 0, SEEK_SET); //posiciona no in ́ıcio do arquivo
  fseek(arq, sizeof(CabFuncionario) + pos * sizeof(Funcionario), SEEK_SET);
  fwrite(funcionario, sizeof(Funcionario), 1, arq);
}

// Lê os dados de um funcionario em determinada posição do arquivo
// Pré-condição: ter um arquivo aberto, uma posição
// Pós-condição: retornará um funcionario com os dados lidos do arquivo
Funcionario* leFuncionarioArq(FILE* arq, int pos){
	Funcionario* x = malloc(sizeof(Funcionario));
	fseek(arq, 0, SEEK_SET);	
	fseek(arq, sizeof(CabFuncionario) + pos * sizeof(Funcionario), SEEK_SET);
	fread(x, sizeof(Funcionario), 1, arq);
	return x;
}

// Lê o cabeçalho do arquivo de dados
// Pré-condição: ter um arquivo aberto
// Pós-condição: retornará o cabecalho do arquivo de dados
CabFuncionario* leCabFuncionario(FILE* arq){
	CabFuncionario* cab = (CabFuncionario*) malloc(sizeof(CabFuncionario));
	fseek(arq, 0, SEEK_SET);
	fread(cab, sizeof(CabFuncionario), 1, arq);
	return cab;
}

// Escreve o cabeçalho do arquivo de dados
// Pré-condição: ter um arquivo aberto e os dados do cabeçalho
// Pós-condição: os dados do cabeçalho serão escritos no arquivo
void escreveCabFuncionario(FILE* arq, CabFuncionario* cabecalho) {
  fseek(arq, 0, SEEK_SET);
  fwrite(cabecalho, sizeof(CabFuncionario), 1, arq); 
}