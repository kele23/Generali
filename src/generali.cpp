#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unordered_set>

/**
* Definizione della struttura di Generale
*/
struct Generale{ //Generale Generico ( Contenuto all'interno di un Set )

	int valore;
	int colore;

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
* Item Dell'array di Generali
*/
struct GeneraleItem{ //Generale contenuto all'interno dell'Array

	int colore = 0;
	SetGenerali generali;

};



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
		g.colore = 0;

		grafo[perd].generali.insert(g);
	}

	fclose(input);

	//ELABORAZIONE DEL RISULTATO
	

	//STAMPA DEL RISULTATO


	return 0;
}
