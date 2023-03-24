#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "encadeamento_exterior.h"
#include "compartimento_endereco.h"
#include "funcionario.h"

int hash(int cod, int tam) {
    return cod % tam;
}

void cria_hash(char *nome_arquivo_hash, int tam) {
    FILE *arq;
    CompartimentoHash *Compartimento = (CompartimentoHash *) malloc(sizeof(CompartimentoHash));
    if ((arq = fopen(nome_arquivo_hash, "wb")) == NULL) {
        printf("Erro ao abrir arquivo!!");
    } else {
        for (int i = 0; i < tam; i++) {
            Compartimento = compartimento_hash(-1);
            salva_compartimento(Compartimento, arq);
        }
    }
    fclose(arq);
}

int insere(TFunc *func, FILE *arq_hash, char *nome_arquivo_dados, int tam) {
    int h = hash(func->cod, tam);
    if(h == 0) {
        rewind(arq_hash);
    } else {
        fseek(arq_hash, tamanho_compartimento() * (h), SEEK_SET);
    }
    CompartimentoHash *comp = le_compartimento(arq_hash);
    // Compartimento vazio
    if(comp->prox == -1) {
        FILE *arq_dado = fopen(nome_arquivo_dados, "a+b");
        if (arq_dado == NULL) {
            printf("Erro ao abrir o arquivo");
            exit(1);
        }
        // Salva o funcionario no arquivo de dados
        salva(func, arq_dado);
        // Atualiza o compatimento com a localização do funcionario no arquivo de dados
        comp->prox = tamanho_arquivo(arq_dado) - 1;
        fseek(arq_hash, tamanho_compartimento() * (h), SEEK_SET);
        salva_compartimento(comp, arq_hash);
        fclose(arq_dado);
        free(comp);
        return 1;
    } else { // Compartimento já tem conteúdo
        FILE *arq_dado = fopen(nome_arquivo_dados, "r+b");
        if (arq_dado == NULL) {
            printf("Erro ao abrir o arquivo");
            exit(1);
        }
        if(comp->prox == 0) {
            rewind(arq_dado);
        } else {
            fseek(arq_dado, tamanho_registro() * comp->prox, SEEK_SET);
        }
        // Variavel para atualizar o prox do funcionario
        int pos = comp->prox;
        while (1) {
            TFunc *f = le(arq_dado);
            if (f->prox == -1) {
                fseek(arq_dado, 0, SEEK_END);
                salva(func, arq_dado);
                f->prox = tamanho_arquivo(arq_dado) - 1;
                if (pos == 0) {
                    rewind(arq_dado);
                } else {
                    fseek(arq_dado, tamanho_registro() * pos, SEEK_SET);
                }
                salva(f, arq_dado);
                free(f);
                fclose(arq_dado);
                return 1;
            } else {
                fseek(arq_dado, tamanho_registro() * f->prox, SEEK_SET);
                pos = f->prox;
            }
        }
    }
}

TFunc *busca(int cod, FILE *arq_hash, char *nome_arquivo_dados, int tam){
    TFunc *f_aux = funcionario(-1, "AA", 0000, -1);
    if(cod < 0) {
        return f_aux;
    }
    int h = hash(cod, tam);
    if(h == 0) {
        rewind(arq_hash);
    } else {
        fseek(arq_hash, tamanho_compartimento() * (h), SEEK_SET);
    }
    CompartimentoHash *comp = le_compartimento(arq_hash);
    if(comp->prox == -1) {
        return f_aux;
    } else {
        FILE *arq_dado = fopen(nome_arquivo_dados, "r+b");
        if (arq_dado == NULL) {
            printf("Erro ao abrir o arquivo!\n");
            exit(1);
        }
        if(comp->prox == 0) {
            rewind(arq_dado);
        } else {
            fseek(arq_dado, tamanho_registro() * comp->prox, SEEK_SET);
        }
        while (1) {
            TFunc *f = le(arq_dado);
            if (f->prox == -1 && f->cod != cod) {
                return f_aux;
            }
            if (f->cod == cod) {
                fclose(arq_dado);
                return f;
            } else {
                fseek(arq_dado, tamanho_registro() * f->prox, SEEK_SET);
            }
        }
    }
}

int exclui(int cod, FILE *arq_hash, char *nome_arquivo_dados, int tam) {
    if(cod < 0) {
        return -1;
    }
    int h = hash(cod, tam);
    if(h == 0) {
        rewind(arq_hash);
    } else {
        fseek(arq_hash, tamanho_compartimento() * (h), SEEK_SET);
    }
    CompartimentoHash *comp = le_compartimento(arq_hash);
    if(comp->prox == -1) {
        return -1;
    } else {
        FILE *arq_dado = fopen(nome_arquivo_dados, "r+b");
        if (arq_dado == NULL) {
            printf("Erro ao abrir o arquivo!\n");
            exit(1);
        }
        if(comp->prox == 0) {
            rewind(arq_dado);
        } else {
            fseek(arq_dado, tamanho_registro() * comp->prox, SEEK_SET);
        }
        int flag = 1;
        int pos = comp->prox;

        while (1) {
            TFunc *f = le(arq_dado);
            // Funcionario nao esta no arquivo
            if (f->prox == -1 && f->cod != cod) {
                fclose(arq_dado);
                free(f);
                return -1;
            }
            // primeiro caso: Funcionario primeiro da lista atualiza o compartimento
            if (f->cod == cod && flag == 1) {
                if(h == 0) {
                    rewind(arq_hash);
                } else {
                    fseek(arq_hash, tamanho_compartimento() * (h), SEEK_SET);
                }
                comp->prox = f->prox;
                salva_compartimento(comp, arq_hash);
                free(comp);
                free(f);
                fclose(arq_dado);
                return 1;
            }
            // segundo caso: Funcionario no meio ou no final da lista
            if (f->cod == cod) {
                TFunc *f_aux = funcionario(f->cod, f->nome, f->salario, f->prox);
                if (pos == 0) {
                    rewind(arq_dado);
                    f = le(arq_dado);
                } else {
                    fseek(arq_dado, tamanho_registro() * pos, SEEK_SET);
                    f = le(arq_dado);
                }
                f->prox = f_aux->prox;
                if (pos == 0) {
                    rewind(arq_dado);
                } else {
                    fseek(arq_dado, tamanho_registro() * pos, SEEK_SET);
                }
                salva(f, arq_dado);
                free(f);
                free(f_aux);
                fclose(arq_dado);
                return 1;
            } else {
                fseek(arq_dado, tamanho_registro() * f->prox, SEEK_SET);
                // Vericar se pode passar o prox de funcionario
                TFunc *f_aux = le(arq_dado);
                int aux = pos;
                if (f_aux->cod != cod) {
                    if (f_aux != NULL) {
                        if(f_aux->cod != cod) {
                            pos = f->prox;
                        }
                    }
                    free(f_aux);
                }
                fseek(arq_dado, tamanho_registro() * f->prox, SEEK_SET);
            }
            flag = 0;
        }
    }
}