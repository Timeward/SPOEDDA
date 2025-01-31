#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gondolas.h"

// Inicializa uma prateleira vazia
void inicializarPrateleira(Prateleira *p) {
    p->topo = NULL;
    p->quantidade = 0;
}

// Adiciona um item ao topo da prateleira
int empilharItem(Prateleira *p, const char *nome, const char *descricao, float peso, float preco) {
    if (p->quantidade >= 5) {
        printf("Prateleira cheia! Não é possível adicionar mais itens.\n");
        return 0;
    }

    Item *novo = (Item *)malloc(sizeof(Item));
    if (!novo) {
        printf("Erro de alocação de memória!\n");
        return 0;
    }

    strcpy(novo->nome, nome);
    strcpy(novo->descricao, descricao);
    novo->peso = peso;
    novo->preco = preco;
    novo->proximo = p->topo;
    p->topo = novo;
    p->quantidade++;

    return 1;
}

// Remove um item do topo da prateleira
int desempilharItem(Prateleira *p) {
    if (p->topo == NULL) {
        printf("Prateleira vazia! Não há itens para remover.\n");
        return 0;
    }

    Item *removido = p->topo;
    p->topo = removido->proximo;
    free(removido);
    p->quantidade--;

    return 1;
}

// Exibe os itens da prateleira
void exibirPrateleira(Prateleira *p) {
    if (p->topo == NULL) {
        printf("Prateleira vazia!\n");
        return;
    }

    Item *atual = p->topo;
    printf("\n--- Prateleira ---\n");
    while (atual) {
        printf("Nome: %s\n", atual->nome);
        printf("Descrição: %s\n", atual->descricao);
        printf("Peso: %.2f kg\n", atual->peso);
        printf("Preço: R$ %.2f\n", atual->preco);
        printf("------------------\n");
        atual = atual->proximo;
    }
}

// Esvazia completamente a prateleira
void limparPrateleira(Prateleira *p) {
    while (desempilharItem(p));
}