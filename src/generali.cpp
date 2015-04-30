#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unordered_set>

/**
* Definizione della struttura di Generale
*/
struct Generale{

	int valore;
	int colore;

	bool operator == ( const Generale& g ) const
    {
        return ( valore == g.valore );
    }

};

struct GeneraleHash
{
    
    std::size_t operator () ( const Generale& g ) const
    {
        return g.valore;
    }

};

typedef std::unordered_set< Generale, GeneraleHash > SetGenerali;



int main(int argc, char* argv[]){
	
	int V = 0;
	int E = 0;
	SetGenerali* grafo;


	//LETTURA DEL GRAFO
	FILE *input = fopen("input.txt","r");
	
	fscanf(input,"%d %d",&V,&E);
	grafo = new SetGenerali[V];

	int vinc,perd;
	for(int I = 0; I < E; I++){
		fscanf(input,"%d %d",&vinc,&perd);
		
		Generale g;
		g.valore = vinc;
		g.colore = 0;
		
		grafo[perd].insert(g);
	}

	fclose(input);

	//ELABORAZIONE DEL RISULTATO
	

	//STAMPA DEL RISULTATO


	return 0;
}
