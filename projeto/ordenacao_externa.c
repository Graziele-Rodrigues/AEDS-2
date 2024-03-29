#include "include/ordenacao_externa.h"
#include "include/lista.h"
#include "include/funcionario.h"
#include <stdio.h>
#include <limits.h>
#include <time.h>

typedef struct vetor{
    TFunc *func;
    FILE *f;
    int aux_p;
}TVet;

void selecao_com_substituicao(char * nome_arquivo_entrada, Lista * nome_arquivos_saida, int M) {
    int comparacoes = 0;
    FILE * arq; //pointer to input file
    int fim = 0;

    clock_t start, end;
    double time_spent = 0.0;
    start = clock();

    //open input file for reading
    if ((arq = fopen(nome_arquivo_entrada, "rb")) == NULL) {
        printf("Error opening input file\n");
    } else {
        int i = 0;
        //Ler primeiro funcionario
        TFunc *cin = le(arq);

        // Vetor para guardar os funcionarios do arquivo
        TFunc *v[M];
        int frozens[M];
        for (i = 0; i < M; i++) {
            frozens[i] = 0;
        }

        //Ler M registros do arquivo para a memória
        i = 0;
        while (!feof(arq) && i < M) {
            v[i] = cin;
            cin = le(arq);
            i++;
        }

        //adjust M if input file ended before filling the array v
        if (i != M) {
            M = i;
        }

        int n_frozen = 0;
        TFunc *menor;
        //open output partition for writing
        while (!fim) {
            char *nome_particao = nome_arquivos_saida->nome;
            nome_arquivos_saida = nome_arquivos_saida->prox;
            FILE *p;
                if ((p = fopen(nome_particao, "wb")) == NULL) {
                    printf("Error creating output partition\n");
                } else {
                while (M - n_frozen > 0) {
                    i = 0;
                    int ind_min = 0;
                    // Tratar exececao de encontrar algum funcionario NULL ou Funcinoario congelado nas primeiras posicoes
                    while (v[i] == NULL || frozens[i] == 1) {
                        ind_min++;
                        i++;
                    }
                    // Encontrar funcionario com mento codigo
                    for (int j = 0; j < M; j++) {
                        if(v[j] == NULL) {
                            continue;
                        }
                        if (v[j]->cod < v[ind_min]->cod && frozens[j] == 0) {
                            comparacoes++;
                            ind_min = j;
                        }
                    }
                    menor = v[ind_min];

                    //imprime(menor);
                    salva(menor, p);

                    // Substituir funcionario no vetor caso estamos no final do arquivo este sera nulo
                    if (cin != NULL) {
                        v[ind_min] = cin;
                        cin = le(arq);
                    } else {
                        v[ind_min] = NULL;
                        n_frozen++;
                    }

                    // Congelar funcionario
                    if (v[ind_min] != NULL) {
                        if (v[ind_min]->cod < menor->cod) {
                            frozens[ind_min] = 1;
                            n_frozen++;
                        }
                        continue;
                    }
                }
            }

            // Fechar particao
            fclose(p);
            // Descongelar
            for (i = 0; i < M; i++) {
                if(v[i] == NULL)
                    continue;
                frozens[i] = 0;
                n_frozen--;
            }

            // Se estiver no final do arquivo e tudo resolvido
            if (feof(arq) && n_frozen == M) {
                fim = 1;
            }
        }
    }
    // Calculando tempo e comparacoes
    end = clock();
    time_spent += (double)(end - start) / CLOCKS_PER_SEC;

    FILE *out = fopen("saida_selecao.txt", "w");
    fprintf(out, "Numero comparacoes: %d\n", comparacoes);
    fprintf(out, "Tempo gasto: %f", time_spent);
    fclose(out);

    printf("Comparacoes: %d\n", comparacoes);
    printf("Tempo de execucao: %f", time_spent);
}

void intercalacao_basico(char *nome_arquivo_saida, int num_p, Lista *nome_particoes) {

    int fim = 0; //variavel que controla fim do procedimento
    FILE *out; //declara ponteiro para arquivo

    //abre arquivo de saida para escrita
    if ((out = fopen(nome_arquivo_saida, "wb")) == NULL) {
        printf("Erro ao abrir arquivo de sa?da\n");
    } else {
        //cria vetor de particoes
        TVet v[num_p];

        //abre arquivos das particoes, colocando variavel de arquivo no campo f do vetor
        //e primeiro funcionario do arquivo no campo func do vetor
        for (int i=0; i < num_p; i++) {
            v[i].f = fopen(nome_particoes->nome, "rb");
            v[i].aux_p = 0;
            if (v[i].f != NULL) {
                fseek(v[i].f, v[i].aux_p * tamanho_registro(), SEEK_SET);
                TFunc *f = le(v[i].f);
                if (f == NULL) {
                    //arquivo estava vazio
                    //coloca HIGH VALUE nessa posi??o do vetor
                    v[i].func = funcionario(INT_MAX, "",0);
                }
                else {
                    //conseguiu ler funcionario, coloca na posi??o atual do vetor
                    v[i].func = f;
                }
            }
            else {
                fim = 1;
            }
            nome_particoes = nome_particoes->prox;
        }

        int aux = 0;
        while (!(fim)) { //conseguiu abrir todos os arquivos
            int menor = INT_MAX;
            int pos_menor;
            //encontra o funcionario com menor chave no vetor
            for(int i = 0; i < num_p; i++){
                if(v[i].func->cod < menor){
                    menor = v[i].func->cod;
                    pos_menor = i;
                }
            }
            if (menor == INT_MAX) {
                fim = 1; //terminou processamento
            }
            else {
                //salva funcionario no arquivo de saÃ¬da
                fseek(out, aux * tamanho_registro(), SEEK_SET);
                salva(v[pos_menor].func, out);
                //atualiza posiÃ§Ã£o pos_menor do vetor com pr?ximo funcionario do arquivo
                v[pos_menor].aux_p++;
                fseek(v[pos_menor].f, v[pos_menor].aux_p * tamanho_registro(), SEEK_SET);
                TFunc *f = le(v[pos_menor].f);
                aux++;
                if (f == NULL) {
                    //arquivo estava vazio
                    //coloca HIGH VALUE nessa posiÃ§ao do vetor
                    v[pos_menor].func = funcionario(INT_MAX, "", 0.0);
                }
                else {
                    v[pos_menor].func = f;
                }

            }
        }

        //fecha arquivos das partiÃ‡Ãµes de entrada
        for(int i = 0; i < num_p; i++){
            fclose(v[i].f);
            //    free(v[i].func);
        }
        //fecha arquivo de saÃ¬da
        printf("Apos ordenacao:\n");
        fclose(out);
    }
}

void intercalacao_otima(char *nome_arquivo_saida, int num_p, Lista *nome_particoes, int f) {
    int n = num_p;
    int i = num_p;
    char nome_arquivo[50];
    clock_t start, end;
    double tempo_gasto = 0.0;
    start = clock();
    Lista *temp = NULL;
    // retirar o primeiro nome da lista
    while(i > f-1) {
        Lista *aux = cria_nomes(cria_str("exception"), NULL);
        for(int j = 0; j < f-1; j++) {
            temp = nome_particoes->prox;
            adiciona_nomes(aux, nome_particoes->nome);
            free(nome_particoes->nome);
            free(nome_particoes);
            nome_particoes = temp;
            i--;
        }
        temp = aux->prox;
        free(aux->nome);
        free(aux);
        aux = temp;
        n++;
        sprintf(nome_arquivo, "p%d.dat", n);
        intercalacao_basico(nome_arquivo, f-1, aux);
        adiciona_nomes(nome_particoes, nome_arquivo);
        libera_nomes(aux);
        i++;
    }
    // Ordenação final
    intercalacao_basico(nome_arquivo_saida, i, nome_particoes);

    // Tempo de execucao em arquivo
    end = clock();
    tempo_gasto += (double)(end - start) / CLOCKS_PER_SEC;
    FILE *out = fopen("saida_intercalacao.txt", "w");
    fprintf(out, "Tempo gasto intercalacao: %f", tempo_gasto);
    fclose(out);
    printf("\nTempo gasto na execucao na intercalacao: %f segundos\n", tempo_gasto);
    // Apagar as particoes
}