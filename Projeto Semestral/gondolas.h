#ifndef GONDOLAS_H
#define GONDOLAS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura usada na memória (com ponteiro para a lista)
typedef struct ItemMemoria {
    char nome[50];
    char descricao[100];
    float peso;
    float preco;
    struct ItemMemoria *prox; // Apenas na memória
} ItemMemoria;

// Estrutura usada para salvar no arquivo (sem ponteiro)
typedef struct {
    char nome[50];
    char descricao[100];
    float peso;
    float preco;
} ItemArquivo;

// Definição da estrutura de uma prateleira (Pilha)
typedef struct Prateleira {
    int id;  // Identificador único da prateleira
    ItemMemoria *topo;
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
