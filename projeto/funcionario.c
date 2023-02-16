#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "include/funcionario.h"
#include "include/lista.h"

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
    fp = fopen("resultados_insertion_sort.txt", "w");
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

/**
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
}*/


void ordenacao_por_substituicao(char * nome_arquivo_entrada, Lista * nome_arquivos_saida, int M) {
    FILE *fp;
    fp = fopen("resultados_particoes.txt", "w");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }
    FILE * arq;  //ponteiro para abrir arquivo
    int fim = 0;

    int comparacoes = 0;
    clock_t tempo_inicial = clock();
    //abre arquivo
    if ((arq = fopen(nome_arquivo_entrada, "rb")) == NULL) {
        printf("Error opening input file\n");
    } else {
        int i = 0;
        //Ler primeiro funcionario
        TFunc *func = le(arq);

        TFunc *v[M]; //vetor que vai guardar funcionarios em cada particao
        int frozens[M]; //descongelar
        for (i = 0; i < M; i++) {
            frozens[i] = 0;
        }

        //Ler M registros do arquivo para a memória
        i = 0;
        while (!feof(arq) && i < M) {
            v[i] = func;
            func = le(arq);
            i++;
            comparacoes++;
        }

        //ajusta M se o arquivo de entrada terminou antes de preencher o array v
        if (i != M) {
            M = i;
            comparacoes++;
        }

        int n_frozen = 0;
        TFunc *menor;
        // Loop enquanto ainda tem registro descongelados
        // Precisa de um loop aqui para abrir nova partição
        // Abre partição para escrever
        while (!fim) {
            char *nome_particao = nome_arquivos_saida->nome;
            nome_arquivos_saida = nome_arquivos_saida->prox;
            FILE *p;
            printf("\n%s\n", nome_particao);
            //int n = 0;

                if ((p = fopen(nome_particao, "wb")) == NULL) {
                    printf("Error creating output partition\n");
                } else {
                while (M - n_frozen > 0) {
                    //find the record with the smallest key
                    i = 0;
                    int ind_min = 0; //indice minimo
                    while (v[i] == NULL || frozens[i] == 1) {
                        ind_min++; //incrementa indice minimo
                        i++;
                    }
                    for (int j = 0; j < M; j++) {
                        if(v[j] == NULL) {
                            continue;
                        }
                        if (v[j]->cod < v[ind_min]->cod && frozens[j] == 0) {
                            ind_min = j;
                        }
                        comparacoes++;
                    }
                    menor = v[ind_min];

                    imprime(menor);
                    //fseek(p, (n) * tamanho_registro(), SEEK_SET);
                    salva(menor, p);
                    //n++;

                    if (func != NULL) {
                        v[ind_min] = func;
                        func = le(arq);
                    } else {
                        v[ind_min] = NULL;
                        n_frozen++;
                    }

                    //replace the record with the smallest key with the next record from input file
                    if (v[ind_min] != NULL) {
                        if (v[ind_min]->cod < menor->cod) {
                            frozens[ind_min] = 1;
                            n_frozen++;
                        }
                        continue;
                    }
                }
            }
            //close output partition
            fclose(p);
            //unfreeze frozen records
            for (i = 0; i < M; i++) {
                if(v[i] == NULL)
                    continue;
                frozens[i] = 0;
                n_frozen--;
            }

            if (feof(arq) && n_frozen == M) {
                fim = 1;
            }

            clock_t tempo_final = clock();
            float tempo_gasto = (float)(tempo_final - tempo_inicial) / CLOCKS_PER_SEC;
            fprintf(fp, "Numero de comparacoes: %d\n", comparacoes);
            fprintf(fp, "Tempo gasto: %f segundos\n", tempo_gasto);
        }
    }
}

/*#################################### INTERCALAÇÃO ÓTIMA ####################################*/

/***
 * A intercalação ótima é uma técnica de ordenação de arquivos externa que combina duas ou mais partições ordenadas de tamanho relativamente pequeno em uma única partição ordenada. 
 * 1 - Inicialize as variáveis: Inicialize variáveis como o número de arquivos abertos, o índice do arquivo atual e o registro lido de cada arquivo.
 * 2 - Encontrar o registro com a menor chave: Encontre o registro com a menor chave entre todos os registros disponíveis na memória.
 * 3 - Salvar o registro: Salve o registro com a menor chave no arquivo de saída.
 * 4 - Lê o próximo registro: Leia o próximo registro do arquivo que forneceu o registro com a menor chave.
 * 5 - Repita os passos 3 a 5 até que todos os arquivos de entrada tenham sido lidos.
 * 6 - Fechar o arquivo de saída: Quando todos os arquivos de entrada tiverem sido lidos e mesclados, feche o arquivo de saída.
*/    


void intercalacao_otima(char * nome_arquivo_entrada1, char * nome_arquivo_entrada2, char * nome_arquivo_entrada3, char * nome_arquivo_saida) {
    FILE * arq1;
    FILE * arq2;
    FILE * arq3;
    FILE * arq_saida;

    FILE *fp;
    fp = fopen("resultados_intercalacao.txt", "w");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    //abre os arquivos de entrada
    if ((arq1 = fopen(nome_arquivo_entrada1, "r+b")) == NULL) {
        printf("Erro ao abrir arq1\n");
    }
    if ((arq2 = fopen(nome_arquivo_entrada2, "r+b")) == NULL) {
        printf("Erro ao abrir arq2\n");
    }
    if ((arq3 = fopen(nome_arquivo_entrada3, "r+b")) == NULL) {
        printf("Erro ao abrir arq3\n");
    }

    //abre o arquivo de saida
    if ((arq_saida = fopen(nome_arquivo_saida, "w+b")) == NULL) {
        printf("Erro ao abrir arquivo saida\n");
    }

    //le o primeiro funcionario de cada arquivo de entrada
    TFunc *func1 = le(arq1);
    TFunc *func2 = le(arq2);
    TFunc *func3 = le(arq3);

    TFunc *menor;

    int comparacoes = 0;
    clock_t tempo_inicial = clock();

    //loop para escrever no arquivo de saida
    while (func1 != NULL || func2 != NULL || func3 != NULL) {
        //encontra o funcionario com a menor chave
        if (func1 != NULL && func2 != NULL && func3 != NULL) {
            if (func1->cod <= func2->cod && func1->cod <= func3->cod) {
                menor = func1;
                func1 = le(arq1);
                comparacoes++;
            } else if (func2->cod <= func1->cod && func2->cod <= func3->cod) {
                menor = func2;
                func2 = le(arq2);
                comparacoes++;
            } else {
                menor = func3;
                func3 = le(arq3);
                comparacoes++;
            }
        } else if (func1 != NULL && func2 != NULL) {
            if (func1->cod <= func2->cod) {
                menor = func1;
                func1 = le(arq1);
                comparacoes++;

            } else {
                menor = func2;
                func2 = le(arq2);
                comparacoes++;
            }
        } else if (func1 != NULL && func3 != NULL) {
            if (func1->cod <= func3->cod) {
                menor = func1;
                func1 = le(arq1);
                comparacoes++;

            } else {
                menor = func3;
                func3 = le(arq3);
                comparacoes++;
            }
        } else if (func2 != NULL && (func1 == NULL || func2->cod < func1->cod)) {
            menor = func2;
            func2 = le(arq2);
            comparacoes++;
        } else {
            menor = func1;
            func1 = le(arq1);
            comparacoes++;
        }
        imprime(menor);
        salva(menor, arq_saida);
        clock_t tempo_final = clock();
        float tempo_gasto = (float)(tempo_final - tempo_inicial) / CLOCKS_PER_SEC;
        fprintf(fp, "Numero de comparacoes: %d\n", comparacoes);
        fprintf(fp, "Tempo gasto: %f segundos\n", tempo_gasto);
        }
        // fecha arquivo de saída
        fclose(arq_saida);
         // fecha arquivos de entrada
        fclose(arq1);
        fclose(arq2);
        fclose(arq3);
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
