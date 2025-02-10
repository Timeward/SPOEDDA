#ifndef PDV_H
#define PDV_H

#include "gondolas.h"

// Estrutura para representar a fila do caixa
typedef struct NoFila {
    Item item;
    struct NoFila *prox;
} NoFila;

typedef struct {
    NoFila *inicio;
    NoFila *fim;
} FilaCaixa;

// Funções para manipular o caixa
void inicializarFila(FilaCaixa *fila);
void enfileirar(FilaCaixa *fila, Item item);
Item desenfileirar(FilaCaixa *fila);
int filaVazia(FilaCaixa *fila);
void carregarCarrinho(FilaCaixa *fila);
void processarCaixa(FilaCaixa *fila);
void exibirItensCaixa(FilaCaixa *fila);

#endif