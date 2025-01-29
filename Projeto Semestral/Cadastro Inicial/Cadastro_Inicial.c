#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 100
#define MAX_PRONTUARIO 20

// Definição da estrutura para armazenar os dados
typedef struct {
    char nome[MAX_NOME];
    char prontuario[MAX_PRONTUARIO];
} Usuarios;

int main() {
    // Dados fornecidos
    Usuarios usuarios[] = {
        {"Domingos Lucas Latorre de Oliveira", "CJ146456"},
        {"Leandro Pinto Santana", "CP220383"},
        {"Rodrigo Ribeiro de Oliveira", "RG134168"},
        {"Andre Luiz da Silva", "SP030028"},
        {"Claudia Miyuki Werhmuller", "SP030041"},
        {"Claudete de Oliveira Alves", "SP03020X"},
        {"Francisco Verissimo Luciano", "SP030247"},
        {"Luk Cho Man", "SP060380"},
        {"Ivan Francolin Martinez", "SP060835"},
        {"Joao Vianei Tamanini", "SP060914"},
        {"Jose Oscar Machado Alexandre", "SP070038"},
        {"Jose Braz de Araujo", "SP070385"},
        {"Paulo Roberto de Abreu", "SP070816"},
        {"Eurides Balbino da Silva", "SP07102X"},
        {"Domingos Bernardo Gomes Santos", "SP090888"},
        {"Andre Evandro Lourenco", "SP100092"},
        {"Miguel Angelo Tancredi Molina", "SP102763"},
        {"Antonio Airton Palladino", "SP112197"},
        {"Luis Fernando Aires Branco Menegueti", "SP145385"},
        {"Antonio Ferreira Viana", "SP200827"},
        {"Leonardo Bertholdo", "SP204973"},
        {"Marcelo Tavares de Santana", "SP20500X"},
        {"Wagner de Paula Gomes", "SP215016"},
        {"Daniel Marques Gomes de Morais", "SP220097"},
        {"Alexandre Beletti Ferreira", "SP226117"},
        {"Vladimir Camelo Pinto", "SP240291"},
        {"Leonardo Andrade Motta de Lima", "SP24031X"},
        {"Aldo Marcelo Paim", "SP240497"},
        {"Cesar Lopes Fernandes", "SP890534"},
        {"Josceli Maria Tenorio", "SZ124382"}
    };

    int totalUsuarios = sizeof(usuarios) / sizeof(usuarios[0]);

    // Abrindo o arquivo para escrita binária
    FILE *arquivo = fopen("USUARIOS.DAT", "wb");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // Escrevendo os dados no arquivo
    for (int i = 0; i < totalUsuarios; i++) {
        fwrite(&usuarios[i], sizeof(Usuarios), 1, arquivo);
    }

    printf("Dados cadastrados com sucesso no arquivo USUARIOS.DAT.\n");

    // Fechando o arquivo
    fclose(arquivo);

    return 0;
}
