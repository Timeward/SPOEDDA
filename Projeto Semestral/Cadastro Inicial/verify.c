#include <stdio.h>
#include <stdlib.h>

#define MAX_NOME 100
#define MAX_PRONTUARIO 20

// Definição da estrutura para armazenar os dados
typedef struct {
    char nome[MAX_NOME];
    char prontuario[MAX_PRONTUARIO];
} Usuario;

int main() {
    // Abrindo o arquivo para leitura binária
    FILE *arquivo = fopen("USUARIOS.DAT", "rb");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    Usuario usuario;

    printf("Listagem de usuários:\n");
    printf("----------------------\n");

    // Lendo os dados do arquivo e exibindo no terminal
    while (fread(&usuario, sizeof(Usuario), 1, arquivo) == 1) {
        printf("Nome: %s\n", usuario.nome);
        printf("Prontuário: %s\n", usuario.prontuario);
        printf("----------------------\n");
    }

    // Fechando o arquivo
    fclose(arquivo);

    // Esperando que o usuário pressione uma tecla antes de fechar o terminal
    printf("Pressione qualquer tecla para sair...");
    getchar();

    return 0;
}