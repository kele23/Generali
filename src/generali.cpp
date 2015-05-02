#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unordered_set>
#include <queue>
#include <sstream>

//Comandato Da
#define NONE -2
#define ROOT -1

//Colore
#define NOT_VISITED 0
#define VISITED 1


/**
* Definizione del Set
*/
typedef std::unordered_set< int > SetGenerali;

/**
* Item dell'array di Generali
*/
struct GeneraleItem{ //Generale contenuto all'interno dell'Array

	int colore = NOT_VISITED;
	SetGenerali sconfitto;

	int comandato = NONE;
	SetGenerali sottoposti;

	//Generali che mi disprezzano
	SetGenerali disprezzo;

};

/**
* Ritorna vero se esiste rivalità tra g1 e g2, falso altrimenti
*/
bool checkRivalita(int g1, int g2, GeneraleItem* grafo);

void checkSottoposto(int generale,GeneraleItem* grafo);

bool puoSottoposto(int gf,int gp,GeneraleItem* grafo);

int main(int argc, char* argv[]){
	
	int V = 0;
	int E = 0;
	GeneraleItem* grafo;


	//LETTURA DEL GRAFO
	FILE *input = fopen("input.txt","r");
	
	fscanf(input,"%d %d",&V,&E);
	grafo = new GeneraleItem[V];

	int vinc,perd;
	for(int I = 0; I < E; I++){
		fscanf(input,"%d %d",&vinc,&perd);
		grafo[perd].sconfitto.insert(vinc);
		grafo[perd].disprezzo.insert(vinc);
	}

	fclose(input);

	//ELABORAZIONE DEL RISULTATO

	for(int i= 0; i<V; i++) {
		std::queue<int> to_control;

		/*
		* Scorro tutti i generali che mi hanno sconfitto
		*/		
		for(int g: grafo[i].disprezzo) {
			/*
			* Scorro tutti i generali che hanno sconfitto g
			*/
			for(int gen: grafo[g].disprezzo){
				if(gen == i)
					continue;
				/*
				* Se per caso non è già un generale che mi ha sconfitto allora lo aggiungo tra chi mi disprezza 
				*/
				if(grafo[i].disprezzo.find(gen) == grafo[i].disprezzo.end()){
					grafo[i].disprezzo.insert(gen);
					/*
					* Elemento è colorato? 
					* SI vuol dire che non dobbiamo andare a controllare gli elementi che contiene
					* NO vuol dire che dobbiamo andare a controllare gli elementi che contiene
					*/
					if(grafo[g].colore != 1){
						to_control.push(gen);
					}
				}
			}
		}

		while (!to_control.empty()){
			int g = to_control.front();

			for(int gen: grafo[g].disprezzo){
				if(gen == i)
					continue;
				
				if(grafo[i].disprezzo.find(gen) == grafo[i].disprezzo.end()){
					grafo[i].disprezzo.insert(gen);

					if(grafo[g].colore != 1){
						to_control.push(gen);
					}
				}
			}
			to_control.pop();
  		}

  		grafo[i].colore = 1;
	}
	
	//Costruzione dell'albero

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

	FILE* output = fopen("output.txt","w");

	fprintf(output,"%d\n",consiglieri);
	fprintf(output,"%s\n",ssCons.str().c_str());
	fprintf(output,"%s\n",ssSottoposti.str().c_str());
	
	fclose(output);

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

