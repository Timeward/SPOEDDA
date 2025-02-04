#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gondolas.h"
#include "locale.h"
#include "windows.h"
#include "login.h"

#define USUARIO_AUTORIZADO "placeholder1"
#define PRONTUARIO_AUTORIZADO "placeholder2"

void salvarLista(ListaPrateleiras *lista, const char *nomeArquivo) {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    FILE *arquivo = fopen(nomeArquivo, "wb");
    if (!arquivo) {
        printf("Erro ao abrir arquivo para salvar os dados.\n");
        return;
    }

    Prateleira *prateleiraAtual = lista->inicio;
    while (prateleiraAtual) {
        fwrite(&prateleiraAtual->id, sizeof(int), 1, arquivo);
        fwrite(&prateleiraAtual->quantidade, sizeof(int), 1, arquivo);

        Item *itemAtual = prateleiraAtual->topo;
        while (itemAtual) {
            // printf("Nome antes de salvar: %s\n", itemAtual->nome);
            // for (int i = 0; i < strlen(itemAtual->nome); i++) {
            //     printf("%02X ", (unsigned char)itemAtual->nome[i]);  // Mostra os valores em hexadecimal
            // }
            printf("\n");
            fwrite(itemAtual->nome, sizeof(char), 50, arquivo);  // Nome do item
            fwrite(itemAtual->descricao, sizeof(char), 100, arquivo); // Descrição
            fwrite(&itemAtual->peso, sizeof(float), 1, arquivo); // Peso
            fwrite(&itemAtual->preco, sizeof(float), 1, arquivo); // Preço

            // printf("Nome salvo: %s\n", itemAtual->nome);//TESTE
            // printf("Descrição salva: %s\n", itemAtual->descricao);//TESTE
            // getch();

            itemAtual = itemAtual->prox;
        }

        prateleiraAtual = prateleiraAtual->prox;
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

    while (1) {
        Prateleira *novaPrateleira = (Prateleira *)malloc(sizeof(Prateleira));
        if (!novaPrateleira) {
            printf("Erro ao alocar memória para a prateleira.\n");
            fclose(arquivo);
            return;
        }

        if (fread(&novaPrateleira->id, sizeof(int), 1, arquivo) != 1) {
            free(novaPrateleira);
            break;
        }
        fread(&novaPrateleira->quantidade, sizeof(int), 1, arquivo);
        novaPrateleira->topo = NULL;
        novaPrateleira->prox = lista->inicio;
        lista->inicio = novaPrateleira;

        for (int i = 0; i < novaPrateleira->quantidade; i++) {
            Item *novoItem = (Item *)malloc(sizeof(Item));
            if (!novoItem) {
                printf("Erro ao alocar memória para um item.\n");
                fclose(arquivo);
                return;
            }

            fread(novoItem->nome, sizeof(char), 50, arquivo);
            fread(novoItem->descricao, sizeof(char), 100, arquivo);
            fread(&novoItem->peso, sizeof(float), 1, arquivo);
            fread(&novoItem->preco, sizeof(float), 1, arquivo);

            novoItem->nome[49] = '\0';
            novoItem->descricao[99] = '\0';

            novoItem->prox = novaPrateleira->topo;
            novaPrateleira->topo = novoItem;
        }
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
        // printf("Teste de acentos: áéíóú âêô ç ãõ\n");//TESTE
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
        printf("3. Adicionar itens ao carrinho\n");
        printf("4. Listar prateleiras\n");
        printf("5. Exibir itens de uma prateleira\n");
        printf("0. Voltar ao menu principal\n");
        printf("====================================\n");
        // Verificar e exibir prateleiras vazias
        int encontrouVazia = 0, encontrouCheia = 0;
        Prateleira *atual = lista->inicio;

        #ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        WORD originalAttributes = csbi.wAttributes;
        #endif

        while (atual != NULL) {
            if (atual->quantidade == 0) {
                if (!encontrouVazia) {
                    #ifdef _WIN32
                    SetConsoleTextAttribute(hConsole, (originalAttributes & 0xF0) | (FOREGROUND_RED | FOREGROUND_INTENSITY));
                    #else
                    printf("\033[31m"); // Vermelho para o texto
                    #endif
                    printf("\nAVISO: Existem prateleiras vazias!\n");
                    encontrouVazia = 1;
                }
                printf(" - Prateleira ID %d está vazia!\n", atual->id);
            } 
            else if (atual->quantidade == 5) { // Prateleira cheia
                if (!encontrouCheia) {
                    #ifdef _WIN32
                    SetConsoleTextAttribute(hConsole, (originalAttributes & 0xF0) | (FOREGROUND_BLUE | FOREGROUND_INTENSITY));
                    #else
                    printf("\033[34m"); // Azul para o texto
                    #endif
                    printf("\nAVISO: Existem prateleiras cheias!\n");
                    encontrouCheia = 1;
                }
                printf(" - Prateleira ID %d está cheia!\n", atual->id);
            }
            atual = atual->prox;
        }

        // Resetar cor para o padrão do terminal
        #ifdef _WIN32
        SetConsoleTextAttribute(hConsole, originalAttributes);
        #else
        printf("\033[0m");
        #endif
        printf("====================================\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                if (strcmp(usuarioLogado.nome, USUARIO_AUTORIZADO) != 0 || strcmp(usuarioLogado.prontuario, PRONTUARIO_AUTORIZADO) != 0) {
                    printf("Acesso negado! Você não tem permissão para realizar esta ação.\n");
                    getch();
                    break;
                } else {
                    criarPrateleira(lista);
                    break;
                }
            case 2:
                if (strcmp(usuarioLogado.nome, USUARIO_AUTORIZADO) != 0 || strcmp(usuarioLogado.prontuario, PRONTUARIO_AUTORIZADO) != 0) {
                    printf("Acesso negado! Você não tem permissão para realizar esta ação.\n");
                    getch();
                    break;
                } else {
                    exibirPrateleiras(lista);
                    printf("Informe o ID da prateleira: ");
                    scanf("%d", &id);
                    prateleira = buscarPrateleira(lista, id);
                    if (prateleira) {
                        getchar();
                        printf("Atenção: Não utilizar caractéres com acento ou cedilha.\n");
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
                }
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