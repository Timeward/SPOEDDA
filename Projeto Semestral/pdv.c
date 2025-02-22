#include "pdv.h"
#include <stdlib.h>
#include <conio.h>
#include "gondolas.h"

void exibirTelaPDV(FilaCaixa *fila) {
    int opcao;

    do {
        #ifdef _WIN32
        system("cls");
        #else
        system("clear");
        #endif

        printf("\n====================================\n");
        printf("          PONTO DE VENDA (PDV)      \n");
        printf("====================================\n");
        printf("1. Processar compra\n");
        printf("0. Retornar ao menu principal\n");
        printf("====================================\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("\nIniciando processamento do carrinho...\n");
                processarCaixa(fila);
                break;
            case 0:
                printf("\nRetornando ao menu principal...\n");
                break;
            default:
                printf("\nOpção inválida! Tente novamente.\n");
                getch();
        }
    } while (opcao != 0);
}

void removerQuebraDeLinha(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

void processarCaixa(FilaCaixa *fila) {
    float total = 0.0;

    FILE *arquivoEntrada = fopen("carrinho.dat", "rb");
    if (!arquivoEntrada) {
        printf("Erro: Não foi possível abrir carrinho.dat\n");
        return;
    }

    FILE *arquivoSaida = fopen("notafiscal.dat", "wb");
    if (!arquivoSaida) {
        printf("Erro: Não foi possível criar notafiscal.dat\n");
        fclose(arquivoEntrada);
        return;
    }

    ItemArquivo itemArquivo;
    ItemMemoria itemMemoria;
    int count = 0;

    printf("\nCarregando itens do carrinho...\n");

    // Lendo os itens do arquivo e enfileirando na FilaCaixa
    while (fread(&itemArquivo, sizeof(ItemArquivo), 1, arquivoEntrada) == 1) {
        // Criando e copiando os dados para ItemMemoria
        ItemMemoria item;
        strcpy(item.nome, itemArquivo.nome);
        strcpy(item.descricao, itemArquivo.descricao);
        item.peso = itemArquivo.peso;
        item.preco = itemArquivo.preco;
        item.prox = NULL;

        enfileirar(fila, item);
        // printf("Item enfileirado: %s | Preço: %.2f\n", item.nome, item.preco);  // Debug
        count++;
    }
    fclose(arquivoEntrada);

    if (count == 0) {
        printf("Nenhum item encontrado no carrinho.\n");
        fclose(arquivoSaida);
        return;
    }

    printf("Foram carregados %d itens.\n", count);
    printf("Processando itens para a nota fiscal...\n");

    // Escrevendo os itens na ordem FIFO no arquivo notafiscal.dat
    count = 0;
    while (!filaVazia(fila)) {
        // printf("Antes de desenfileirar, filaVazia: %d\n", filaVazia(fila)); // Debug
        itemMemoria = desenfileirar(fila);
        // printf("Depois de desenfileirar: %s | Preço: %.2f\n", itemMemoria.nome, itemMemoria.preco); // Debug
        // Convertendo ItemMemoria -> ItemArquivo
        strcpy(itemArquivo.nome, itemMemoria.nome);
        strcpy(itemArquivo.descricao, itemMemoria.descricao);
        itemArquivo.peso = itemMemoria.peso;
        itemArquivo.preco = itemMemoria.preco;

        if (fwrite(&itemArquivo, sizeof(ItemArquivo), 1, arquivoSaida) != 1) {
            printf("Erro ao gravar item na nota fiscal.\n");
            break;      
        }
        // printf("Item gravado na nota fiscal: %s | Preço: %.2f\n", itemArquivo.nome, itemArquivo.preco);//Debug
        count++;
        
    }
    fclose(arquivoSaida);

    printf("\nProcessamento concluído. %d itens adicionados a notafiscal.dat\n", count);
    limparCarrinho();

    // Exibir os itens gravados na nota fiscal
    arquivoSaida = fopen("notafiscal.dat", "rb");
    if (!arquivoSaida) {
        printf("Erro ao abrir notafiscal.dat para leitura.\n");
        return;
    }

    printf("\n============= NOTA FISCAL =============\n");
    printf("Produto                     | Preço (R$)\n");
    printf("----------------------------------------\n");

    while (fread(&itemArquivo, sizeof(ItemArquivo), 1, arquivoSaida) == 1) {
        removerQuebraDeLinha(itemArquivo.nome);
        printf("%-28s | %.2f\n", itemArquivo.nome, itemArquivo.preco);
        total += itemArquivo.preco;
    }

    printf("----------------------------------------\n");
    printf("TOTAL                       | %10.2f\n", total);

    fclose(arquivoSaida);

    printf("\nPressione qualquer tecla para continuar...\n");
    getch();
}

// Inicializar fila
void inicializarFila(FilaCaixa *fila) {
    fila->frente = NULL;
    fila->tras = NULL;
}

// Verifica se a fila está vazia
int filaVazia(FilaCaixa *fila) {
    return fila->frente == NULL;
}

// Enfileirar um item na fila do caixa
void enfileirar(FilaCaixa *fila, ItemMemoria item) {
    NoFila *novo = (NoFila *)malloc(sizeof(NoFila));
    if (!novo) {
        printf("Erro ao alocar memória para o item.\n");
        return;
    }

    // Copia os dados corretamente
    strcpy(novo->item.nome, item.nome);
    strcpy(novo->item.descricao, item.descricao);
    novo->item.peso = item.peso;
    novo->item.preco = item.preco;

    // Inserir o novo nó no início da fila para inverter a ordem
    novo->prox = fila->frente;
    fila->frente = novo;

    // Se a fila estava vazia, ajustamos o ponteiro traseiro
    if (fila->tras == NULL) {
        fila->tras = novo;
    }
}

// Desenfileirar um item da fila do caixa
ItemMemoria desenfileirar(FilaCaixa *fila) {
    ItemMemoria itemVazio = {"", "", 0.0, 0.0, NULL};

    if (filaVazia(fila)) {
        printf("Fila vazia!\n");
        return itemVazio;
    }

    NoFila *removido = fila->frente;
    ItemMemoria item = removido->item; // Copia os dados

    // printf("Item desenfileirado: %s | Preço: %.2f\n", item.nome, item.preco); // Debug

    fila->frente = removido->prox; // Avança na fila

    if (fila->frente == NULL) {
        fila->tras = NULL; // Se a fila ficou vazia, atualiza o ponteiro traseiro
    }

    free(removido);
    return item;
}

// Liberar memória da fila
void liberarFila(FilaCaixa *fila) {
    while (!filaVazia(fila)) {
        desenfileirar(fila);
    }
}

void limparCarrinho() {
    FILE *arquivo = fopen("carrinho.dat", "wb");
    if (!arquivo) {
        printf("Erro ao limpar carrinho.dat.\n");
        return;
    }
    fclose(arquivo);
    printf("\nCarrinho esvaziado após o processamento.\n");
}