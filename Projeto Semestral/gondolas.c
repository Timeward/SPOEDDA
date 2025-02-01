//TODO: Ao ler o arquivo gondolas.dat, dados são perdidos.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gondolas.h"

void salvarLista(ListaPrateleiras *lista, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "wb");
    if (!arquivo) {
        printf("Erro ao abrir arquivo para salvar os dados.\n");
        return;
    }
    
    // Primeiro, grave o número total de prateleiras
    int totalPrateleiras = 0;
    Prateleira *p = lista->inicio;
    while (p) {
        totalPrateleiras++;
        p = p->prox;
    }
    fwrite(&totalPrateleiras, sizeof(int), 1, arquivo);
    
    // Para cada prateleira, grave os dados essenciais
    p = lista->inicio;
    while (p) {
        // Grave os dados da prateleira: id e quantidade
        fwrite(&p->id, sizeof(int), 1, arquivo);
        fwrite(&p->quantidade, sizeof(int), 1, arquivo);
        
        // Para cada prateleira, grave os itens (número de itens é p->quantidade)
        Item *it = p->topo;
        while (it) {
            fwrite(it->nome, sizeof(char), 50, arquivo);
            fwrite(it->descricao, sizeof(char), 100, arquivo);
            fwrite(&it->peso, sizeof(float), 1, arquivo);
            fwrite(&it->preco, sizeof(float), 1, arquivo);
            it = it->prox;
        }
        
        p = p->prox;
    }
    
    fclose(arquivo);
    printf("Dados salvos com sucesso!\n");
}

void carregarLista(ListaPrateleiras *lista, const char *nomeArquivo) {
        FILE *arquivo = fopen(nomeArquivo, "rb");
    if (!arquivo) {
        printf("Nenhum arquivo de dados encontrado. Criando uma nova lista...\n");
        inicializarLista(lista);
        return;
    }

    inicializarLista(lista);

    int totalPrateleiras;
    fread(&totalPrateleiras, sizeof(int), 1, arquivo);

    for (int i = 0; i < totalPrateleiras; i++) {
        // Criar nova prateleira
        Prateleira *novaPrateleira = (Prateleira *)malloc(sizeof(Prateleira));
        if (!novaPrateleira) {
            printf("Erro ao alocar memória para a prateleira.\n");
            fclose(arquivo);
            return;
        }

        fread(&novaPrateleira->id, sizeof(int), 1, arquivo);
        fread(&novaPrateleira->quantidade, sizeof(int), 1, arquivo);
        novaPrateleira->topo = NULL;
        novaPrateleira->prox = lista->inicio;
        lista->inicio = novaPrateleira;

        // Criar um array temporário para armazenar os itens
        Item *itensTemp = (Item *)malloc(novaPrateleira->quantidade * sizeof(Item));
        if (!itensTemp) {
            printf("Erro ao alocar memória para itens temporários.\n");
            fclose(arquivo);
            return;
        }

        // Ler os itens do arquivo para o array temporário
        fread(itensTemp, sizeof(Item), novaPrateleira->quantidade, arquivo);

        // Empilhar os itens de trás para frente para manter a ordem original
        for (int j = novaPrateleira->quantidade - 1; j >= 0; j--) {
            Item *novoItem = (Item *)malloc(sizeof(Item));
            if (!novoItem) {
                printf("Erro ao alocar memória para um item.\n");
                fclose(arquivo);
                return;
            }

            *novoItem = itensTemp[j];  // Copia o item do array
            novoItem->prox = novaPrateleira->topo; // Empilha corretamente
            novaPrateleira->topo = novoItem;
        }

        free(itensTemp); // Liberar memória do array temporário
    }

    fclose(arquivo);
    printf("Dados carregados com sucesso!\n");
}

// Inicializa a lista de prateleiras
void inicializarLista(ListaPrateleiras *lista) {
    lista->inicio = NULL;
    lista->contador = 0;
}

// Cria uma nova prateleira e a adiciona à lista
void criarPrateleira(ListaPrateleiras *lista) {
    Prateleira *nova = (Prateleira *)malloc(sizeof(Prateleira));
    if (nova == NULL) {
        printf("Erro ao alocar memória para a nova prateleira!\n");
        return;
    }
    nova->id = ++lista->contador;
    nova->topo = NULL;
    nova->quantidade = 0;
    nova->prox = lista->inicio;
    lista->inicio = nova;

    printf("Nova prateleira criada com ID: %d\n", nova->id);
}

// Busca uma prateleira pelo ID
Prateleira* buscarPrateleira(ListaPrateleiras *lista, int id) {
    Prateleira *atual = lista->inicio;
    while (atual != NULL) {
        if (atual->id == id) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

// Adiciona um item à prateleira (empilhar)
void empilharItem(Prateleira *prateleira, const char *nome, const char *descricao, float peso, float preco, ListaPrateleiras *lista) {
    if (prateleira->quantidade >= 5) {
        printf("A prateleira %d está cheia!\n", prateleira->id);
        return;
    }

    Item *novo = (Item *)malloc(sizeof(Item));
    if (novo == NULL) {
        printf("Erro ao alocar memória para o item!\n");
        return;
    }

    strcpy(novo->nome, nome);
    strcpy(novo->descricao, descricao);
    novo->peso = peso;
    novo->preco = preco;
    novo->prox = prateleira->topo;
    prateleira->topo = novo;
    prateleira->quantidade++;

    printf("Item '%s' adicionado à prateleira %d\n", nome, prateleira->id);

    // 🚀 Aqui está a chamada correta
    salvarLista(lista, "gondolas.dat");
}

// Remove um item da prateleira (desempilhar)
void desempilharItem(Prateleira *prateleira, ListaPrateleiras *lista) {
    if (prateleira->topo == NULL) {
        printf("A prateleira %d está vazia!\n", prateleira->id);
        return;
    }

    Item *removido = prateleira->topo;
    prateleira->topo = removido->prox;
    prateleira->quantidade--;

    printf("Item '%s' removido da prateleira %d\n", removido->nome, prateleira->id);
    free(removido);

    // 🚀 Salva a lista após remover o item
    salvarLista(lista, "gondolas.dat");
}

// Exibe todas as prateleiras disponíveis
void exibirPrateleiras(ListaPrateleiras *lista) {
        SetConsoleOutputCP(65001); // Define código de página UTF-8 no Windows
    Prateleira *atual = lista->inicio;
    if (atual == NULL) {
        printf("Nenhuma prateleira cadastrada.\n");
        return;
    }

    printf("\n==== PRATELEIRAS ====\n");
    while (atual != NULL) {
        printf("Prateleira ID: %d | Itens: %d/5\n", atual->id, atual->quantidade);
        atual = atual->prox;
    }
}

// Exibe os itens de uma prateleira específica
void exibirItensPrateleira(Prateleira *prateleira) {
    if (prateleira->topo == NULL) {
        printf("A prateleira %d está vazia!\n", prateleira->id);
        return;
    }

    printf("\nItens na Prateleira %d:\n", prateleira->id);
    Item *atual = prateleira->topo;
    while (atual != NULL) {
        printf("- %s | %s | Peso: %.2f kg | Preço: R$ %.2f\n",
               atual->nome, atual->descricao, atual->peso, atual->preco);
        atual = atual->prox;
    }
    getch();
}

// Limpa toda a memória alocada
void limparLista(ListaPrateleiras *lista) {
    while (lista->inicio != NULL) {
        Prateleira *atual = lista->inicio;
        lista->inicio = atual->prox;

        while (atual->topo != NULL) {
            Item *removido = atual->topo;
            atual->topo = removido->prox;
            free(removido);
        }

        free(atual);
    }
}

// Menu de gerenciamento de prateleiras
void menuGondolas(ListaPrateleiras *lista) {
    SetConsoleOutputCP(65001); // Define código de página UTF-8 no Windows
    int opcao, id;
    char nome[50], descricao[100];
    float peso, preco;
    Prateleira *prateleira;

    do {
        #ifdef _WIN32
        system("cls");
        #else
        system("clear");
        #endif

        printf("\n====================================\n");
        printf("     GERENCIAMENTO DE GÔNDOLAS\n");
        printf("====================================\n");
        printf("1. Criar nova prateleira\n");
        printf("2. Incluir item em prateleira\n");
        printf("3. Remover item de prateleira\n");
        printf("4. Listar prateleiras\n");
        printf("5. Exibir itens de uma prateleira\n");
        printf("0. Voltar ao menu principal\n");
        printf("====================================\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                criarPrateleira(lista);
                break;
            case 2:
                exibirPrateleiras(lista);
                printf("Informe o ID da prateleira: ");
                scanf("%d", &id);
                prateleira = buscarPrateleira(lista, id);
                if (prateleira) {
                    getchar();
                    printf("Nome do item: ");
                    fgets(nome, sizeof(nome), stdin);
                    printf("Descrição: ");
                    fgets(descricao, sizeof(descricao), stdin);
                    printf("Peso: ");
                    scanf("%f", &peso);
                    printf("Preço: ");
                    scanf("%f", &preco);
                    empilharItem(prateleira, nome, descricao, peso, preco, lista);
                } else {
                    printf("Prateleira não encontrada!\n");
                }
                break;
            case 3:
                exibirPrateleiras(lista);
                printf("Informe o ID da prateleira: ");
                scanf("%d", &id);
                prateleira = buscarPrateleira(lista, id);
                if (prateleira) {
                    desempilharItem(prateleira, lista);
                } else {
                    printf("Prateleira não encontrada!\n");
                }
                break;
            case 4:
                exibirPrateleiras(lista);
                break;
            case 5:
                printf("Informe o ID da prateleira: ");
                scanf("%d", &id);
                prateleira = buscarPrateleira(lista, id);
                if (prateleira) exibirItensPrateleira(prateleira);
                break;
        }
    } while (opcao != 0);

    limparLista(lista);
}