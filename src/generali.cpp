#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unordered_set>

int main(int argc, char* argv[]){
	
	int V = 0;
	int E = 0;
	std::unordered_set<int>* graph;


	std:unordered_set<int>* generali; 

	//LETTURA DEL GRAFO
	FILE *input = fopen("input.txt","r");
	
	fscanf(input,"%d %d",&V,&E);
	graph = new std::unordered_set<int>[V];

	int vinc,perd;
	for(int I = 0; I < E; I++){
		fscanf(input,"%d %d",&vinc,&perd);
		graph[perd].insert(vinc);
	}

	fclose(input);

	//ELABORAZIONE DEL RISULTATO
	

	//STAMPA DEL RISULTATO


	return 0;
}
