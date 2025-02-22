#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gondolas.h"
#include "locale.h"
#include "windows.h"
#include "login.h"
#include "usuarios.h"

#define USUARIO_AUTORIZADO "Eduardo Massaru Tutui"
#define PRONTUARIO_AUTORIZADO "SP3056945"

void salvarLista(ListaPrateleiras *lista, const char *nomeArquivo) {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    if (lista->inicio == NULL) {
        printf("Nenhuma prateleira para salvar. Operação ignorada.\n");
        return;
    }
    FILE *arquivo = fopen(nomeArquivo, "wb");
    if (!arquivo) {
        printf("Erro ao abrir arquivo para salvar os dados.\n");
        return;
    }

    Prateleira *prateleiraAtual = lista->inicio;
    while (prateleiraAtual) {
        fwrite(&prateleiraAtual->id, sizeof(int), 1, arquivo);
        fwrite(&prateleiraAtual->quantidade, sizeof(int), 1, arquivo);

        ItemMemoria *itemAtual = prateleiraAtual->topo;
        while (itemAtual) {
            ItemArquivo itemSalvar;
            strcpy(itemSalvar.nome, itemAtual->nome);
            strcpy(itemSalvar.descricao, itemAtual->descricao);
            itemSalvar.peso = itemAtual->peso;
            itemSalvar.preco = itemAtual->preco;

            fwrite(&itemSalvar, sizeof(ItemArquivo), 1, arquivo);
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

    fseek(arquivo, 0, SEEK_END);
    if (ftell(arquivo) == 0) {  // Arquivo vazio
        printf("Arquivo de dados está vazio. Criando uma nova lista...\n");
        fclose(arquivo);
        inicializarLista(lista);
        return;
    }
    rewind(arquivo);

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
            break;  // Fim do arquivo
        }
        fread(&novaPrateleira->quantidade, sizeof(int), 1, arquivo);

        novaPrateleira->topo = NULL;
        novaPrateleira->prox = lista->inicio;
        lista->inicio = novaPrateleira;

        // Criando um array temporário para armazenar os itens
        ItemArquivo *itensTemp = (ItemArquivo *)malloc(novaPrateleira->quantidade * sizeof(ItemArquivo));
        if (!itensTemp) {
            printf("Erro ao alocar memória para os itens temporários.\n");
            fclose(arquivo);
            return;
        }

        // Lendo os itens do arquivo para o array temporário
        fread(itensTemp, sizeof(ItemArquivo), novaPrateleira->quantidade, arquivo);

        // Empilhando os itens na ordem correta (LIFO)
        for (int i = novaPrateleira->quantidade - 1; i >= 0; i--) {
            ItemMemoria *novoItem = (ItemMemoria *)malloc(sizeof(ItemMemoria));
            if (!novoItem) {
                printf("Erro ao alocar memória para um item.\n");
                free(itensTemp);
                fclose(arquivo);
                return;
            }

            strcpy(novoItem->nome, itensTemp[i].nome);
            strcpy(novoItem->descricao, itensTemp[i].descricao);
            novoItem->peso = itensTemp[i].peso;
            novoItem->preco = itensTemp[i].preco;

            // Empilhar corretamente (LIFO)
            novoItem->prox = novaPrateleira->topo;
            novaPrateleira->topo = novoItem;
        }

        free(itensTemp);  // Libera a memória do array temporário
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

    ItemMemoria *novo = (ItemMemoria *)malloc(sizeof(ItemMemoria));
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
    salvarLista(lista, "gondolas.dat");
}

// Remove um item da prateleira (desempilhar)
void desempilharItem(Prateleira *prateleira, ListaPrateleiras *lista) {
    if (prateleira->topo == NULL) {
        printf("A prateleira %d está vazia!\n", prateleira->id);
        return;
    }

    ItemMemoria *removido = prateleira->topo;
    prateleira->topo = removido->prox;
    prateleira->quantidade--;

    FILE *arquivo = fopen("carrinho.dat", "ab+");
    if (!arquivo) {
        printf("Erro ao abrir carrinho.dat para salvar item.\n");
        return;
    }

    ItemArquivo itemSalvar;
    strcpy(itemSalvar.nome, removido->nome);
    strcpy(itemSalvar.descricao, removido->descricao);
    itemSalvar.peso = removido->peso;
    itemSalvar.preco = removido->preco;

    fwrite(&itemSalvar, sizeof(ItemArquivo), 1, arquivo);
    fclose(arquivo);

    printf("Item '%s' removido da prateleira %d e adicionado ao carrinho.\n", removido->nome, prateleira->id);

    free(removido);
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
    getch();
}

// Exibe os itens de uma prateleira específica
void exibirItensPrateleira(Prateleira *prateleira) {
    if (prateleira->topo == NULL) {
        printf("A prateleira %d está vazia!\n", prateleira->id);
        return;
    }

    printf("\nItens na Prateleira %d:\n", prateleira->id);
    ItemMemoria *atual = prateleira->topo;
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
            ItemMemoria *removido = atual->topo;
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
            case 0:
                salvarLista(lista, "gondolas.dat");
                printf("Saindo e salvando dados...\n");
                break;
        }
    } while (opcao != 0);

    limparLista(lista);
}