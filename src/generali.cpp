#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unordered_set>
#include <queue>
#include <sstream>

#include <chrono>
#include <ctime>

//Comandato Da
#define NONE -2
#define ROOT -1

//Colore BFS1
#define WHITE	0
#define BLACK	1	

//Colore BFS2
#define GREEN 	2 //Visitato attraverso altri nodi

/**
* Definizione del Set
*/
typedef std::unordered_set< int > SetGenerali;

typedef struct _Ritorna {
	
	int comandato = NONE;
	SetGenerali* ciclo = NULL;

}Ritorna;
/**
* Item dell'array di Generali
*/
struct GeneraleItem{ //Generale contenuto all'interno dell'Array

	int colore = WHITE;
	int comandato = NONE;

	SetGenerali sconfitto;
	SetGenerali vincitore;
	SetGenerali sottoposti;

	SetGenerali* ciclo = NULL;

};

/**
* Ritorna vero se esiste rivalità tra g1 e g2, falso altrimenti
*/

Ritorna DFS(int i,GeneraleItem* grafo);

void rimuoviPerdenti(int p,GeneraleItem* grafo);

int main(int argc, char* argv[]){

	std::chrono::time_point<std::chrono::system_clock> start, end;
	
	int V = 0;
	int E = 0;
	GeneraleItem* grafo;

	//LETTURA DEL GRAFO

	start = std::chrono::system_clock::now();
	FILE *input = fopen("input.txt","r");
	
	fscanf(input,"%d %d",&V,&E);
	grafo = new GeneraleItem[V];

	int vinc,perd;
	for(int I = 0; I < E; I++){
		fscanf(input,"%d %d",&vinc,&perd);
		grafo[perd].sconfitto.insert(vinc);
		grafo[vinc].vincitore.insert(perd);
	}

	fclose(input);
	end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end-start;

	std::cout << "Lettura: " << elapsed_seconds.count() << std::endl;	

	start = std::chrono::system_clock::now();
	for(int i = 0; i < V; i++){
		if(grafo[i].vincitore.empty())
			rimuoviPerdenti(i,grafo);
	}
	end = std::chrono::system_clock::now();
	elapsed_seconds = end-start;

	std::cout << "Rimozione Perdenti: " << elapsed_seconds.count() << std::endl;

	//ELABORAZIONE DEL RISULTATO
	start = std::chrono::system_clock::now();
	for(int i=0; i<V; i++){
		DFS(i,grafo);
	}

	end = std::chrono::system_clock::now();
	elapsed_seconds = end-start;

	std::cout << "DFS: " << elapsed_seconds.count() << std::endl;

	//Creazione Albero
	std::stringstream ssCons;
	std::stringstream ssSottoposti;
	int consiglieri = 0;

	for(int i = 0; i < V; i++){

		if(grafo[i].comandato == NONE){
			grafo[i].comandato = ROOT;
		}

		if(grafo[i].comandato == ROOT){
			consiglieri++;
			ssCons << i << " ";
		}else{
			ssSottoposti<< grafo[i].comandato << " " << i << std::endl;
		}
	}

	//OUTPUT
	start = std::chrono::system_clock::now();
	FILE* output = fopen("output.txt","w");

	fprintf(output,"%d\n",consiglieri);
	fprintf(output,"%s\n",ssCons.str().c_str());
	fprintf(output,"%s\n",ssSottoposti.str().c_str());
	
	fclose(output);
	end = std::chrono::system_clock::now();
	elapsed_seconds = end-start;

	std::cout << "Scrittura: " << elapsed_seconds.count() << std::endl;

	return 0;
}

SetGenerali* merge(SetGenerali* cicli[],int size,GeneraleItem* grafo){

	SetGenerali* ciclo = NULL;
	for(int i = 0; i < size; i++){
		if(cicli[i] == NULL)
			continue;
		if(ciclo == NULL){
			ciclo = cicli[i];
		}else{
			for(int c : *cicli[i]){
				grafo[c].ciclo = ciclo;
				ciclo->insert(c);
			}
			//free(cicli[i]);
		}
	}
	return ciclo;
}

Ritorna DFS(int i,GeneraleItem* grafo){

	Ritorna r;

	if(grafo[i].colore == BLACK){
		if(grafo[i].ciclo == NULL){
			r.comandato = i;
		}
		r.ciclo = grafo[i].ciclo;

		//std::cout << "b: "<< i << std::endl;
		return r;
	}

	if(grafo[i].colore == GREEN){
		r.ciclo = new SetGenerali();
		r.ciclo->insert(i);

		//std::cout << "g: "<< i << std::endl;
		return r;
	}

	if(grafo[i].sconfitto.empty()){
		r.comandato = i;
		r.ciclo = NULL;

		grafo[i].colore = BLACK;
		grafo[i].comandato = ROOT;

		//std::cout << "t: "<< i << std::endl;
		return r;
	}

	//std::cout << "w: "<< i << std::endl;
	grafo[i].colore = GREEN;

	SetGenerali ppPadri;
	SetGenerali *cicli[grafo[i].sconfitto.size()];
	int pos = 0;
	for(int v:grafo[i].sconfitto){
		//std::cout << i << "->" << v << std::endl;
		Ritorna ret = DFS(v,grafo);
		//std::cout << "R" << i << std::endl;
		if(ret.comandato != NONE){
			ppPadri.insert(ret.comandato);
		}
		cicli[pos++] = ret.ciclo;
	}

	grafo[i].ciclo = merge(cicli,grafo[i].sconfitto.size(),grafo);
	r.ciclo = grafo[i].ciclo;

	if(grafo[i].ciclo != NULL){
		r.ciclo->insert(i);
	}

	for(int p: ppPadri){
		bool fratello = true;
		for(int f: grafo[p].sottoposti){
			if(grafo[i].ciclo->count(f) == 1){
				fratello = false;
				break;
			}
		}
		if(fratello){
			grafo[i].comandato = p;
			grafo[p].sottoposti.insert(i);
		}
	}

	if(grafo[i].comandato != NONE)
		r.comandato = grafo[i].comandato;

	grafo[i].colore = BLACK;

	return r;

}

void rimuoviPerdenti(int p,GeneraleItem* grafo){

	grafo[p].colore = BLACK;
	if(grafo[p].sconfitto.empty())
		grafo[p].comandato = ROOT;
	else
		grafo[p].comandato = *(grafo[p].sconfitto.begin());

	for(int v: grafo[p].sconfitto){
		grafo[v].vincitore.erase(p);

		if(grafo[v].vincitore.empty())
			rimuoviPerdenti(v,grafo);
	}

}