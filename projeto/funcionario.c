#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "include/funcionario.h"

// Imprime funcionario
void imprime(TFunc *func) {
    printf("**********************************************");
    printf("\nFuncionario de codigo: ");
    printf("%d", func->cod);
    printf("\nNome: ");
    printf("%s", func->nome);
    printf("\nSalario: ");
    printf("%4.2f", func->salario);
    printf("\n**********************************************");
}

//Cria funcionario
TFunc *funcionario(int cod, char *nome, double salario) {
    TFunc *func = (TFunc *) malloc(sizeof(TFunc));
    //inicializa espaÃƒÂ§o de memÃƒÂ³ria com ZEROS
    if (func) memset(func, 0, sizeof(TFunc));
    //copia valores para os campos de func
    func->cod = cod;
    strcpy(func->nome, nome);
    func->salario = salario;
    return func;
}

//Salva funcionario no arquivo out, na posicao atual do cursor
void salva(TFunc *func, FILE *out) {
    fwrite(&func->cod, sizeof(int), 1, out);
    //func->nome ao invasao de &func->nome, pois string 
    fwrite(func->nome, sizeof(char), sizeof(func->nome), out);
    fwrite(&func->salario, sizeof(double), 1, out);
}


/*
 * Le um funcionario do arquivo in na posicao atual do cursor
 * Retorna um ponteiro para funcionario lido do arquivo
 */
TFunc *le(FILE *in) {
    TFunc *func = (TFunc *) malloc(sizeof(TFunc));
    if (0 >= fread(&func->cod, sizeof(int), 1, in)) {
        free(func);
        return NULL;
    }
    fread(func->nome, sizeof(char), sizeof(func->nome), in);
    fread(&func->salario, sizeof(double), 1, in);
    return func;
}



void le_funcionarios(FILE *in) {
    printf("\n\nLendo funcionarios do arquivo...\n\n");
    rewind(in);
    TFunc *f;
    while ((f = le(in)) != NULL) {
        imprime(f);
        free(f);
    }
}

void imprime_arquivo(FILE *arq) {
    //le o arquivo e coloca no vetor
    rewind(arq); //posiciona cursor no inicio do arquivo
    TFunc *f = le(arq);
    int i = 0;
    while (!feof(arq)) {
        imprime(f);
        f = le(arq);
    }
}


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

void insere_funcionarios(FILE *out) {
    printf("Inserindo funcionarios no arquivo...");
    // Gerar nomes e salários aleatórios
    char *nome[] = {"Maria", "Jose", "Carlos", "Ana", "Paulo", "Lucia"};
    int num_names = sizeof(nome) / sizeof(nome[0]);
    double salario;
    generate_unique_id();
    for (int i = 0; i < NUM_EMPLOYEES; i++) {
        int random_index = rand() % num_names;
        salario = (double)rand() / RAND_MAX * (10000 - 1000) + 1000;
        TFunc *f = funcionario(id[i], nome[random_index], salario);
        salva(f, out);
        free(f);
    }
}

/* ############################ BUSCA SEQUENCIAL ############################ */
/**
 * Na busca sequencial, você precisa comparar o ID procurado com o ID de cada 
 * funcionário, até encontrar o funcionário ou chegar ao final da lista. 
 * Neste caso, o número de comparações é linear em relação ao número de funcionários.
**/
TFunc *busca_sequencial(FILE *out, int chave){
    rewind(out);
    comparisons=0;
    TFunc *func = (TFunc *) malloc(sizeof(TFunc));
    int encontrado=0;
    start = clock();
    //o arquivo é percorrido usando a função fread() e, para cada registro lido, é verificado se o ID corresponde ao ID procurado.
    for(int i=0;i<NUM_EMPLOYEES; i++){
        fread(func, tamanho_registro(), 1, out);
        comparisons++;
        if(func->cod==chave){
            encontrado = 1;
            break;
        }
    }
    end = clock();
    if(encontrado){
        return func;
    }
    else{
        return NULL;
    }
}

void busca_funcionarios_sequencial(FILE *out){
    FILE *busca;
    busca = fopen("busca.dat", "w+b");
    TFunc *f;
    int chave=0;
    for(int i=0; i<25;i++){
        chave = rand() % NUM_EMPLOYEES;
        f = busca_sequencial(out, chave);
        if(f!= NULL){
            imprime(f);
            salva(f, busca);  
            FILE *fp = fopen("busca.dat", "a"); //pra salvar no fim do arquivo
            fprintf(fp, "\nQuantidade de comparacoes em funcionario cod %d: %d\n",f->cod, comparisons);
            fprintf(fp, "Tempo gasto em funcionario cod %d: %.2f segundos\n", f->cod, (double)(end - start) / CLOCKS_PER_SEC);      
        }
        else{
            printf("%d, \nnao encontrado\n", chave);
        }
        free(f);
    }
    imprime_arquivo(busca);
    fclose(busca);
}

/*#################################### INSERTION SORT DISCO ####################################*/

void insertion_sort_disco(FILE *arq, int tam) {
    FILE *fp;
    fp = fopen("resultados_ordenacao.txt", "w");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }
    int i;
    int comparacoes = 0;
    clock_t tempo_inicial = clock();
    //faz o insertion sort
    for (int j = 2; j <= tam; j++) {
        comparacoes++;
        //posiciona o arquivo no registro j
        fseek(arq, (j-1) * tamanho_registro(), SEEK_SET);
        TFunc *fj = le(arq);
        i = j - 1;
        //posiciona o cursor no registro i
        fseek(arq, (i-1) * tamanho_registro(), SEEK_SET);
        TFunc *fi = le(arq);
        while ((i > 0) && (fi->cod > fj->cod)) {
            //posiciona o cursor no registro i+1
            fseek(arq, i * tamanho_registro(), SEEK_SET);
            salva(fi, arq);
            i = i - 1;
            //le registro i
            fseek(arq, (i-1) * tamanho_registro(), SEEK_SET);
            fi = le(arq);
        }
        //posiciona cursor no registro i + 1
        fseek(arq, (i) * tamanho_registro(), SEEK_SET);
        //salva registro j na posicao i
        salva(fj, arq);
        clock_t tempo_final = clock();
        float tempo_gasto = (float)(tempo_final - tempo_inicial) / CLOCKS_PER_SEC;
        fprintf(fp, "Numero de comparacoes: %d\n", comparacoes);
        fprintf(fp, "Tempo gasto: %f segundos\n", tempo_gasto);
    }
    fclose(fp);
    //descarrega o buffer para ter certeza que dados foram gravados
    fflush(arq);
}

/*#################################### PARTIÇÃO POR SUBSTITUIÇÃO ####################################*/


/**
 * Diferença codigo ordenacao por substituicao e classificacao interna
 * O código classificacao_interna grava os funcionários ordenados em arquivos distintos, enquanto o código substituição grava os funcionários ordenados de volta no mesmo arquivo de entrada.
 * O código "ordenacao_por_substituicao" realiza a ordenação de todos os registros do arquivo de uma só vez. Ele lê todos os registros de funcionários, coloca em um vetor, realiza a ordenação no vetor e escreve o resultado ordenado no arquivo.
 * Já o código "classificacao_interna" divide o arquivo de registros de funcionários em pedaços (chamados de partições) e realiza a ordenação de cada partição separadamente. Cada partição é lida, ordenada e gravada em um arquivo separado. Ao final, todos os arquivos de partições serão mesclados em um único arquivo ordenado.
*/
void ordenacao_por_substituicao(FILE *arquivo, int nFunc) {
    comparisons=0;
    FILE *arq;
    arq = fopen("resultados_substituicao.txt", "w");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }
    rewind(arquivo);
    int reg = 0;
    clock_t tempo_inicial = clock();
    while (reg != nFunc) {
        // Lê o arquivo e coloca no vetor
        TFunc *vetor[NUM_EMPLOYEES];
        int i = 0;
        while (!feof(arquivo)) {
            fseek(arquivo, reg * tamanho_registro(), SEEK_SET);
            vetor[i] = le(arquivo);
            i++;
            reg++;
            if (i >= NUM_EMPLOYEES) break;
        }

        // Faz a ordenação
        for (int j = 1; j < i; j++) {
            comparisons++;
            TFunc *f = vetor[j];
            int k = j - 1;
            while (k >= 0 && vetor[k]->cod > f->cod) {
                vetor[k + 1] = vetor[k];
                k--;
            }
            vetor[k + 1] = f;
        }

        // Grava os funcionários ordenados no arquivo
        rewind(arquivo);
        for (int j = 0; j < i; j++) {
            fseek(arquivo, j * tamanho_registro(), SEEK_SET);
            salva(vetor[j], arquivo);
        }

        // Libera a memória alocada para o vetor
        for (int j = 0; j < i; j++) {
            free(vetor[j]);
        }
        
        clock_t tempo_final = clock();
        float tempo_gasto = (float)(tempo_final - tempo_inicial) / CLOCKS_PER_SEC;
        printf("Numero de comparacoes: %d\n", comparisons);
        printf("Tempo gasto: %f segundos\n", tempo_gasto);
        fprintf(arq, "Numero de comparacoes: %d\n", comparisons);
        fprintf(arq, "Tempo gasto: %f segundos\n", tempo_gasto);
    }
    fclose(arq);
    printf("ARQUIVO APOS ORDENADO...\n");
    imprime_arquivo(arquivo);
}
           

/*#################################### BUSCA BINARIA ####################################*/

/***
 * A busca binária divide o vetor de funcionários em duas metades em cada iteração, 
 * então o número de comparações necessárias para encontrar o funcionário é logarítmico em relação ao número de funcionários 
 * na base de dados.
 * ***/

TFunc *busca_binaria(int chave, FILE *in, int inicio, int fim) {
    comparisons=0;
    TFunc *f = NULL;
    int cod = -1;
    start = clock();
    while (inicio <= fim && cod != chave) {
        comparisons++;
        int meio = trunc((inicio + fim) / 2);
        fseek(in, (meio -1) * tamanho_registro(), SEEK_SET);
        f = le(in);
        cod = f->cod;
        if (f) {
            if (cod > chave) {
                fim = meio - 1;
            } else {
                inicio = meio + 1;
            }
        }
    }
    if (cod == chave) {
        end = clock();
        return f;
    }
    else return NULL;
}

void busca_funcionarios_binario(FILE *out){
    FILE *busca;
    busca = fopen("busca.dat", "w+b");
    TFunc *f;
    int chave=0;
    for(int i=0; i<25;i++){
        chave = rand() % NUM_EMPLOYEES;
        f = busca_binaria(chave, out, 0, tamanho_arquivo(out)-1);
        if(f!= NULL){
            imprime(f);
            salva(f, busca); 
            FILE *fp = fopen("busca.dat", "a"); //pra salvar no fim do arquivo
            fprintf(fp, "\nQuantidade de comparacoes no funcionario com id %d: %d\n", f->cod, comparisons);
            fprintf(fp, "Tempo gasto no funcionario com id %d: %.2f segundos\n",f->cod, (double)(end - start) / CLOCKS_PER_SEC);       }
        else{
            printf("%d, nao encontrado\n", chave);
        }
        free(f);
    }
    imprime_arquivo(busca);
    fclose(busca);
}

// Retorna tamanho do funcionario em bytes
int tamanho_registro() {
    return sizeof(int)  //cod
           + sizeof(char) * 50 //nome
           + sizeof(double); //salario
}


int tamanho_arquivo(FILE *arq) {
    fseek(arq, 0, SEEK_END);
    int tam = trunc(ftell(arq) / tamanho_registro());
    return tam;
}
