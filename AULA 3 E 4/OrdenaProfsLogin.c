/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Constantes */
#define   TOTPROFS    6

/* Tipos pr�-definidos pelo programador */
typedef struct
{
	char  nome[100];
	char  pront[10];	
}reg;

/* Fun��es */
void bubble_sort(reg * vetor)
{ int i, j; 
  reg aux;
  
  for ( j = 0; j<TOTPROFS-1; j++) 
   for ( i = 0; i<TOTPROFS-1-j; i++) 
    if ( strcmp ( vetor[i].nome, vetor[i+1].nome ) == 1 ) 
    {   aux 	   = vetor[i];
	    vetor[i]   = vetor[i+1];
		vetor[i+1] = aux;
    }
}

void cadastroDefault (reg * vet)
{
	FILE * Arq;
	int i;
	
	memset(vet, '\0', sizeof(vet));
	
	strcpy (vet[0].nome , "Rodrigo Ribeiro de Oliveira");
	strcpy (vet[0].pront, "RG134168");
	strcpy (vet[1].nome , "Domingos Lucas Latorre de Oliveira");
	strcpy (vet[1].pront, "CJ146456");
	strcpy (vet[2].nome , "Leandro Pinto Santana");
	strcpy (vet[2].pront, "CP220383");
	strcpy (vet[3].nome , "Andre Luiz da Silva");
	strcpy (vet[3].pront, "SP030028");
	strcpy (vet[4].nome , "Claudia Miyuki Werhmuller");
	strcpy (vet[4].pront, "SP030041");
	strcpy (vet[5].nome , "Claudete de Oliveira Alves");
	strcpy (vet[5].pront, "SP03020X");

    bubble_sort (vet); /* ordena��o por nomes */
 
	Arq = fopen ("USUARIOS.DAT" , "w");
	if (Arq == NULL)
	{
		printf ("\nErro ao gerar USUARIOS.DAT");
		getch();
		exit(0);
	} 
    for (i=0; i<TOTPROFS; i++)
		fwrite (&vet[i], sizeof(reg), 1, Arq);
		
	fclose (Arq);
}

void mostraCadastroDefault (void)
{
	FILE * Arq;
	reg aux;
	int i=1;
	
	Arq = fopen ("USUARIOS.DAT" , "r");
	if (Arq == NULL)
	{
		printf ("\nErro ao ler USUARIOS.DAT");
		getchar();
		exit(0);
	} 
	printf ("\n# NOME\t\t\t\t\t\t\tPRONTUARIO");
	while (!feof(Arq))
	{
		fread (&aux, sizeof(aux), 1, Arq);
		if (!feof(Arq))
			printf ("\n%i [%-50s]\t[%-s]", i++, aux.nome, aux.pront );
	}
	fclose(Arq);
	getchar();
}
/* Corpo do programa */
int main ()
{
    reg  vet[TOTPROFS];	
    cadastroDefault (vet);
    mostraCadastroDefault();
	return 0;
}
