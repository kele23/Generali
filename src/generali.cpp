#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unordered_set>
#include <queue>
#include <sstream>
#include <vector>
#include <stack>

#include <chrono>
#include <ctime>

//Comandato Da
#define ROOT -1

//Colore BFS1
#define WHITE	0
#define BLACK	1	

//Colore BFS2
#define GREEN 	2 //Visitato attraverso altri nodi

/**
* Definizione del Set
*/
typedef std::unordered_set< int > SetNodi;

/**
* Item dell'array di Generali
*/
struct Generale{ //Generale contenuto all'interno dell'Array
	int indice = -1;
	int min_dist;
	SetNodi predecessori;
	SetNodi successori;
	SetNodi sottoposti;
	bool onStack = false;

	int comandato = ROOT;
};

std::vector<SetNodi> scc;
int indice = 0;
std::stack<int> s;
Generale* grafo;

void tarjan(int nodo);
bool nodoGiaPresente(int nodo, std::stack<int> st);
bool sameScc(int i, int j);
	
int main(int argc, char* argv[]){

	std::chrono::time_point<std::chrono::system_clock> start, end;
	
	int V = 0;
	int E = 0;

	//LETTURA DEL GRAFO

	start = std::chrono::system_clock::now();
	FILE *input = fopen("input.txt","r");
	
	fscanf(input,"%d %d",&V,&E);
	grafo = new Generale[V];

	int vinc,perd;
	for(int I = 0; I < E; I++){
		fscanf(input,"%d %d",&vinc,&perd);
		grafo[perd].predecessori.insert(vinc);
		grafo[vinc].successori.insert(perd);
	}

	fclose(input);
	end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end-start;

	std::cout << "Lettura: " << elapsed_seconds.count() << std::endl;

	//ELABORAZIONE DEL RISULTATO
	start = std::chrono::system_clock::now();
	
	//Identificazione SCC
	for(int i=0; i<V; i++)
		if(grafo[i].indice == -1)
			tarjan(i);
	
	end = std::chrono::system_clock::now();
	elapsed_seconds = end-start;

	std::cout << "BFS: " << elapsed_seconds.count() << std::endl;	

	//Costruzione dell'albero
	start = std::chrono::system_clock::now();
	std::stringstream ssCons;
	std::stringstream ssSottoposti;
	int numConsiglieri = 0;	

	for(int i=0; i<V; i++) {
		for(int s: grafo[i].successori){
			if(!sameScc(i,s)){
				bool ok = true;
				for(int f: grafo[i].sottoposti)
					if(sameScc(f,s))
						ok = false;
				if(ok){
					grafo[i].sottoposti.insert(s);
					grafo[s].comandato = i;
					ssSottoposti << i << " " << s << std::endl;
				}
			}
		}
	}
	end = std::chrono::system_clock::now();
	elapsed_seconds = end-start;
	std::cout << "Albero: " << elapsed_seconds.count() << std::endl;


	int consiglieri = 0;
	for(int i=0; i<V; i++) {
		
		if(grafo[i].comandato == ROOT){
			consiglieri++;
			ssCons << i << " ";
		}

		//Stampa sottoposti
		/*std::cout << i << ": " << grafo[i].comandato <<" -> ";
		for(int g: grafo[i].sottoposti){
			std::cout << g << " ";
		}
		std::cout << std::endl;*/
	}



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


void tarjan(int nodo) {
	grafo[nodo].indice = indice;
	grafo[nodo].min_dist = indice;
	indice++;
	s.push(nodo);
	grafo[nodo].onStack = true;
	int tmp;
	for(int succ: grafo[nodo].successori) {
		if(grafo[succ].indice == -1) {
			tarjan(succ);
			grafo[nodo].min_dist = std::min(grafo[nodo].min_dist, grafo[succ].min_dist);
		}
		else if(grafo[nodo].onStack) 
			grafo[nodo].min_dist = std::min(grafo[nodo].min_dist, grafo[succ].indice);
	}
	if(grafo[nodo].min_dist == grafo[nodo].indice) {
		SetNodi setN;
		
		do {
			tmp = s.top();
			grafo[tmp].onStack = false;
			setN.insert(tmp);
			s.pop();
		} while (tmp != nodo);

		scc.push_back(setN);
	}
}

bool sameScc(int i, int j){

	for(SetNodi set: scc){
		if(set.count(i) == 1 && set.count(j) == 1)
			return true;
	}
	return false;

}

