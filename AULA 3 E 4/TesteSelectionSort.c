/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Constantes */
#define  TAMANHO_VETOR  1000

/* Fun��es */
void    abastece_notas (float *notas)
{  int i, inteiro; float decimal;
   /* 
   srand(time(NULL)) objetiva inicializar o gerador de n�meros aleat�rios 
   com o valor da fun��o time(NULL). 
   Este por sua vez, � calculado como sendo o total de segundos passados 
   desde 1 de janeiro de 1970 at� a data atual.
   Desta forma, a cada execu��o o valor da "semente" ser� diferente.
  */
   srand(time(NULL));
   for (i=0; i<TAMANHO_VETOR; i++)
   {
  	 	do
   	 	{
   			inteiro  = rand()%11;
   			decimal  = rand()%100;
   			notas[i] = inteiro + decimal*0.01;
   		}while ( notas[i] > 10 );
   }
}

void    mostra_vetor (float *notas)
{   unsigned long long i;
    FILE * Arq;
    Arq = fopen ("RELAT.TXT", "w");
    if (Arq == NULL)
    { 	printf ("Erro ao criar RELAT.TXT");
    	getchar();
    	exit(0);
	}
    fprintf (Arq, "\nNotas do vetor:\n");
	for ( i=0; i<TAMANHO_VETOR; i++)
	    fprintf (Arq, "\n%5llu = [%5.2f]", i+1, notas[i]);
	fclose (Arq);
	system ("notepad RELAT.TXT");
}

void bubble_sort(float * vetor) 
{ int i, j; float aux;
  for ( j = 0; j<TAMANHO_VETOR-1; j++) 
  { for ( i = 0; i<TAMANHO_VETOR-1-j; i++) 
    { if ( vetor[i] > vetor[i+1])
      { aux 	   = vetor[i];
	    vetor[i]   = vetor[i+1];
		vetor[i+1] = aux;
      }
    }
   }
}

void selection_sort (float * vetor) 
{
    int 	pos_min, i, j;
    float 	aux;
    for(i=0; i < TAMANHO_VETOR-1; i++) 
    {
        pos_min = i; 
        for (j=i+1; j < TAMANHO_VETOR; j++) 
            if (vetor[j] < vetor[pos_min]) 
                pos_min = j; 
        if (pos_min != i) 
        {
            aux            = vetor[i];
            vetor[i]       = vetor[pos_min];
            vetor[pos_min] = aux;
        }
    }
}

void insertion_sort(float * vetor){
    float escolhido;
    int   anterior, i;
    for (i = 1; i < TAMANHO_VETOR; i++) /* começa do 2º elemento */
    {	escolhido = vetor[i];
	anterior  = i - 1;
	/* Vai puxando os menores que o escolhido até encontrar
             um lugar para inseri-lo*/
         while ( (anterior >= 0) && (vetor[anterior] > escolhido) ) 
	{
	    vetor[anterior + 1] = vetor[anterior];
	    anterior--;
	}
	vetor[anterior + 1] = escolhido;
    }
}

void testaBubbleSort (float * vetor) 
{
	mostra_vetor (vetor);
	bubble_sort (vetor);
	mostra_vetor (vetor);	
}

void testaSelectionSort (float * vetor) 
{
	mostra_vetor (vetor);
	selection_sort (vetor);
	mostra_vetor (vetor);	
}

void testaInsertionSort (float * vetor) 
{
	mostra_vetor (vetor);
	insertion_sort (vetor);
	mostra_vetor (vetor);	
}

/* Corpo do programa */
int main ()
{
	float  vet[TAMANHO_VETOR];
	abastece_notas(vet);	
	testaInsertionSort(vet);
	return 0;
}


