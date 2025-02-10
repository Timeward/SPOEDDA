#include "pdv.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

typedef struct Item {
    char nome[50];
    char descricao[100];
    float peso;
    float preco;
    struct Item *prox;
} Item;

typedef struct {
    Item *frente;
    Item *tras;
} FilaCaixa;

void inicializarFila(FilaCaixa *fila) {
    fila->frente = NULL;
    fila->tras = NULL;
}

int filaVazia(FilaCaixa *fila) {
    return fila->frente == NULL;
}

void enfileirar(FilaCaixa *fila, Item item) {
    Item *novo = (Item *)malloc(sizeof(Item));
    if (!novo) {
        printf("Erro ao alocar memória para o item.\n");
        return;
    }
    *novo = item;
    novo->prox = NULL;
    
    if (filaVazia(fila)) {
        fila->frente = novo;
    } else {
        fila->tras->prox = novo;
    }
    fila->tras = novo;
}

Item desenfileirar(FilaCaixa *fila) {
    Item itemVazio = {"", "", 0.0, 0.0, NULL};
    if (filaVazia(fila)) {
        printf("Fila vazia!\n");
        return itemVazio;
    }
    
    Item *removido = fila->frente;
    Item item = *removido; // Copia os dados
    fila->frente = removido->prox;
    
    if (fila->frente == NULL) {
        fila->tras = NULL;
    }
    
    free(removido);
    return item;
}

void carregarCarrinho(FilaCaixa *caixa) {
    FILE *arquivo = fopen("carrinho.dat", "rb");
    if (!arquivo) {
        printf("Nenhum carrinho encontrado.\n");
        return;
    }

    Item item;
    while (fread(&item, sizeof(Item), 1, arquivo) == 1) {
        enfileirar(caixa, item);
    }

    fclose(arquivo);
    printf("Carrinho carregado. Pressione qualquer tecla para continuar...\n");
    getch();
}

void processarCaixa(FilaCaixa *caixa) {
    printf("\nItens no caixa:\n");
    while (!filaVazia(caixa)) {
        Item item = desenfileirar(caixa);
        printf("- %s | %.2f kg | R$ %.2f\n", item.nome, item.peso, item.preco);
    }
}
