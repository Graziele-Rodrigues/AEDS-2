#include <stdio.h>
#include <stdlib.h>
//#include "arquivo.h"
#include "arquivo.c"


int main()
{
    //le_arquivo_caracteres("dados.txt");
    //le_arquivo_strings("dados.txt");
    //le_arquivo_strings_numeros("numeros.txt");
    grava_arquivo_strings("gravado.txt", 1, 10, 1);

    return 0;
}
