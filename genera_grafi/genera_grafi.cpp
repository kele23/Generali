#include <iostream>
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>

using namespace std;

int main(int argc,char* argv[]){


	FILE *output = fopen("input.txt","w");

	int vertici,archi;
	cout << "Vertici: ";
	cin >> vertici;
	cout << "Archi: ";
	cin >> archi;

	fprintf(output,"%d %d\n",vertici,archi);

	srand(time(NULL));

	for(int i = 0; i < archi; i++){

		int v1 = rand() % vertici;
		int v2 = rand() % vertici;

		fprintf(output,"%d %d\n",v1,v2);

	}

	fclose(output);


	return 0;
}