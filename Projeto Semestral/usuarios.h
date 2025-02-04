#ifndef USUARIOS_H
#define USUARIOS_H

// Definição da estrutura Usuario
typedef struct {
    char nome[100];
    char prontuario[20];
} Usuario;

// Declaração global (será definida em main.c)
extern Usuario usuarioLogado;

#endif