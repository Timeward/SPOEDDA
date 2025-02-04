#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usuarios.h"

#define ARQUIVO_USUARIOS "USUARIOS.DAT"

// Função para carregar os usuários do arquivo
Usuario* carregarUsers(int *quantidade) {
    FILE *arquivo = fopen(ARQUIVO_USUARIOS, "rb");
    if (!arquivo) {
        *quantidade = 0;
        return NULL;
    }

    fseek(arquivo, 0, SEEK_END);
    *quantidade = ftell(arquivo) / sizeof(Usuario);
    rewind(arquivo);

    Usuario *usuarios = (Usuario*) malloc(*quantidade * sizeof(Usuario));
    if (!usuarios) {
        fclose(arquivo);
        return NULL;
    }

    fread(usuarios, sizeof(Usuario), *quantidade, arquivo);
    fclose(arquivo);
    return usuarios;
}

// Função para salvar usuários no arquivo
void salvarUsers(Usuario *usuarios, int quantidade) {
    FILE *arquivo = fopen(ARQUIVO_USUARIOS, "wb");
    if (!arquivo) return;
    fwrite(usuarios, sizeof(Usuario), quantidade, arquivo);
    fclose(arquivo);
}

// Função para inserir um usuário de forma ordenada (Insertion Sort)
void inserirUsuarioOrdenado(Usuario novoUsuario) {
    int quantidade;
    Usuario *usuarios = carregarUsers(&quantidade);
    
    usuarios = realloc(usuarios, (quantidade + 1) * sizeof(Usuario));
    if (!usuarios) return;

    int i = quantidade - 1;
    while (i >= 0 && strcmp(usuarios[i].nome, novoUsuario.nome) > 0) {
        usuarios[i + 1] = usuarios[i];
        i--;
    }
    usuarios[i + 1] = novoUsuario;

    salvarUsers(usuarios, quantidade + 1);
    free(usuarios);
}

// Função para cadastrar um novo usuário
void cadastrarUser() {
    Usuario novoUsuario;
    printf("\nDigite o nome do usuário: ");
    fgets(novoUsuario.nome, 100, stdin);
    novoUsuario.nome[strcspn(novoUsuario.nome, "\n")] = 0;
    
    printf("Digite o prontuário do usuário: ");
    fgets(novoUsuario.prontuario, 20, stdin);
    novoUsuario.prontuario[strcspn(novoUsuario.prontuario, "\n")] = 0;

    inserirUsuarioOrdenado(novoUsuario);
    printf("Usuário cadastrado com sucesso!\n");
}

// Menu de gerenciamento de usuários
void menuGerenciarUsuarios() {
    int opcao;
    do {
        #ifdef _WIN32
        system("cls");
        #else
        system("clear");
        #endif
            printf("\n====================================\n");
            printf("         GERENCIAR USUÁRIOS         \n");
            printf("====================================\n");
        printf("1. Cadastrar Usuário\n");
        printf("0. Voltar\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // Limpa buffer

        switch (opcao) {
            case 1:
                cadastrarUser();
                break;
            case 0:
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);
}
