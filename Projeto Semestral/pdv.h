#ifndef PDV_H
#define PDV_H

#include "gondolas.h"  // Inclui a definição de Item

// Estrutura da fila do caixa
typedef struct NoFila {
    ItemMemoria item;
    struct NoFila *prox;
} NoFila;

typedef struct {
    NoFila *frente;
    NoFila *tras;
} FilaCaixa;

// Funções para manipular a fila do caixa
void inicializarFila(FilaCaixa *fila);
void enfileirar(FilaCaixa *fila, ItemMemoria item);
ItemMemoria desenfileirar(FilaCaixa *fila);
int filaVazia(FilaCaixa *fila);
void processarCaixa(FilaCaixa *fila);
void exibirTelaPDV(FilaCaixa *fila);

#endif