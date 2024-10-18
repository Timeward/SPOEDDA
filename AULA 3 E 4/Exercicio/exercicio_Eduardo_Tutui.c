#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Constantes
int runner = 30;
//TYPEDEFs

//Prototipo de função

//Funcoes
void swap(char *str1, char *str2){
	char temp[100];
	strcpy(temp, str1);
	strcpy(str1, str2);
	strcpy(str2, temp);
}

void bubbleSort(char names[30][100], int runner){
	for(int i = 0; i < runner - 1; i++){
		for (int j = 0; j < runner-i-1; j++){
			if(strcmp(names[j], names[j+1]) > 0){
				swap(names[j], names[j+1]);
			}
		}
	}
}

void selectionSort(char names[30][100], int runner){

}

void insertionSort(char names[30][100], int runner){

}
//Main
int main(){
	char names[30][100] = {"Domingos Lucas Latorre de Oliveira", "Leandro Pinto Santana",
	"Rodrigo Ribeiro de Oliveira", "Andre Luiz da Silva", "Claudia Miyuki Werhmuller", 
	"Claudete de Oliveira Alves", "Francisco Verissimo Luciano", "Luk Cho Man", 
	"Ivan Francolin Martinez", "Joao Vianei Tamanini", "Jose Oscar Machado Alexandre", 
	"Jose Braz de Araujo", "Paulo Roberto de Abreu", "Eurides Balbino da Silva",
	"Domingos Bernardo Gomes Santos", "Andre Evandro Lourenco", "Miguel Angelo Tancredi Molina",
	"Antonio Airton Palladino", "Luis Fernando Aires Branco Menegueti", "Antonio Ferreira Viana",
	"Leonardo Bertholdo", "Marcelo Tavares de Santana", "Wagner de Paula Gomes", "Daniel Marques Gomes de Morais",
	"Alexandre Beletti Ferreira", "Vladimir Camelo Pinto", "Leonardo Andrade Motta de Lima",
	"Aldo Marcelo Paim", "Cesar Lopes Fernandes", "Josceli Maria Tenorio"};

	printf("Selecione o método:\n1: Bubble Sort\n2: Selection Sort\n3: Insertion Sort\n\n\n");

	printf("Lista Original:\n");
	for(int i = 0; i < runner; i++){
		printf("%s\n", names[i]);
	}

	
}