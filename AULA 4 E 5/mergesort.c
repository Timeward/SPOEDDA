/* Bibliotecas */
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <locale.h>

/* Prot�tipos de fun��es */
void merge_sort        (float * vetor, int inicio, int fim);
void merge             (float * vetor, int inicio, int meio, int fim);
void mostra_notas      (float * notas);
void abastece_notas    (float *notas);
void define_qtd_notas  (void);
void mostra_subvetor   (float *v, int inicio, int fim);
int  main              (void);

/* vari�veis globais */
int     TAMANHO_VETOR;

/* Fun��es */

/* Observa��es para implementar o merge_sort:

I. A primeira etapa consiste no seguinte algoritmo

01. mergesort(A[0...n - 1], inicio, fim)
02. |   se(inicio < fim)
03. |   |   meio <- (inicio + fim) / 2 //calcula o meio
04. |   |   mergesort(A, inicio, meio) //ordena o subvetor esquerdo
05. |   |   mergesort(A, meio + 1, fim) //ordena o subvetor direito
06. |   |   merge(A, inicio, meio, fim) //funde os subvetores esquerdo e direito
07. |   fim_se
08. fim_mergesort

*/

void mostra_subvetor (float *v, int inicio, int fim) 
{
    int i;
    printf ("\n------------ SUBVETOR -------------\n");
    for (i=inicio; i<fim+1; i++)
       printf ("%3i=[%5.2f]", i, v[i] );
    printf ("\n--------------------------------\n");
    getch();
}

void merge_sort (float * vetor, int inicio, int fim) 
{
	int  meio;
	if ( inicio < fim )
	{
		meio = (inicio + fim) / 2;         /* calcula o meio                         */
	    printf ("\ninicio = %i\tmeio=%i\tfim=%i[vai ordenar o esquerdo]", inicio, meio, fim);
	    mostra_subvetor(vetor, inicio, fim);
		merge_sort (vetor, inicio, meio);  /* ordena o subvetor esquerdo             */
		printf ("\ninicio = %i\tmeio=%i\tfim=%i[vai ordenar o direito]", inicio, meio, fim);
		mostra_subvetor(vetor, inicio, fim);
		merge_sort (vetor, meio+1, fim );  /* ordena o subvetor direito              */
		printf ("\ninicio = %i\tmeio=%i\tfim=%i[vai fundir]", inicio, meio, fim);
		merge (vetor, inicio, meio, fim);  /* funde os subvetores esquerdo e direito */
		mostra_subvetor(vetor, inicio, fim);
	}
}


/*
A etapa de fus�o � a seguinte:

01. merge(A[0...n - 1], inicio, meio, fim)
02. |   tamEsq <- meio - inicio + 1 //tamanho do subvetor esquerdo
03. |   tamDir <- fim - meio //tamanho do subvetor direito
04. |   inicializar vetor Esq[0...tamEsq - 1]
05. |   inicializar vetor Dir[0...tamDir - 1]
06. |   para i <- 0 at� tamEsq - 1
07. |   |   Esq[i] <- A[inicio + i] //elementos do subvetor esquerdo
08. |   fim_para
09. |   para j <- 0 at� tamDir - 1 
10. |   |   Dir[j] <- A[meio + 1 + j] //elementos do subvetor direito
11. |   fim_para
12. |   idxEsq <- 0 //�ndice do subvetor auxiliar esquerdo
13. |   idxDir <- 0 //�ndice do subvetor auxiliar direito
14. |   para k <- inicio at� fim
15. |   |   se(idxEsq < tamEsq)
16. |   |   |   se(idxDir < tamDir)
17. |   |   |   |   se(Esq[idxEsq] < Dir[idxDir])
18. |   |   |   |   |   A[k]   <- Esq[idxEsq]
19. |   |   |   |   |   idxEsq <- idxEsq + 1
20. |   |   |   |   sen�o
21. |   |   |   |   |   A[k]   <- Dir[idxDir]
22. |   |   |   |   |   idxDir <- idxDir + 1
23. |   |   |   |   fim_se
24. |   |   |   sen�o
25. |   |   |   |   A[k]   <- Esq[idxEsq]
26. |   |   |   |   idxEsq <- idxEsq + 1
27. |   |   |   fim_se
28. |   |   sen�o
29. |   |   |   A[k]   <- Dir[idxDir]
30. |   |   |   idxDir <- idxDir + 1
31. |   |   fim_se
32. |   fim_para
33. fim_merge
*/

void merge (float * vetor, int inicio, int meio, int fim)
{
	int tamEsq, tamDir, i, idxEsq, idxDir;
    float *vetor_Esq;
    float *vetor_Dir;
    
	tamEsq = meio - inicio + 1; /* tamanho do subvetor esquerdo */
	tamDir = fim  - meio;       /* tamanho do subvetor direito  */
	
    /* Tenta alocar tamEsq posi��es para abastecer o subvetor esquerdo */   
    vetor_Esq = (float *) malloc (tamEsq * sizeof (float)); 
    if (vetor_Esq == NULL)
    {
        printf ("malloc devolveu NULL!\n");
        getch();
        exit (EXIT_FAILURE);
    }        

    /* Tenta alocar tamDir posi��es para abastecer o subvetor direito */   
    vetor_Dir = (float *) malloc (tamDir * sizeof (float)); 
    if (vetor_Dir == NULL)
    {
        printf ("malloc devolveu NULL!\n");
        getch();
        exit (EXIT_FAILURE);
    }   
	
    /* inicializar vetor_Esq */
    for (i=0; i<tamEsq; i++)
       vetor_Esq[i] = vetor[inicio+i]; /* elementos do subvetor esquerdo */

    /* inicializar vetor_Dir */
    for (i=0; i<tamDir; i++)
       vetor_Dir[i] = vetor[meio+1+i]; /* elementos do subvetor direito */

    idxEsq = 0; /* �ndice do subvetor auxiliar esquerdo */
    idxDir = 0; /* �ndice do subvetor auxiliar direito  */
    
	/* intercala os vetores */
    for ( i=inicio; i<=fim; i++ )
       if (idxEsq < tamEsq)
            if (idxDir < tamDir)
                if (vetor_Esq[idxEsq] < vetor_Dir[idxDir])
                	vetor[i] = vetor_Esq[idxEsq++];
				else
			        vetor[i] = vetor_Dir[idxDir++];
			else
				vetor[i] = vetor_Esq[idxEsq++];
		else
			vetor[i] = vetor_Dir[idxDir++];
			/*
    printf ("\n[vetor esquerdo]"); 
	for (i=0; i<tamEsq; i++)
	   printf ("[%.2f]", vetor_Esq[i]);
    printf ("\n[vetor direito ]"); 
    for (i=0; i<tamDir; i++)
       printf ("[%.2f]", vetor_Dir[i]); */
}

void mostra_notas (float *notas)
{
    int i;
    printf ("\n------------ NOTAS -------------\n");
    for (i=0; i<TAMANHO_VETOR; i++)
       printf ("%3i=%5.2f ", i, notas[i] );
    printf ("\n--------------------------------\n");
    getch();
}

void    abastece_notas (float *notas)
{  int i;
   for (i=0; i<TAMANHO_VETOR; i++)
      do 
      {
         notas[i] = rand()%11 + rand()%10*0.23;
      }
      while ( notas[i] > 10 );
}

void define_qtd_notas(void)
{
   system ("cls");
   printf ("\n------------ NOTAS -------------\n");
   printf ("\nQuantidade de notas: "); fflush(stdin); scanf ("%i", &TAMANHO_VETOR);
}

/* CORPO DO PROGRAMA */
int main(void)
{
   float   * vet_notas;
   char      resp;
   setlocale (LC_ALL,"");
   system ("cls"); system ("mode 100,100");
   do
   {    system ("color 60");
        define_qtd_notas();        /* Define o tamanho do vetor             */
        /* Tenta alocar TAMANHO_VETOR posi��es para abastecer o vetor com as notas    */   
        vet_notas = (float *) malloc (TAMANHO_VETOR * sizeof (float)); 
        if (vet_notas == NULL)
        {
            printf ("malloc devolveu NULL!\n");
            getch();
            exit (EXIT_FAILURE);
        }        
        abastece_notas(vet_notas);  /* abastece o vetor com valores aleat�rios*/
		
		printf("\n------- notas ANTES do merge_sort --------\n");
        mostra_notas(vet_notas);    /* exibe os valores das notas             */
        
		/* ordena��o crescente por merge_sort     */
        merge_sort(vet_notas,0,TAMANHO_VETOR-1 );      
        
		printf("\n------- notas DEPOIS do merge_sort --------\n");
		mostra_notas(vet_notas);    /* exibe os valores das notas ordenadas   */
        
		free(vet_notas);            /* libera mem�ria alocada                 */
        /* Testa outra vez? */
        printf ("\nNovo teste [n/N] = n�o"); fflush (stdin); resp = getche();
    }
    while ( resp != 'n' && resp != 'N' );
    return 0;
}
