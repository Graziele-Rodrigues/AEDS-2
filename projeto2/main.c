#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "menu.c"
#include "arvore.c"
#include "arqFuncionario.c"
#include "arq.c"
#include "funcionario.c"

int main() {
  iniciarArvore(); //arvore.bin
  iniciaArquivo(); //profissional.bin
  menuEntrada();

  return 0;
}