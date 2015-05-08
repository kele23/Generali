#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <queue>
#include <sstream>
#include <vector>
#include <stack>

#include <chrono>
#include <ctime>

//Comandato Da
#define ROOT -1
#define NONE -2

#define PRINT 


typedef struct _LNodo{
	int n;
	struct _LNodo* next;
}LNodo;

class Stack{

private:
	LNodo* top;

public:
	Stack(){
		top = NULL;
	};

	void push(int n){
		LNodo* nodo = new LNodo();
		nodo->n = n;
		nodo->next = this->top;
		this->top = nodo;
	};

	int pop(){
		int n = top->n;
		top = top->next;
		return n;
	};
};

/**
* Item dell'array di Generali
*/
struct Generale{ //Generale contenuto all'interno dell'Array
	//Tarjan
	int indice = -1;
	int min_dist;
	int distHK = 0; //Distanza per Hopcroft-Karp
	bool onStack = false;

	//Grafo
	LNodo* predecessori = NULL;
	LNodo* successori = NULL;

	//Ciclo
	int scc = NONE;

	//Albero
	int comandato = NONE;
	
	//HK
	int pairU = NONE;
	int pairV = NONE;
};

std::vector<LNodo*> scc;
int indice = 0;
Stack s;
Generale* grafo;
int currentScc = 0;

void tarjan(int nodo);
void hopcroftKarp(LNodo* comp);
bool BFS(LNodo* comp);
bool DFS(int nodo);

#ifdef PRINT
void print(int V){

	for(int i = 0; i < V; i++) {
		std::cout << i << " scc: " << grafo[i].scc << " cmd: " << grafo[i].comandato <<  " -> ";
		
		LNodo* current = grafo[i].successori;
		while( current != NULL)  {
			int succ = current->n;
			std::cout << succ << " ";
			current = current->next;
		}
		std::cout << "    <- ";
		current = grafo[i].predecessori;
		while( current != NULL)  {
			int succ = current->n;
			std::cout << succ << " ";
			current = current->next;
		}
		std::cout << std::endl;
	}

}
#endif
	
int main(int argc, char* argv[]){

	std::chrono::time_point<std::chrono::system_clock> start, end;
	
	int V = 0;
	int E = 0;
	LNodo* current;

	




	/**********************************************************
	*****LETTURA********/
	start = std::chrono::system_clock::now();
	FILE *input = fopen("input.txt","r");
	
	fscanf(input,"%d %d",&V,&E);
	grafo = new Generale[V];

	for(int I = 0; I < E; I++){
		int vinc,perd;
		fscanf(input,"%d %d",&vinc,&perd);
		
		LNodo* pv = new LNodo;
			pv->n = vinc;
			pv->next = grafo[perd].predecessori;
			grafo[perd].predecessori = pv;

		LNodo* vp = new LNodo;
			vp->n = perd;
			vp->next = grafo[vinc].successori;
			grafo[vinc].successori = vp;
	}

	fclose(input);
	end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end-start;
	//std::cout << "Lettura: " << elapsed_seconds.count() << std::endl;
	/**********************************************************
	*****LETTURA********/




	/**********************************************************
	*****TARJAN********/
	start = std::chrono::system_clock::now();
	//Identificazione SCC
	for(int i=0; i<V; i++)
		if(grafo[i].indice == -1)
			tarjan(i);
	
	end = std::chrono::system_clock::now();
	elapsed_seconds = end-start;
	//std::cout << "Tarjan: " << elapsed_seconds.count() << std::endl;	
	/**********************************************************
	*****TARJAN********/




	/**********************************************************
	*****SCC RIMOZIONE ARCHI********/
	start = std::chrono::system_clock::now();	
	for(int i = 0; i < V; i++) {
		if(grafo[i].scc == NONE)
			continue;

		LNodo* pre = NULL;
		current = grafo[i].successori;
		while( current != NULL)  {
			int succ = current->n;

			bool modified = false;
			//std::cout<< i << " scc: " << grafo[i].scc << " -- " << succ << " scc: " << grafo[succ].scc <<std::endl;
			if(grafo[i].scc == grafo[succ].scc){
				//std::cout << "       removed" << std::endl;
				if(pre == NULL){
					grafo[i].successori = current->next; //FORSE RIMUOVERE
				}
				else{
					pre->next = current->next;
				}
				modified = true;
			}

			if(!modified)
				pre = current;
			current = current->next;
		}

		pre = NULL;
		current = grafo[i].predecessori;
		while( current != NULL)  {
			int succ = current->n;

			bool modified = false;
			//std::cout<< i << " scc: " << grafo[i].scc << " -- " << succ << " scc: " << grafo[succ].scc <<std::endl;
			if(grafo[i].scc == grafo[succ].scc){
				//std::cout << "       removed" << std::endl;
				if(pre == NULL){
					grafo[i].predecessori = current->next;
				}
				else{
					pre->next = current->next;
				}
				modified = true;
			}

			if(!modified)
				pre = current;
			current = current->next;
		}

	}
	end = std::chrono::system_clock::now();
	elapsed_seconds = end-start;
	//std::cout << "Rimozione Archi: " << elapsed_seconds.count() << std::endl;
	/**********************************************************
	*****SCC RIMOZIONE ARCHI********/

	#ifdef PRINT	
		print(V);
	#endif

	int consiglieri = 0;
	std::stringstream ssCons;
	std::stringstream ssSottoposti;





	/**********************************************************
	*****CREAZIONE ALBERO 2 LEGGI********/
	/*for(int i = 0; i < V; i++) {

		current = grafo[i].successori;
		while( current != NULL )  {
			int succ = current->n;

			grafo[succ].comandato = i;

			current = current->next;
		}
	}


		
	for(int i = 0; i < V; i++) {

		if(grafo[i].comandato == NONE)
			grafo[i].comandato = ROOT;

		if(grafo[i].comandato == ROOT){
			consiglieri++;
			ssCons << i << " ";
		}
		else{
			ssSottoposti << grafo[i].comandato << " " << i << std::endl;
		}
	}*/
	/**********************************************************
	*****CREAZIONE ALBERO 2 LEGGI********/










	/**********************************************************
	*****CREAZIONE ALBERO 3 LEGGI********/
	for(LNodo* comp:scc) {
		hopcroftKarp(comp);
	}
	
	for(int i = 0; i < V; i++) {

			if(grafo[i].comandato == NONE)
				grafo[i].comandato = ROOT;

			if(grafo[i].comandato == ROOT){
				consiglieri++;
				ssCons << i << " ";
			}
			else{
				ssSottoposti << grafo[i].comandato << " " << i << std::endl;
			}
		}	


	/**********************************************************
	*****CREAZIONE ALBERO 3 LEGGI********/






	start = std::chrono::system_clock::now();
	FILE* output = fopen("output.txt","w");

	fprintf(output,"%d\n",consiglieri);
	fprintf(output,"%s\n",ssCons.str().c_str());
	fprintf(output,"%s",ssSottoposti.str().c_str());
	
	fclose(output);
	end = std::chrono::system_clock::now();
	elapsed_seconds = end-start;

	//std::cout << "Scrittura: " << elapsed_seconds.count() << std::endl;

	return 0;
}


void tarjan(int nodo) {
	grafo[nodo].indice = indice;
	grafo[nodo].min_dist = indice;
	indice++;
	s.push(nodo);
	grafo[nodo].onStack = true;
	int tmp;
	LNodo* current = grafo[nodo].successori;
	while( current != NULL)  {
		int succ = current->n;

		if(grafo[succ].indice == -1) {
			tarjan(succ);
			grafo[nodo].min_dist = std::min(grafo[nodo].min_dist, grafo[succ].min_dist);
		}
		else if(grafo[succ].onStack)  //ORRORE
			grafo[nodo].min_dist = std::min(grafo[nodo].min_dist, grafo[succ].indice);

		current = current->next;
	}
	if(grafo[nodo].min_dist == grafo[nodo].indice) {
		LNodo* setN = NULL;
		
		do {
			tmp = s.pop();
			grafo[tmp].onStack = false;
			LNodo* nodotmp = new LNodo();
			nodotmp->n = tmp;
			nodotmp->next = setN;
			setN = nodotmp;
			grafo[tmp].scc = currentScc;
		} while (tmp != nodo);

		scc.push_back(setN);
		currentScc++;
	}
}

void hopcroftKarp(LNodo* comp) {
	int matching = 0;
	LNodo* current = comp;
	while( current != NULL)  {
		int u = current->n;	
		current = current->next;
	}
}

bool BFS(LNodo* comp) {
	int distNONE = -1;
	std::queue<int> coda;
	LNodo* current = comp;
	while( current != NULL)  {
		int u = current->n;	
		if(grafo[u].pairU == NONE) {
			grafo[u].distHK = 0;
			coda.push(u);
		}	
		else {
			grafo[u].distHK = -1;
		}
		current = current->next;
	}
	while(!coda.empty()) {
		int u = coda.front();
		coda.pop();
		if(grafo[u].distHK < distNONE || distNONE == -1) {
				current = grafo[u].predecessori;
				while( current != NULL)  {
					int v = current->n;	
					if(grafo[v].pairV == NONE && distNONE == -1) {
						
					}
					current = current->next;
				}
		}		
	}

}
