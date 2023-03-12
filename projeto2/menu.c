#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "include/menu.h"

#include "include/arvore.h"
#include "include/arq.h"
#include "include/arqFuncionario.h"
#include "include/funcionario.h"


// Imprime o menu principal para o usuário
// Pré-condição: nenhuma
// Pós-condição: nenhuma
void mostraOpcoes(){
  printf("Selecione uma das opcoes abaixo\n\n");

  printf("0-> Inserir funcionarioS\n");                     
  printf("1-> Remover funcionario\n");             
  printf("2-> Ver informacoes de um funcionario\n");
  printf("3-> Listar funcionario\n"); 
  printf("4-> Imprimir arvore por nivel\n");     
  printf("5-> Imprimir posicoes livres Dados\n");    
  printf("6-> Imprimir posicoes livres Indices\n");           
  printf("-1-> Sair\n\n");
}

// Chama uma função que imprime as opções, lê a entra da do usuário e chama a função escolhida pelo usuário
// Pré-condição: Nenhuma
// Pós-condição: Chamará a função desejada pelo usuário
void menuEntrada(){
  int option = -2;
  while (option != -1){
    mostraOpcoes();
    scanf("%d", &option);

  	switch(option){
	    case 0: {
	      inserir();
	      break;
	    }
	    case 1:{
	      remover();
	      break;
	    }
	    case 2:{
	      mostrar();
	      break;
	    }
	    case 3:{
	      listarCodigos();
	      break;
	    }
	    case 4:{
	      imprimirNivel();
	      break;
	    }
	    case 5:
	    {
	      mostrarDadosLivres();
	      break;
	    }
	    case 6:
	    {
	      mostrarIndicesLivres();
	      break;
	    }
	    default:{
	      break;
	    }
    }
  }
}

// Insere um profissional na árvore
// Pré-condição: ter uma struct de profissional com os dados que o código não existir na árvore
// Pós-condição: o profissional será adicionado a árvore
void insereFuncionario(Funcionario* funcionario){
  FILE *arq = fopen(arquivoArvore, "rb+");
  Cab *cab = leCab(arq);
  int posFuncionario = posDado(arq,cab->raiz,funcionario->cod);

  if(posFuncionario != -1){
    printf("\n\nEsse funcionario ja existe no sistema, insercao cancelada.\n");
    fclose(arq);
    return;
  }

  AuxNo* auxNo = (AuxNo*)malloc(sizeof(AuxNo));
  auxNo->posDado = inserirFuncionario(funcionario);
  auxNo->codigo = funcionario->cod;
  int posRaiz = insereCodigo(arq,cab->raiz,auxNo);
  cab = leCab(arq);
  cab->raiz = posRaiz;
  escreveCab(arq,cab);
	
	free(auxNo);
  fclose(arq);
}

// Remove um profissional na árvore a partir de um código que esteja presente na árvore
// Pré-condição: ter o código de um profissional que exista na árvore
// Pós-condição: o profissional será removido da árvore
void removeFuncionario(int codigo){
  FILE *arq = fopen(arquivoArvore, "rb+");
  Cab *cab = leCab(arq);
  int posFuncionario = posDado(arq,cab->raiz,codigo);
  
  if(posFuncionario == -1){
    printf("\n\nEsse profissional nao existe no sistema, remocao cancelada.\n");
    fclose(arq);
    return;
  }
  
  int posRaiz = removeCodigo(arq,cab->raiz,codigo,-1,0);
  cab = leCab(arq);
  cab->raiz = posRaiz;
	
  escreveCab(arq,cab);
  fclose(arq);
}

/*
// Altera o endereço e/ou telefone de um determinado profissional
// Pré-condição: ter o código de um profissional que exista na árvore, o novo endereço e/ou telefone e o que será alterado endereço e/ou telefone
// Pós-condição: será alterado o endereço e/ou telefone do profissional
void alteraProfissional(int codigo, char* endereco, char* telefone, int tEnd, int tTel){
  FILE *arq = fopen(arquivoArvore, "rb+");
  Cab *cab = leCab(arq);
  int posProfissional = posDado(arq,cab->raiz,codigo);
  fclose(arq);
	
  if(posProfissional == -1){
    printf("\n\nEsse profissional nao existe no sistema, alteracao cancelada.\n");
    return;
  }
	
	if(tEnd) 
		editarEndereco(posProfissional, endereco);
	if(tTel) 
		editarTelefone(posProfissional, telefone);
}*/


// Lê os dados e insere um profissional
// Pré-condição: o código não existir na árvore
// Pós-condição: o profissional será adicionado a árvore

void generate_unique_id() {
    for (int i = 0; i < NUM_EMPLOYEES; i++) {
        do {
            is_unique = 1;
            id[i] = rand() % NUM_EMPLOYEES;
            for (int j = 0; j < i; j++) {
                if (id[i] == id[j]) {
                    is_unique = 0;
                    break;
                }
            }
        } while (!is_unique);
    }
}

void inserir(){
  printf("\nInserindo funcionarios\n");
  // Gerar nomes e salários aleatórios
    char *nome[] = {"Maria", "Jose", "Carlos", "Ana", "Paulo", "Lucia"};
    int num_names = sizeof(nome) / sizeof(nome[0]);
    double salario;
    generate_unique_id();
    for (int i = 0; i < NUM_EMPLOYEES; i++) {
        int random_index = rand() % num_names;
        salario = (double)rand() / RAND_MAX * (10000 - 1000) + 1000;
        Funcionario *funcionario = criarFuncionario(id[i], nome[random_index], salario);
        insereFuncionario(funcionario);
        free(funcionario);
  }
  printf("\nFuncionarios adicionados com sucesso!\n\n");
}

// Remove um profissional da árvore
// Pré-condição: o código existir na árvore
// Pós-condição: o profissional será removido da árvore
void remover(){
  printf("\nRemover um funcionario\n");
  printf("Insera o codigo a ser removido: \n");

  int codigo;
  printf("Codigo: ");
  scanf("%d",&codigo);
	removeFuncionario(codigo);

	printf("\nFuncionario removido com sucesso!\n\n");
}

/*

// Altera o endereço de um determinado profissional
// Pré-condição: o código existir na árvore
// Pós-condição: será alterado o endereço do profissional
void alterarEndereco(){
  printf("\nAlterar endereco de um profissional\n");
  printf("Insira os dados a seguir: \n");

  int codigo; char endereco[16];
  printf("Codigo: ");
  scanf("%d%*c",&codigo);
  printf("Novo endereco: ");
  scanf("%[^\n]%*c", endereco);
	
  alteraProfissional(codigo, endereco, NULL, 1, 0);

  printf("\Endereco alterado com sucesso!\n");
}

// Altera o telefone de um determinado profissional
// Pré-condição: o código existir na árvore
// Pós-condição: será alterado o telefone do profissional
void alterarTelefone(){
  printf("\nAlterar telefone de um profissional\n");
  printf("Insira os dados a seguir: \n");

  int codigo; char telefone[16];
  printf("Codigo: ");
  scanf("%d%*c",&codigo);
  printf("Novo telefone: ");
  scanf("%[^\n]%*c", telefone);
	
  alteraProfissional(codigo, NULL, telefone, 0, 1);

  printf("\nTelefone alterado com sucesso!\n");
}
*/

// Remove um determinado caracter do vetor
// Pré-condição: ter um vetor e uma posição que será removida
// Pós-condição: o caracter da posição esolhida seá removido
void removeChar(char linha[], int i, int* t){
	for(; i<=*t; i++)
		linha[i] = linha[i+1];
	*t = *t-1;
}

// Retira os espaços em branco do início, fim e dos lados dos pontos e vírgulas
// Pré-condição: ter um vetor de caracteres 
// Pós-condição: removerá os espaços em brancos indesejados
void removeBrancos(char linha[]){
	int i, t = strlen(linha);
	char* fim;
	
	while(linha[0] == ' ') removeChar(linha, 0, &t);
	
	for(i=0; i<t; i++){
		if(linha[i] == ' ' && (linha[i+1] == ';' || linha[i-1] == ';')) {
			removeChar(linha, i, &t);
			i-=2;
		}	
	}

	fim = linha + t - 1;
  while(fim > linha && isspace(*fim)) fim--;
  fim[1] = '\0';
}

/*
// Determina se o que foi lido para alteração é endereço ou telefone
// Pré-condição: ter um vetor de caracteres
// Pós-condição: retornará 0 caso seja um endereço e 1 se for telefone
int ehTelefone(char endereco[]){
	int i;
	for(i=0; endereco[i]; i++)
		if(!isdigit(endereco[i]))
			return 0;
	return 1;
}*/

/*
// Executa as operações de inserção, remoção e alteração lidas de um arquivo de texto
// Pré-condição: ter um arquivo de texto válido
// Pós-condição: será executado o que foi determinado pelo arquivo
void carregarArq(){
	char nomeArq[100];
  printf("\nCarregar informacoes de arquivo texto\n");
  printf("Insira o nome do arquivo de texto: \n");
	scanf("%*c%[^\n]%*c", nomeArq);
	printf("\n");
	
  FILE *arq = fopen(nomeArq, "r");
  char linha[300];

	while(fscanf(arq, "%[^\n]%*c", linha) != EOF) {
		removeBrancos(linha);
    char* token = strtok(linha, ";");
		
    if(strcmp(token,"I") == 0){
      int codigo = atoi(strtok(NULL, ";"));
      char* nome = strtok(NULL, ";");
      char* cpf = strtok(NULL, ";");
      char* registro = strtok(NULL, ";");
      char* endereco = strtok(NULL, ";");
			char* telefone = strtok(NULL, ";");
			Profissional* profissional = criarProfissional(codigo, nome, cpf, registro, endereco, telefone);
			insereProfissional(profissional);
			free(profissional);
		}
		
    if(strcmp(token,"R") == 0){
      int codigo = atoi(strtok(NULL, ";"));
      removeProfissional(codigo);
    }

    if(strcmp(token,"A") == 0){
			int temEndereco=0, temTelefone=0;
      int codigo = atoi(strtok(NULL, ";"));
      char* endereco = strtok(NULL, ";");
      char* telefone = strtok(NULL, ";");
			if(telefone != NULL) temEndereco = temTelefone = 1;
			else if(ehTelefone(endereco)){
				telefone = endereco;
				endereco = NULL;
				temTelefone = 1;
			}
			else temEndereco = 1;
			alteraProfissional(codigo, endereco, telefone, temEndereco, temTelefone);
		}
  }
  fclose(arq);
}*/

// Imprime os dados de um profissional
// Pré-condição: o profissional estar na árvore
// Pós-condição: nenhuma
void mostrar(){  
  printf("\nMostrar um funcionario\n");
  printf("Insira o codigo: \n");

  int codigo;
  printf("Codigo: ");
  scanf("%d",&codigo);

  FILE *arq = fopen(arquivoArvore, "rb+");
  Cab *cab = leCab(arq);
  int posFuncionario = posDado(arq,cab->raiz,codigo);
  fclose(arq);
	
  if(posFuncionario == -1){
    printf("\n\nEsse funcionario nao existe no sistema.\n");
    return;
  }
	
  mostrarFuncionarioPorPos(posFuncionario);
}

// Imprime os dados de todos os profissional presentes na árvore
// Pré-condição: nenhuma
// Pós-condição: nenhuma 
void listarCodigos(){
  FILE *arq = fopen(arquivoArvore, "rb+");
  Cab *cab = leCab(arq);
  mostraFuncionarios(arq,cab->raiz);
  fclose(arq);
}

// Imprime todos os códigos da forma como eles estariam na árvore
// Pré-condição: nenhuma
// Pós-condição: nenhuma
void imprimirNivel(){
  arvorePorNivel();
}

// Imprime todas as posições livre no arquivo de dados
// Pré-condição: nenhuma
// Pós-condição: nenhuma
void mostrarDadosLivres(){
  FILE *arq = fopen(arquivoFuncionario, "rb+");
  CabFuncionario *cab = leCabFuncionario(arq);
  int pos = cab->posLivre;
	printf("\n");
  if(pos == -1){
    printf("Sem posicoes livres no arquivo de dados.\n\n");
  }
  
  while(pos != -1){
    Funcionario* aux = leFuncionarioArq(arq,pos);
    printf("Posicao: %d\n",pos);
    pos = aux->cod;
		free(aux);
  }
	free(cab);
  fclose(arq);
}

// Imprime todas as posições livre no arquivo de indices
// Pré-condição: nenhuma
// Pós-condição: nenhuma
void mostrarIndicesLivres(){
  FILE *arq = fopen(arquivoArvore, "rb+");
  Cab *cab = leCab(arq);
	printf("\n");
  int pos = cab->livre;
	
  if(pos == -1){
    printf("Sem posicoes livres no arquivo de indices.\n\n");
  }
  
  while(pos != -1){
    No* aux = leNoArquivo(arq,pos);
    printf("Posicao: %d\n",pos);
    pos = aux->numChaves;
  }

}