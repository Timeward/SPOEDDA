#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "login.h"
#include "locale.h"

#ifdef _WIN32
#include <windows.h> // Para Sleep no Windows
#else
#include <unistd.h>  // Para sleep no Linux/Unix
#endif

#define MAX_NOME 100
#define MAX_PRONTUARIO 20

// Função para remover espaços extras no final da string
void removerEspacos(char *str) {
    int len = strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[len - 1] = '\0';
        len--;
    }
}

// Função para buscar um usuário no arquivo usando busca binária
int buscarUsuario(Usuario **usuarios, int *quantidade, const char *nome, const char *prontuario) {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    #ifdef _WIN32
    SetConsoleOutputCP(65001); // Define código de página UTF-8 no Windows
    #endif
    FILE *file = fopen("USUARIOS.DAT", "rb");
    if (!file) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
        system("cls");
        printf("ARQUIVO USUARIOS.DAT INEXISTENTE!\n");
        Sleep(1000);
        return -1;
    }

    // Conta o número de usuários no arquivo
    fseek(file, 0, SEEK_END);
    long tamanho = ftell(file);
    *quantidade = tamanho / sizeof(Usuario);
    fseek(file, 0, SEEK_SET);

    // Aloca dinamicamente o array de usuários
    *usuarios = (Usuario *)malloc(*quantidade * sizeof(Usuario));
    if (*usuarios == NULL) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
        system("cls");
        printf("malloc devolveu NULL!\n");
        fclose(file);
        Sleep(1000);
        return -1;
    }

    // Lê todos os usuários do arquivo
    fread(*usuarios, sizeof(Usuario), *quantidade, file);
    fclose(file);

    // Debug: Exibir os usuários lidos do arquivo
    // printf("\n--- Usuários cadastrados ---\n");
    // for (int i = 0; i < *quantidade; i++) {
    //     printf("Nome: %s | Prontuário: %s\n", (*usuarios)[i].nome, (*usuarios)[i].prontuario);
    // }
    // printf("-----------------------------\n");
    // Sleep(3000);
    // Realiza a busca binária
    int esquerda = 0, direita = *quantidade - 1;
    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;

        // Removendo espaços extras para evitar falhas na comparação
        removerEspacos((*usuarios)[meio].nome);
        removerEspacos((*usuarios)[meio].prontuario);

        int cmpNome = strcmp((*usuarios)[meio].nome, nome);
        if (cmpNome == 0) {
            if (strcmp((*usuarios)[meio].prontuario, prontuario) == 0) {
                return 1; // Usuário encontrado
            } else {
                return 0; // Nome correto, mas prontuário errado
            }
        }

        if (cmpNome < 0) {
            esquerda = meio + 1;
        } else {
            direita = meio - 1;
        }
    }

    return 0; // Usuário não encontrado
}

// Função principal de login
int realizarLogin() {
        setlocale(LC_ALL, "pt_BR.UTF-8");
    #ifdef _WIN32
        SetConsoleOutputCP(65001); // Define código de página UTF-8 no Windows
    #endif

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    system("cls");

    char nome[MAX_NOME], prontuario[MAX_PRONTUARIO];

    printf("NOME DO USUÁRIO: ");
    fgets(nome, MAX_NOME, stdin);
    removerEspacos(nome);

    printf("PRONTUÁRIO DO USUÁRIO: ");
    fgets(prontuario, MAX_PRONTUARIO, stdin);
    removerEspacos(prontuario);

    Usuario *usuarios = NULL;
    int quantidade = 0;

    int resultado = buscarUsuario(&usuarios, &quantidade, nome, prontuario);

    // Exibe a mensagem de erro ou sucesso
    if (resultado == -1) {
        return -1; // Erro de leitura
    } else if (resultado == 0) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        system("cls");
        printf("USUÁRIO E/OU PRONTUÁRIO INVÁLIDO!\n");
        free(usuarios);
#ifdef _WIN32
        Sleep(3000); // Espera 3 segundos no Windows
#else
        sleep(3);    // Espera 3 segundos no Linux/Unix
#endif
        return 0; // Usuário ou prontuário inválido
    } else {
        printf("Login bem-sucedido!\n");
        free(usuarios);
        return 1; // Login bem-sucedido
    }
}
