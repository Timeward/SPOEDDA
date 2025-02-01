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
    struct Item *prox;
} Item;

// Definição da estrutura de uma prateleira (Pilha)
typedef struct Prateleira {
    int id;  // Identificador único da prateleira
    Item *topo;
    int quantidade;
    struct Prateleira *prox;
} Prateleira;

// Lista encadeada de prateleiras
typedef struct {
    Prateleira *inicio;
    int contador;
} ListaPrateleiras;

// Funções para gerenciar as prateleiras
void inicializarLista(ListaPrateleiras *lista);
void criarPrateleira(ListaPrateleiras *lista);
void empilharItem(Prateleira *prateleira, const char *nome, const char *descricao, float peso, float preco, ListaPrateleiras *lista);
void desempilharItem(Prateleira *prateleira, ListaPrateleiras *lista);
void exibirPrateleiras(ListaPrateleiras *lista);
void exibirItensPrateleira(Prateleira *prateleira);
Prateleira* buscarPrateleira(ListaPrateleiras *lista, int id);
void menuGondolas(ListaPrateleiras *lista);
void limparLista(ListaPrateleiras *lista);
void salvarLista(ListaPrateleiras *lista, const char *nomeArquivo);
void carregarLista(ListaPrateleiras *lista, const char *arquivo);

#endif
