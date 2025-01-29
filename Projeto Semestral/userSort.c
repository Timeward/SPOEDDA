#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "userSort.h"
#include "locale.h"

#define ARQUIVO_USUARIOS "USUARIOS.DAT"

// Função para encontrar o índice do pivô mediano sem alterar os valores do array
int encontrarMediano(int esquerda, int direita, Usuario usuarios[]) {
    int meio = (esquerda + direita) / 2;

    int a = esquerda, b = meio, c = direita;

    if ((strcmp(usuarios[a].nome, usuarios[b].nome) > 0) != (strcmp(usuarios[a].nome, usuarios[c].nome) > 0))
        return a;
    else if ((strcmp(usuarios[b].nome, usuarios[a].nome) > 0) != (strcmp(usuarios[b].nome, usuarios[c].nome) > 0))
        return b;
    else
        return c;
}

// Função para ordenar os usuários usando QuickSort com pivô mediano
void userSort(Usuario usuarios[], int esquerda, int direita) {
    if (esquerda >= direita) return;

    int pivoIndex = encontrarMediano(esquerda, direita, usuarios);
    Usuario pivot = usuarios[pivoIndex];

    Usuario temp = usuarios[pivoIndex];
    usuarios[pivoIndex] = usuarios[direita];
    usuarios[direita] = temp;

    int i = esquerda - 1;

    for (int j = esquerda; j < direita; j++) {
        if (strcmp(usuarios[j].nome, pivot.nome) < 0) {
            i++;
            Usuario temp = usuarios[i];
            usuarios[i] = usuarios[j];
            usuarios[j] = temp;
        }
    }

    temp = usuarios[i + 1];
    usuarios[i + 1] = usuarios[direita];
    usuarios[direita] = temp;

    userSort(usuarios, esquerda, i);
    userSort(usuarios, i + 1, direita);
}

// Função para carregar usuários do arquivo
Usuario* carregarUsuarios(int *quantidade) {
    FILE *file = fopen(ARQUIVO_USUARIOS, "rb");
    if (!file) {
        printf("Erro ao abrir o arquivo %s.\n", ARQUIVO_USUARIOS);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long tamanho = ftell(file);
    *quantidade = tamanho / sizeof(Usuario);
    fseek(file, 0, SEEK_SET);

    Usuario *usuarios = (Usuario *)malloc(*quantidade * sizeof(Usuario));
    if (!usuarios) {
        printf("Erro ao alocar memória!\n");
        fclose(file);
        return NULL;
    }

    fread(usuarios, sizeof(Usuario), *quantidade, file);
    fclose(file);

    return usuarios;
}

// Função para salvar usuários ordenados no arquivo
void salvarUsuarios(Usuario *usuarios, int quantidade) {
    FILE *file = fopen(ARQUIVO_USUARIOS, "wb");
    if (!file) {
        printf("Erro ao abrir o arquivo %s para escrita.\n", ARQUIVO_USUARIOS);
        return;
    }

    fwrite(usuarios, sizeof(Usuario), quantidade, file);
    fclose(file);
}

// Função principal para ordenar os usuários no arquivo
void ordenarUsuariosArquivo() {
    int quantidade;
    Usuario *usuarios = carregarUsuarios(&quantidade);

    if (usuarios) {
        userSort(usuarios, 0, quantidade - 1);
        salvarUsuarios(usuarios, quantidade);
        printf("Usuários ordenados e salvos com sucesso!\n");
        free(usuarios);
    }
}
