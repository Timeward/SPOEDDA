#include <stdio.h>
#include <stdlib.h>
#include "login.h"
#include "userSort.h"
#include "usuarios.h"
#include "locale.h"

int main() {
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
        printf("Bem-vindo ao sistema!\n");
        // Outros procedimentos após login bem-sucedido podem ser colocados aqui
    } else {
        printf("Falha no login.\n");
    }

    return 0;
}