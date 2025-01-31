#include <stdio.h>
#include <stdlib.h>
#include "login.h"
#include "userSort.h"
#include "usuarios.h"
#include "locale.h"
#include "windows.h"

int main(){
    setlocale(LC_ALL, "pt_BR.UTF-8"); // Configura locale para UTF-8
    #ifdef _WIN32
    SetConsoleOutputCP(65001); // Define código de página UTF-8 no Windows
    #endif

    // Carrega os usuários do arquivo
    int quantidade = 0;
    Usuario *usuarios = carregarUsuarios(&quantidade);

    if (usuarios != NULL && quantidade > 0) {
        // Ordena a lista de usuários
        userSort(usuarios, 0, quantidade - 1);

        // Salva os usuários ordenados de volta no arquivo
        salvarUsuarios(usuarios, quantidade);

        printf("Usuários ordenados e salvos com sucesso!\n");

        // Libera a memória
        free(usuarios);
    } else {
        printf("Nenhum usuário encontrado para ordenar.\n");
    }

    // Realiza o login
    int loginResultado = realizarLogin();

    // Verifica o resultado do login
    if (loginResultado == 1) {

        int opcao;

        do{
            #ifdef _WIN32
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_INTENSITY);
            system("cls");  // Limpa a tela no Windows
            #else
            system("clear"); // Limpa a tela no Linux/macOS
            printf("\033[47m"); // Fundo branco
            printf("\033[30m"); // Texto preto
            #endif


            printf("\n====================================\n");
            printf("  SISTEMA GERENCIADOR DE SUPERMERCADO  \n");
            printf("====================================\n");
            printf("\n   Menu de Opções\n");
            printf("   1. Abastecer Gôndolas\n");
            printf("   2. Caixa/PDV\n");
            printf("   3. Gerenciar Usuários\n");
            printf("   0. Sair\n");
            printf("\n   Escolha opção: ");


            scanf("%d", &opcao);
            getchar(); // Limpa o buffer do teclado

            switch (opcao) {
                case 1:
                    printf("\nOpção 1 - Abastecer Gôndolas selecionada.\n");
                    break;
                case 2:
                    printf("\nOpção 2 - Caixa/PDV selecionada.\n");
                    break;
                case 3:
                    printf("\nOpção 3 - Gerenciar Usuários selecionada.\n");
                    break;
                case 0:
                    printf("\nSaindo do sistema...\n");
                    break;
                default:
                    printf("\nOpção inválida! Tente novamente.\n");
            }
        }while (opcao != 0);   

    return 0;
    }
}