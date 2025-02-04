#ifndef USERS_H
#define USERS_H
#include "usuarios.h"

Usuario* carregarUsers(int *quantidade);
void salvarUsers(Usuario *usuarios, int quantidade);
void inserirUsuarioOrdenado(Usuario novoUsuario);
void cadastrarUser();
void menuGerenciarUsuarios();

#endif
