#ifndef ORDENACAO_EXTERNA_H_INCLUDED
#define ORDENACAO_EXTERNA_H_INCLUDED

#include "lista.h"

#include <stdio.h>
#include "funcionario.h"

// Executa o algoritmo de geracao de particoes por Selecao com Substituicao
// nome_arquivo_entrada: nome do arquivo de entrada
// nome_arquivo_saida: lista encadeada com nomes de arquivos de saida
// M: tamanho do array em memoria para manipulacao dos registros
void selecao_com_substituicao(char *nome_arquivo_entrada, Lista *nome_arquivos_saida, int M);
void intercalacao_basico(char *nome_arquivo_saida, int num_p, Lista *nome_particoes);
void intercalacao_otima(char *nome_arquivo_saida, int num_p, Lista *nome_particoes, int f);

#endif //PRATICAI_INTERCALACAO_H
