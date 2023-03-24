#include <stdlib.h>

#include "funcionario.c"
#include "encadeamento_exterior.c"
#include "compartimento_endereco.c"



#define TAM 5

void MENU() {
    printf("\nMENU\n");
    printf("1. Inserir funcionarios\n");
    printf("2. Buscar funcionarios\n");
    printf("3. Remover funcionarios\n");
    printf("4. Imprimir tabela\n");
    printf("0. Sair\n");
}


int main() {
    FILE *arq = fopen("funcionarios.dat", "w+b");
    FILE *arq_hash = fopen("hash.dat", "w+b");
    cria_hash("hash.dat", TAM);
    int opt;
    TFunc *func_teste = funcionario(-1, "AA", 0000, -1);

    do {
        MENU();
        printf("\nDigite uma opcao:\n");
        fflush(stdin);
        scanf("%d", &opt);
        switch (opt) {
            case 0:
                printf("\nSAINDO DO PROGRAMA\n");
                break;
            case 1:
                printf("\nINSERINDO 1000 FUNCIONARIOS SEQUENCIALMENTE\n");
                for (int i = 1; i <= 1000; ++i) {
                    char *nome[] = {"Maria", "Jose", "Carlos", "Ana", "Paulo", "Lucia"};
                    int num_names = sizeof(nome) / sizeof(nome[0]);
                    float salario;
                    int random_index = rand() % num_names;
                    salario = (float)rand() / RAND_MAX * (10000 - 1000) + 1000;
                    func_teste = funcionario(i, nome[random_index], salario, -1);
                    insere(func_teste, arq_hash, "funcionarios.dat", TAM);
                }
                break;
            case 2:
                printf("\nPESQUISANDO FUNCIONARIO\n");
                printf("Digite o codigo do funcionario a ser pesquisado:\n");
                fflush(stdin);
                scanf("%d", &codigo);

                TFunc *busc = busca(codigo, arq_hash, "funcionarios.dat", TAM);

                if(busc->cod != -1) {
                    imprime(busc);
                } else {
                    printf("Funcionario NAO esta na tabela.\n");
                }
                break;
            case 3:
                printf("\nREMOVENDO FUNCIONARIO\n");
                printf("Digite o codigo do funcionario a ser pesquisado:\n");
                fflush(stdin);
                scanf("%d", &codigo);

                int r = exclui(codigo, arq_hash, "funcionarios.dat", TAM);

                if(r == 1) {
                    printf("Funcionario removido com sucesso.\n");
                } else {
                    printf("Funcionario nao foi removido.\n");
                }
                break;
            case 4:
                //le o arquivo e coloca no vetor
                rewind(arq_hash); //posiciona cursor no inicio do arquivo
                CompartimentoHash *c = le_compartimento(arq_hash);
                int x = 0;
                while (!feof(arq_hash))
                {
                    printf("\nCompartimento %d\n", x);
                    if (c->prox == -1) {
                        c = le_compartimento(arq_hash);
                        x++;
                        continue;
                    }
                    fseek(arq, tamanho_registro() * (c->prox), SEEK_SET);
                    func_teste = le(arq);
                    imprime(func_teste);
                    while(func_teste->prox != -1) {
                        fseek(arq, tamanho_registro() * (func_teste->prox), SEEK_SET);
                        func_teste = le(arq);
                        imprime(func_teste);
                    }
                    c = le_compartimento(arq_hash);
                    x++;
                }
                break;
            default:
                printf("Digite uma opcao valida\n");
        }
    }while(opt != 0);

    free(func_teste);
    fclose(arq);
    fclose(arq_hash);
    return 0;
}
