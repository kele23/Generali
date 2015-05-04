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
	SetGenerali sconfitto;

	int comandato = NONE;
	SetGenerali sottoposti;

	//Generali che mi disprezzano
	SetGenerali disprezzo;

	//Generali che ho sconfitto
	SetGenerali vincitore;

};

/**
* Ritorna vero se esiste rivalità tra g1 e g2, falso altrimenti
*/
bool checkRivalita(int g1, int g2, GeneraleItem* grafo);

void checkSottoposto(int generale,GeneraleItem* grafo);

bool puoSottoposto(int gf,int gp,GeneraleItem* grafo);

void BFS2(int i,GeneraleItem* grafo);

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
		//grafo[perd].disprezzo.insert(vinc); //SOLO BFS NO BFS2
		grafo[vinc].vincitore.insert(perd);
	}

	fclose(input);
	end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end-start;

	std::cout << "Lettura: " << elapsed_seconds.count() << std::endl;

	//ELABORAZIONE DEL RISULTATO
	start = std::chrono::system_clock::now();
	for(int i=0; i<V; i++){
		BFS2(i,grafo);
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

	
	//Costruzione dell'albero
	start = std::chrono::system_clock::now();
	std::stringstream ssCons;
	std::stringstream ssSottoposti;
	int consiglieri = 0;	

	for(int i=0; i<V; i++) {
		//L'elemento è già stato aggiunto nell'albero?
		if(grafo[i].comandato != NONE)
			continue;
		checkSottoposto(i,grafo);

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

bool checkRivalita(int g1, int g2, GeneraleItem* grafo) {
	if(grafo[g1].disprezzo.count(g2)==1 && grafo[g2].disprezzo.count(g1)==1)
		return true;
	return false;
}

bool puoSottoposto(int gf,int gp,GeneraleItem* grafo){

	//Se per caso si odiano a vicenda NO! SECONDA LEGGE
	if(grafo[gp].disprezzo.count(gf) == 1) 		
		return false;

	//potrebbe essere figlio.. controllare i fratelli
	bool ok = true;	 //Se non ha fratelli OK! //TERZA LEGGE
	for(int g: grafo[gp].sottoposti){	
		ok = checkRivalita(g,gf,grafo) ? false : ok;
	}

	return ok;
}

void checkSottoposto(int generale,GeneraleItem* grafo){
	for(int g: grafo[generale].sconfitto){
		if(puoSottoposto(generale,g,grafo)){
			grafo[generale].comandato = g;
			grafo[g].sottoposti.insert(generale);
			return;
		}
	}
	grafo[generale].comandato = ROOT;
	return;
}

void BFS2(int i,GeneraleItem* grafo){
	
	if(grafo[i].colore == BLACK) //Elemento già completato
		return;

	std::queue<int> to_control;

	/*
	* Scorro tutti i generali che mi hanno sconfitto
	*/		
	for(int g: grafo[i].sconfitto) {

		if(grafo[i].colore == GREEN && grafo[i].disprezzo.count(g) == 1)
			continue;
		
		grafo[i].disprezzo.insert(g);

		if(grafo[g].colore == BLACK){
			for(int gen: grafo[g].disprezzo){
				grafo[i].disprezzo.insert(gen);
			}
		}else{
			for(int gen: grafo[g].sconfitto){
				/*
				* Se per caso non è già un generale che mi ha sconfitto allora lo aggiungo tra chi mi disprezza 
				*/
				if(grafo[i].disprezzo.find(gen) == grafo[i].disprezzo.end()){
					grafo[i].disprezzo.insert(gen);
					to_control.push(gen);
				}
			}
		}
	}

	while (!to_control.empty()){
		int g = to_control.front();

		if(grafo[g].colore == BLACK){
			for(int gen: grafo[g].disprezzo){
				grafo[i].disprezzo.insert(gen);
			}
		}else{
			for(int gen: grafo[g].sconfitto){
				/*
				* Se per caso non è già un generale che mi ha sconfitto allora lo aggiungo tra chi mi disprezza 
				*/
				if(grafo[i].disprezzo.find(gen) == grafo[i].disprezzo.end()){
					grafo[i].disprezzo.insert(gen);
					to_control.push(gen);
				}
			}
		}

		to_control.pop();
	}

	grafo[i].colore = BLACK;

	//Seconda fase BFS ( DFS )
	for(int g: grafo[i].vincitore) {

		if(grafo[g].colore == BLACK)
			continue;

		grafo[g].disprezzo.insert(i);
		for(int d: grafo[i].disprezzo){

			if(d != g && grafo[g].disprezzo.find(d) == grafo[g].disprezzo.end()){
				grafo[g].disprezzo.insert(d);
			}

		}

		grafo[g].colore = GREEN;

		BFS2(g,grafo);
	}
}

