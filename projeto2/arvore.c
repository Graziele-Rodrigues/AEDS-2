#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/arvore.h"


// Verifica se existe um arquivo de árvore, caso não exista cria e adiciona o cabeçalho
// Pré-condição: nenhuma
// Pós-condição: caso não exista o arquivo será criado
void iniciarArvore() {
  FILE* arq = fopen(arquivoArvore, "ab+");
	Cab* cab = (Cab*) malloc(sizeof(Cab));
	fseek(arq, 0, SEEK_SET);
	if(fread(cab, sizeof(Cab), 1, arq)){
		free(cab);	
		fclose(arq);
		return;
	}
	cab->raiz = -1;
  cab->livre = -1;
  cab->topo = 0;
  escreveCab(arq, cab);
	
	free(cab);
  fclose(arq);
}

// Verifica se o nó está com mais filhos que o máximo
// Pré-condição: ponteriro para um nó
// Pós-condição: retorna um caso estejá com overflow ou zero caso contrário
int overflow(No* r) {
  return r->numChaves == ORDEM;
}

// Verifica se o nó tem o mínimo de chaves necessário
// Pré-condição: ponteriro para um nó
// Pós-condição: retorna um caso falte chaves ou zero caso contrário
int faltaChave(No* r) {
  return r->numChaves < ORDEM/2;
}

// Verifica se o nó é uma folha
// Pré-condição: ponteriro para um nó
// Pós-condição: retorna um caso o nó seja uma folha ou zero caso contrário
int ehFolha(No* r) {
  return r->filho[0] == -1;
}

// Cria um novo nó na memória
// Pré-condição: nenhuma
// Pós-condição: retorna o ponteiro para o nó criado
No* criaNo() {
  No* no = (No*) malloc(sizeof(No));
  no->numChaves = 0;
	int i;
  for(i = 0; i < ORDEM; i++){
    no->posDado[i] = -1;
    no->chave[i] = -1;
    no->filho[i] = -1;
  }

  no->filho[ORDEM] = -1;
  return no;
}

// Cria um novo nó e escreve nele metade dos dados do nó anterior
// Pré-condição: ponteiro para o arquivo de indices, ponteiro para um nó e um ponteiro para um auxiliar de nó
// Pós-condição: retorna a posição no arquivo onde o novo nó foi armazenado
int split(FILE* arq, No* no, AuxNo* auxNo) {
  No* novoNo = (No*) malloc(sizeof(No));
  int q = no->numChaves / 2;

  novoNo->numChaves = no->numChaves - q - 1;
  no->numChaves = q;
  auxNo->codigo = no->chave[q];
  auxNo->posDado = no->posDado[q];

  int i = 0;
  novoNo->filho[0] = no->filho[q + 1];

  for (i = 0; i < novoNo->numChaves; i++) {
    novoNo->posDado[i] = no->posDado[q + i + 1];
    novoNo->chave[i] = no->chave[q + i + 1];
    novoNo->filho[i + 1] = no->filho[q + i + 2];
  }

  int pos = insereNoArq(arq, novoNo);
  return pos;
}

// Adiciona um código na posição determinada de um nó
// Pré-condição: ponteiro para um nó, posição em que vai ser inserido, ponteiro para auxiliar de no e posição na árvore do ultimo filho anterior daquele nó
// Pós-condição: é inserido o codigo na posição mais a esquerda do nó
void adicionaDireita(No* no, int pos, AuxNo* auxNo, int ultimoFilho) {
  int i;
  for(i = no->numChaves; i > pos; i--) {
    no->posDado[i] = no->posDado[i-1];
    no->chave[i] = no->chave[i-1];
    no->filho[i+1] = no->filho[i];
  }

  no->posDado[pos] = auxNo->posDado;
  no->chave[pos] = auxNo->codigo;
  no->filho[pos+1] = ultimoFilho;
  no->numChaves++;
}

// Adiciona um código na esquerda de um nó
// Pré-condição: ponteiro para um nó e ponteiro par um auxiliar de nó
// Pós-condição: é inserido o codigo na posição mais a esquerda do nó
void adicionaNaEsquerda(No* no, AuxNo* auxNo) {
	int i;
  for (i = no->numChaves; i > 0; i--) {
    no->posDado[i] = no->posDado[i-1];
    no->chave[i] = no->chave[i-1];
    no->filho[i] = no->filho[i-1];
  }
  no->posDado[0] = auxNo->posDado;
  no->chave[0] = auxNo->codigo;
  no->filho[no->numChaves + 1] = no->filho[no->numChaves];
  no->numChaves++;
}

// Busca a posição que será inserido um código dentro de um nó
// Pré-condição: ponteiro para um nó e ponteiro par um auxiliar de nó
// Pós-condição: retorna a posição que deve ser inserido
int buscaPos(No* no, AuxNo* auxNo) {
	int i;
  for(i=0; i < no->numChaves; i++) {
    if(auxNo->codigo < no->chave[i]) {
      return i;
    }
  }
  return no->numChaves;
}

// Função usada para auxiliar na função insereCodigo
// Pré-condição: um ponteiro para arquivo, a posição que será inserido e um ponteiro para um auxiliar de no
// Pós-condição: é inserido o codigo na árvore
void insereCodigoAux(FILE* arq, int pos, AuxNo* auxNo) {
  No* no = leNoArquivo(arq, pos);
  int posAdd = buscaPos(no, auxNo);

  if (ehFolha(no)) {
    adicionaDireita(no, posAdd, auxNo, -1);
    escrevenoArq(arq, no, pos);
  }
	else {
    insereCodigoAux(arq, no->filho[posAdd], auxNo);
    No* filho = leNoArquivo(arq, no->filho[posAdd]);

    if (overflow(filho)) {
      AuxNo* novoAux = (AuxNo * ) malloc(sizeof(AuxNo));
      int posFilho = split(arq, filho, novoAux);

      escrevenoArq(arq, filho, no->filho[posAdd]);
      adicionaDireita(no, posAdd, novoAux, posFilho);
      escrevenoArq(arq, no, pos);
    }

  }
}

// Insere um código na árvore
// Pré-condição: um ponteiro para arquivo, a posição que será inserido e um ponteiro para um auxiliar de no
// Pós-condição: é inserido o codigo na árvore e retorna a psoição
int insereCodigo(FILE* arq, int pos, AuxNo* auxNo) {
  if (pos == -1) {
    //criar raiz
    No* no = criaNo();
    no->numChaves = 1;

    no->posDado[0] = auxNo->posDado;
    no->chave[0] = auxNo->codigo;
    int pos = insereNoArq(arq, no);
    
		return pos;
  } 
	else {
    insereCodigoAux(arq, pos, auxNo);
    No* no = leNoArquivo(arq, pos);

    if (overflow(no)) {
      AuxNo* novoAux = (AuxNo * ) malloc(sizeof(AuxNo));
      int x = split(arq, no, novoAux);

      No* novaRaiz = criaNo();
      novaRaiz->posDado[0] = novoAux->posDado;
      novaRaiz->chave[0] = novoAux->codigo;
      novaRaiz->filho[0] = pos;
      novaRaiz->filho[1] = x;
			int i;
      for(i = (((int) ORDEM / 2) + 1); i < ORDEM; i++) {
        no->filho[i] = -1;
      }

      escrevenoArq(arq, no, pos);
      novaRaiz->numChaves = 1;
      int novaPosRaiz = insereNoArq(arq, novaRaiz);

      return novaPosRaiz;
    }
    return pos;
  }
}

// Imprime os dados de todos os profissionais daquela árvore
// Pré-condição: um ponteiro para arquivo e a posição da raiz
// Pós-condição: nenhuma
void mostraProfissionais(FILE* arq, int pos) {
  if (pos == -1) {
    return;
  }
	int i;
  No* no = leNoArquivo(arq, pos);
  for(i = 0; i < no->numChaves; i++) {
    mostraProfissionais(arq, no->filho[i]);
    mostrarProfissionalPorPos(no->posDado[i]);
  }
  mostraProfissionais(arq, no->filho[no->numChaves]);
}

// Busca a posição de um código na sub-árvore
// Pré-condição: um ponteiro para arquivo, a posição da raiz e o código a ser buscado
// Pós-condição: retorna a posição no arquivo
int posDado(FILE* arq, int pos, int codigo) {
  if (pos == -1) {
    return -1;
  }
	int i;
  No* no = leNoArquivo(arq, pos);
  for (i = 0; i < no->numChaves; i++) {
    if (codigo < no->chave[i]) {
      return posDado(arq, no->filho[i], codigo);
    }
    if (codigo == no->chave[i]) {
      return no->posDado[i];
    }
  }
  return posDado(arq, no->filho[no->numChaves], codigo);
}

// Busca o maior código da sub-árvore
// Pré-condição: um ponteiro para arquivo e a posição da raiz
// Pós-condição: retorna o maior código naquela sub-árvore
AuxNo* maiorCodigo(FILE* arq, int pos) {
  No* no = leNoArquivo(arq, pos);
  if (ehFolha(no)) {
    AuxNo* auxNo = (AuxNo*) malloc(sizeof(AuxNo));
    auxNo->codigo = no->chave[no->numChaves - 1];
    auxNo->posDado = no->posDado[no->numChaves - 1];
    return auxNo;
  } 
	else {
    return maiorCodigo(arq, no->filho[no->numChaves]);
  }
}

// Remove um profissional da árvore
// Pré-condição: ponteiro para arquivo, posição do profissional na árvore, código do profissional a ser removido, posição do pai na árvore e um valor que determina se há mais nós a direita
// Pós-condição: será removido da árvore o código do profissional e a árvore será ajustada para que continue balanceada
int removeCodigo(FILE* arq, int pos, int codigo, int posPai, int desceDireita) {
  No* no = leNoArquivo(arq, pos);
	int i, j, k;
  if (ehFolha(no)) {
    if (desceDireita) {
      //tera que remover o utlimo
      no->posDado[no->numChaves - 1] = -1;
      no->chave[no->numChaves - 1] = -1;
      no->numChaves--;

      escrevenoArq(arq, no, pos);
      return faltaChave(no);
    }
		
    for (i = 0; i < no->numChaves; i++) {
      if (codigo == no->chave[i]) {
        removerProfissional(no->posDado[i]);
      }
      if (codigo <= no->chave[i]) {
        no->posDado[i] = no->posDado[i + 1];
        no->chave[i] = no->chave[i + 1];
      }
    }

    no->posDado[no->numChaves] = -1;
    no->chave[no->numChaves] = -1;
    no->numChaves = no->numChaves - 1;
    escrevenoArq(arq, no, pos);

    if (posPai == -1) {
      //eh raiz
      if (no->numChaves == 0) {
        //acabou a arvore
        return -1;
      }
      return pos;
    }
    return faltaChave(no);
  }

  int filhoEstaFaltando = -1;

  if (desceDireita) {
    filhoEstaFaltando = removeCodigo(arq, no->filho[no->numChaves], codigo, pos, 1);
  } 
	else {
    int i = 0;
    while (i < no->numChaves) {
      if (codigo < no->chave[i] || codigo == no->chave[i]) {
        break;
      }
      i++;
    }

    if (codigo == no->chave[i]) {
      //deletar no meio da arvore;
      removerProfissional(no->posDado[i]);

      AuxNo* auxNo = maiorCodigo(arq, no->filho[i]);
      no->chave[i] = auxNo->codigo;
      no->posDado[i] = auxNo->posDado;

      filhoEstaFaltando = removeCodigo(arq, no->filho[i], codigo, pos, 1);
    } 
		else {
      filhoEstaFaltando = removeCodigo(arq, no->filho[i], codigo, pos, 0);
    }
  }

  if (filhoEstaFaltando == 0) {
    //não precisa arrumar filho
    escrevenoArq(arq, no, pos);

    if (posPai == -1) {
      //eh raiz
      return pos;
    }
    return 0;
  }

  //tem que arrumar o filho
  int indexFilhoArrumar = -1;
  int indexChave = -1;

  for( j = 0; j < no->numChaves; j++) {
    No* filhoArrumarTest = leNoArquivo(arq, no->filho[j]);

    if (faltaChave(filhoArrumarTest)) {
      indexChave = j;
      indexFilhoArrumar = j;
    }
  }

  if (indexFilhoArrumar == -1) {
    indexFilhoArrumar = no->numChaves;
  }

  if (indexChave == -1) {
    indexChave = no->numChaves - 1;
  }

	//tenta emprestar da direita
  if (indexFilhoArrumar < no->numChaves) {
    No* noFilhoDirTest = leNoArquivo(arq, no->filho[indexFilhoArrumar + 1]);

    if (noFilhoDirTest->numChaves > (ORDEM / 2)) {
      No* noFilhoArrumar = leNoArquivo(arq, no->filho[indexFilhoArrumar]);

      noFilhoArrumar->chave[noFilhoArrumar->numChaves] = no->chave[indexFilhoArrumar];
      noFilhoArrumar->posDado[noFilhoArrumar->numChaves + 1] = no->posDado[indexFilhoArrumar];

      no->chave[indexFilhoArrumar] = noFilhoDirTest->chave[0];
      no->posDado[indexFilhoArrumar] = noFilhoDirTest->posDado[0];

      for(i = 0; i < noFilhoDirTest->numChaves; i++) {
        noFilhoDirTest->chave[i] = noFilhoDirTest->chave[i + 1];
        noFilhoDirTest->posDado[i] = noFilhoDirTest->posDado[i + 1];
      }

      noFilhoDirTest->chave[noFilhoDirTest -> numChaves] = -1;
      noFilhoDirTest->posDado[noFilhoDirTest -> numChaves] = -1;

      noFilhoArrumar->numChaves++;
      noFilhoDirTest->numChaves--;

      escrevenoArq(arq, no, pos);
      escrevenoArq(arq, noFilhoArrumar, no->filho[indexFilhoArrumar]);
      escrevenoArq(arq, noFilhoDirTest, no->filho[indexFilhoArrumar + 1]);

      if (posPai == -1) {
        return pos;
      }
      return 0;
    }
  }
	
	//tenta emprestar da esquerda
  if (indexFilhoArrumar != 0) {
    No* noFilhoEsqTest = leNoArquivo(arq, no -> filho[indexFilhoArrumar - 1]);

    if (noFilhoEsqTest->numChaves > (ORDEM / 2)) {
      No* noFilhoArrumar = leNoArquivo(arq, no->filho[indexFilhoArrumar]);

      for(i=0; i < noFilhoArrumar->numChaves; i++) {
        noFilhoArrumar->chave[i + 1] = noFilhoArrumar->chave[i];
        noFilhoArrumar->posDado[i + 1] = noFilhoArrumar->posDado[i];
      }
      
      noFilhoArrumar->chave[0] = no->chave[indexChave];
      noFilhoArrumar->posDado[0] = no->posDado[indexChave];

      no->chave[indexChave] = noFilhoEsqTest->chave[noFilhoEsqTest->numChaves - 1];
      no->posDado[indexChave] = noFilhoEsqTest->posDado[noFilhoEsqTest->numChaves - 1];

      noFilhoEsqTest->chave[noFilhoEsqTest -> numChaves - 1] = -1;
      noFilhoEsqTest->posDado[noFilhoEsqTest -> numChaves - 1] = -1;

      noFilhoEsqTest->numChaves--;
      noFilhoArrumar->numChaves++;

      escrevenoArq(arq, no, pos);
      escrevenoArq(arq, noFilhoArrumar, no->filho[indexFilhoArrumar]);
      escrevenoArq(arq, noFilhoEsqTest, no->filho[indexFilhoArrumar - 1]);

      if (posPai == -1) {
        return pos;
      }
      return 0;
    }
  }

	//terá que juntar dois nós
  int indexJunta = -1;
  if (indexFilhoArrumar == no->numChaves) {
    indexJunta = indexFilhoArrumar - 1;
  } else {
    indexJunta = indexFilhoArrumar + 1;
  }

  int posFilhoJunta = no->filho[indexJunta];
  No* filhoJunta = leNoArquivo(arq, posFilhoJunta);

  int posFilhoArrumar = no->filho[indexFilhoArrumar];
  No* noFilhoArrumar = leNoArquivo(arq, posFilhoArrumar);

  if (indexJunta < indexFilhoArrumar) {
    AuxNo* auxNo = (AuxNo* ) malloc(sizeof(AuxNo));

    auxNo->codigo = no->chave[indexChave];
    auxNo->posDado = no->posDado[indexChave];

    adicionaNaEsquerda(noFilhoArrumar, auxNo);
    for (k = filhoJunta->numChaves - 1; k >= 0; k--) {

      auxNo->codigo = filhoJunta->chave[k];
      auxNo->posDado = filhoJunta->posDado[k];

      adicionaNaEsquerda(noFilhoArrumar, auxNo);
    }

    for(k = 0; k < no->numChaves; k++) {
      if (k >= indexChave) {
        no->posDado[k] = no->posDado[k + 1];
        no->chave[k] = no->posDado[k + 1];
      }

      if (k > indexFilhoArrumar) {
        no->filho[k] = no->filho[k + 1];
      }
    }

    no->posDado[no->numChaves - 1] = -1;
    no->chave[no->numChaves - 1] = -1;
    no->filho[no->numChaves] = -1;
    no->numChaves--;

    escrevenoArq(arq, noFilhoArrumar, posFilhoArrumar);

    //exlui filho que foi juntado
    int quebrouAraiz = 0;

    if (posPai == -1 && no->numChaves == 0) {
      quebrouAraiz = 1;
      removeNoArq(arq, no, pos);
    }
    removeNoArq(arq, filhoJunta, posFilhoJunta);

    if (quebrouAraiz) {
      return posFilhoArrumar;
    } 
		else {
      return faltaChave(no);
    }
  } 
	else {
    //junta com o da direta
    //desce do pai pro filho
    noFilhoArrumar->posDado[noFilhoArrumar->numChaves] = no->posDado[indexChave];
    noFilhoArrumar->chave[noFilhoArrumar->numChaves] = no->chave[indexChave];
    noFilhoArrumar->numChaves++;

    //passa tudo do filho da direta pro da esq;
    for (k = 0; k < filhoJunta->numChaves; k++) {
      noFilhoArrumar->posDado[noFilhoArrumar->numChaves] = filhoJunta->posDado[k];
      noFilhoArrumar->chave[noFilhoArrumar->numChaves] = filhoJunta->chave[k];
      noFilhoArrumar->numChaves++;
    }

    for(k = 0; k < no -> numChaves; k++) {
      if (k >= indexChave) {
        no->posDado[k] = no->posDado[k + 1];
        no->chave[k] = no->posDado[k + 1];
      }
      if (k > indexFilhoArrumar) {
        no->filho[k] = no->filho[k + 1];
      }
    }

    no->posDado[no->numChaves-1] = -1;
    no->chave[no->numChaves-1] = -1;
    no->filho[no->numChaves] = -1;
    no->numChaves--;

    escrevenoArq(arq, noFilhoArrumar, posFilhoArrumar);
    int quebrouAraiz = 0;

    if (posPai == -1 && no->numChaves == 0) {
      quebrouAraiz = 1;
      removeNoArq(arq, no, pos);
    }
    removeNoArq(arq, filhoJunta, posFilhoJunta);
    if (quebrouAraiz) {
      return posFilhoArrumar;
    } 
		else {
      return faltaChave(no);
    }
  }
}

// Verifica a altura da árvore
// Pré-condição: ponteriro para o arquivo de indices e posição da raiz
// Pós-condição: retorna a altura da árvore
int alturaDaArvore(FILE* arq, int pos) {
  if (pos == -1) {
    return 0;
  }

  No* no = leNoArquivo(arq, pos);
  if (ehFolha(no)) {
    return 1;
  } 
	else{
    return 1 + alturaDaArvore(arq, no->filho[0]);
  }
}

// Imprime os códigos de um determinado nível da árvore
// Pré-condição: ponteriro para o arquivo de indices, posição da raiz, nível que se deseja imprimir e o nível atual para a recursão
// Pós-condição: será impresso os códigos do nível 
void mostraNivel(FILE* arq, int pos, int nivel, int nivelAtual) {
  if (pos == -1) {
    return;
  }
	
  No* no = leNoArquivo(arq, pos);
	int i;
  if(nivel == nivelAtual) {
    printf("[");
    for(i = 0; i < no->numChaves; i++) {
      printf("%d", no->chave[i]);
			if(i < no->numChaves-1)
				printf(", ");
    }
    printf("] ");
    return;
  }

  for(i=0; i <= no->numChaves; i++) {
    mostraNivel(arq, no->filho[i], nivel, nivelAtual + 1);
  }
}

// Imprime todos os códigos da árvore por nível
// Pré-condição: nenhuma
// Pós-condição: será impresso os códigos por nível
void arvorePorNivel() {
  FILE* arq = fopen(arquivoArvore, "rb+");
  Cab* cab = leCab(arq);
  int altura = alturaDaArvore(arq, cab -> raiz);
  printf("\nAltura da arvore eh %d\n", altura);
	int i;
  for(i = 1; i <= altura; i++) {
    mostraNivel(arq, cab->raiz, i, 1);
    printf("\n");
  }

  fclose(arq);
}
