void BFS(int i,GeneraleItem* grafo){
	std::cout << i << std::endl;
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
				if(grafo[g].colore != BLACK){
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

				if(grafo[g].colore != BLACK){
					to_control.push(gen);
				}
			}
		}
		to_control.pop();
	}

	grafo[i].colore = BLACK;
}