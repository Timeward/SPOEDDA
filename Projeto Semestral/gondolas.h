#ifndef GONDOLAS_H
#define GONDOLAS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da estrutura de um item
typedef struct Item {
    char nome[50];
    char descricao[100];
    float peso;
    float preco;
    struct Item *proximo;
} Item;

// Definição da estrutura da pilha (prateleira)
typedef struct Prateleira {
    Item *topo;
    int quantidade;
} Prateleira;

// Funções para manipulação da pilha
void inicializarPrateleira(Prateleira *p);
int empilharItem(Prateleira *p, const char *nome, const char *descricao, float peso, float preco);
int desempilharItem(Prateleira *p);
void exibirPrateleira(Prateleira *p);
void limparPrateleira(Prateleira *p);

#endif // GONDOLAS_H