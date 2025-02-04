#ifndef LOGIN_H
#define LOGIN_H

#include "usuarios.h"

// Declaração global (será definida em login.c)
extern Usuario usuarioLogado;

// Função que exibe a tela de login, realiza a busca no arquivo e retorna o status do login
int realizarLogin();

#endif
