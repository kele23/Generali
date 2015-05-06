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

/**
* Item dell'array di Generali
*/
struct GeneraleItem{ //Generale contenuto all'interno dell'Array
	int colore = WHITE;
	SetGenerali figli;
	int gradoE;
	SetGenerali sottoposti;
};

void visita(int v, GeneraleItem* grafo, stack path);

bool ha1elementoComune(SetGenerali a, SetGenerali b);

void merge(SetGenerali a, SetGenerali b);


int main(int argc, char* argv[]){

	std::chrono::time_point<std::chrono::system_clock> start, end;
	
	int V = 0;
	int E = 0;
	GeneraleItem* grafo;
	SetGenerali consiglieri;
	
	vector<SetGenerali> cicli;


	//LETTURA DEL GRAFO

	start = std::chrono::system_clock::now();
	FILE *input = fopen("input.txt","r");
	
	fscanf(input,"%d %d",&V,&E);
	grafo = new GeneraleItem[V];

	int vinc,perd;
	for(int I = 0; I < E; I++){
		fscanf(input,"%d %d",&vinc,&perd);
		grafo[vinc].figli.insert(perd);
		grafo[vinc].gradoE++;
	}

	fclose(input);
	end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end-start;

	std::cout << "Lettura: " << elapsed_seconds.count() << std::endl;

	//ELABORAZIONE DEL RISULTATO
	start = std::chrono::system_clock::now();
	
	// Chi non ha archi entranti è consigliere
	for(int i=0; i<V; i++){
		if(grafo[i].gradoE == 0){
			consiglieri.insert(i);					
		}
	}
	
	end = std::chrono::system_clock::now();
	elapsed_seconds = end-start;

	std::cout << "BFS: " << elapsed_seconds.count() << std::endl;

	/*for(int i=0; i<V; i++){
		std::cout << i << " -> ";
		for(int g: grafo[i].disprezzo){
			std::cout << g << " ";
		}
		std::cout << std::endl;
	}*/

	for(int v: consiglieri){
	
		stack<int> path;
		
		grafo[v].colore = GREEN; //marco il consigliere iniziale
		visita(v, grafo, path);
		
		for(int i=0; i<V; i++){
		if(grafo[i].gradoE == 0){
			consiglieri.insert(i);					
		}
	}
	
	
	}















	
	//Costruzione dell'albero
	start = std::chrono::system_clock::now();
	std::stringstream ssCons;
	std::stringstream ssSottoposti;
	int consiglieri = 0;	

	for(int i=0; i<V; i++) {
		//L'elemento è già stato aggiunto nell'albero?
		if(grafo[i].comandato == NONE){
			checkSottoposto(i,grafo);
		}

		if(grafo[i].comandato == -1){
			ssCons << i << " ";
			consiglieri++;
		}else{
			ssSottoposti << grafo[i].comandato << " " << i << "\n";
		}
	}
	end = std::chrono::system_clock::now();
	elapsed_seconds = end-start;

	std::cout << "Albero: " << elapsed_seconds.count() << std::endl;

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


void visita(int v, GeneraleItem* grafo, stack path){

	if(grafo[v].colore != GREEN){ return; }

	for(int f: grafo[v].figli){
		if(f.colore == BLACK) continue;
		if(f.colore == GREEN) { // c'è un ciclo!
			int q;
			int p=v;
			SetGenerali cicloNuovo;
			cicloNuovo.insert(v);
			do {
				q=path.pop();
				cicloNuovo.insert(q);
				grafo[p].color=WHITE;
				grafo[q].sottoposti.erase(p);
				for(int figlio:grafo[p].figli){
					grafo[figlio].gradoE++;	
				}
				p=q;
			}
			while(q!=f);
			
			for(SetGenerali c: cicli){
				if (hanno1elementoComune(c, cicloNuovo)){
					merge(c, cicloNuovo);
				}
				else cicli.insert(cicloNuovo);
			}
			
		}
		if(f.colore == WHITE) { //può essere mio figlio
			// controllo se non è in un ciclo con v
			// controllo se non è in un ciclo con gli altri figli già assegnati di v --TERZA LEGGE
			bool canBeS=true;
			
			if(areInCicle(f,v)) continue;
			for(int s:grafo[v].sottoposti)
				if(areInCicle(s,f)) {canBeS=false; break;}
			
			if (canBeS){
				f.colore = GREEN;
				grafo[v].sottoposti.insert(f);
				
				for(int figlio:grafo[f].figli){
					grafo[figlio].gradoE--;	
				}
				
				path.push(v);
				visita(f, grafo, path);
			}
			}
		}
	
	}

	grafo[v].colore = BLACK;


}


bool ha1elementoComune(SetGenerali a, SetGenerali b){

return true;


}

void merge(SetGenerali a, SetGenerali b){


}
