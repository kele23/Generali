#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unordered_set>
#include <queue>

/**
* Definizione della struttura di Generale
*/
struct Generale { //Generale Generico ( Contenuto all'interno di un Set )

	int valore;

	bool operator == ( const Generale& g ) const
    {
        return ( valore == g.valore );
    }

};

/**
* Definizione della funzione di Hash per il Set
*/
struct GeneraleHash
{
    
    std::size_t operator () ( const Generale& g ) const
    {
        return g.valore;
    }

};

/**
* Definizione del Set
*/
typedef std::unordered_set< Generale, GeneraleHash > SetGenerali;

/**
* Item dell'array di Generali
*/
struct GeneraleItem{ //Generale contenuto all'interno dell'Array

	int colore = 0;
	SetGenerali generali;

};


struct Nodo{ //Un nodo nell'albero della gerarchia che costruiamo

	Generale *padre;
	SetGenerali *figli;

};

/**
* Ritorna vero se esiste rivalità tra g1 e g2, falso altrimenti
*/
bool checkRivalita(Generale g1, Generale g2, GeneraleItem* grafo);

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
		
		Generale g;
		g.valore = vinc;

		grafo[perd].generali.insert(g);
	}

	fclose(input);

	//ELABORAZIONE DEL RISULTATO

	for(int i= 0; i<V; i++) {
		std::queue<Generale> to_control;
		/*
		* Scorro tutti i generali che mi hanno sconfitto
		*/		
		for(Generale g: grafo[i].generali) {
			/*
			* Scorro tutti i generali che hanno sconfitto g
			*/
			for(Generale gen: grafo[g.valore].generali){
				if(gen.valore == i)
					continue;
				/*
				* Se per caso non è già un generale che mi ha sconfitto allora lo aggiungo tra chi mi disprezza 
				*/
				if(grafo[i].generali.find(gen) == grafo[i].generali.end()){
					grafo[i].generali.insert(gen);
					/*
					* Elemento è colorato? 
					* SI vuol dire che non dobbiamo andare a controllare gli elementi che contiene
					* NO vuol dire che dobbiamo andare a controllare gli elementi che contiene
					*/
					if(grafo[g.valore].colore != 1){
						to_control.push(gen);
					}
				}
			}
		}

		while (!to_control.empty()){
			Generale g = to_control.front();
			/*
			* Scorro tutti i generali che hanno sconfitto g
			*/
			for(Generale gen: grafo[g.valore].generali){
				if(gen.valore == i)
					continue;
				/*
				* Se per caso non è già un generale che mi ha sconfitto allora lo aggiungo tra chi mi disprezza 
				*/
				if(grafo[i].generali.find(gen) == grafo[i].generali.end()){
					grafo[i].generali.insert(gen);
					/*
					* Elemento è colorato? 
					* SI vuol dire che non dobbiamo andare a controllare gli elementi che contiene
					* NO vuol dire che dobbiamo andare a controllare gli elementi che contiene
					*/
					if(grafo[g.valore].colore != 1){
						to_control.push(gen);
					}
				}
			}
			to_control.pop();
  		}

  		grafo[i].colore = 1;
	}
	
	//Costruzione dell'albero
	Nodo root;
	root.padre = NULL;
	root.figli = new SetGenerali();
	
	Generale g;
	for(int i=0; i<V; i++) {
		if(grafo[i].generali.empty())
			//sicuro consigliere!
			g.valore = i;
			g.colore = 0; //forse si potrebbe mettere già inizializzato così di default?
			root.figli->insert(g);
	}
	
	//STAMPA DEL RISULTATO
	for(int i = 0; i < V ; i++){
		std::cout << "Generale: " << i << " -> ";
		for(Generale g: grafo[i].generali) {
			std::cout << g.valore << ",";
		}
		std::cout << "\n";
	}

	return 0;
}

bool checkRivalita(Generale g1, Generale g2, GeneraleItem* grafo) {
	if(grafo[g1.valore].generali.count(g2)==1 && grafo[g2.valore].generali.count(g1)==1)
		return true;
	return false;
}

