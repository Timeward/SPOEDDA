#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "login.h"
#include "locale.h"
#include "usuarios.h"

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
    SetConsoleOutputCP(65001);
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

    fseek(file, 0, SEEK_END);
    long tamanho = ftell(file);
    *quantidade = tamanho / sizeof(Usuario);
    fseek(file, 0, SEEK_SET);

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

    fread(*usuarios, sizeof(Usuario), *quantidade, file);
    fclose(file);

    int esquerda = 0, direita = *quantidade - 1;
    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;

        removerEspacos((*usuarios)[meio].nome);
        removerEspacos((*usuarios)[meio].prontuario);

        int cmpNome = strcmp((*usuarios)[meio].nome, nome);
        if (cmpNome == 0) {
            if (strcmp((*usuarios)[meio].prontuario, prontuario) == 0) {
                return meio; // Retorna o índice do usuário encontrado
            } else {
                return -2; // Nome correto, mas prontuário errado
            }
        }

        if (cmpNome < 0) {
            esquerda = meio + 1;
        } else {
            direita = meio - 1;
        }
    }

    return -3; // Usuário não encontrado
}

// Função principal de login
int realizarLogin(Usuario *usuarioLogado) {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    #ifdef _WIN32
        SetConsoleOutputCP(65001);
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

    int indice = buscarUsuario(&usuarios, &quantidade, nome, prontuario);

    if (indice == -1) {
        return -1; // Erro de leitura do arquivo
    } else if (indice == -2 || indice == -3) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        system("cls");
        printf("USUÁRIO E/OU PRONTUÁRIO INVÁLIDO!\n");
        free(usuarios);
        #ifdef _WIN32
            Sleep(3000);
        #else
            sleep(3);
        #endif
        return 0;
    } else {
        // Armazena os dados do usuário logado usando o índice correto
        strcpy(usuarioLogado->nome, usuarios[indice].nome);
        strcpy(usuarioLogado->prontuario, usuarios[indice].prontuario);
        printf("Login bem-sucedido!\n");
        free(usuarios);
        return 1;//DEBUG: possible issue here? maybe it's not returning the actual user? but it should be a global variable
    }
}
