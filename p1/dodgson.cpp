#include "dodgson.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <vector>
#include <map>

#define NO_ALGORITMO 0
using namespace std;

int num_candidatos;
int num_votantes;
string final;				/* Nombre de archivo para colocar la salida */
string candidatos[250];

int num_generados = 0;
int num_expandidos = 0;
int num_cambios = 0;

int main(int argc, char* argv[]){
	char algoritmo = NO_ALGORITMO;
	bool all = false;
	bool ok = true;
	string filename;

	filename = "";

	/* Verificamos que los parametros se pasen correctamente
	 */
	for (int i=1;i<argc && ok;i++) {
		if(!strcmp(argv[i],"-ida") || !strcmp(argv[i],"-bfs"))
		{
			/* Asignacion del algoritmo */
			if (algoritmo == NO_ALGORITMO){
				algoritmo = (strcmp(argv[i],"-ida") ? 1:2);
			} else {
				ok = false;	// Caso de doble asignacion de algoritmo
			}

		} else if (!strcmp(argv[i],"-all")){

			/* Indica que se deben devolver todos los optimos */
			if (!all) {
				all = true;
			} else {
				ok = false;	// Caso de doble asignacion
			}

		} else if (!strcmp(argv[i],"-final")){

			/* Asignacion de archivo de salida */
			if (++i < argc && final.size() == 0){
				final = argv[i];
			} else {
				ok = false; // Doble asignacion o falta de nombre
			}

		} else if (filename.size() == 0){

			/* Asignacion de archivo de entrada */
			filename = argv[i];

			/* Verificacion de existencia */
			struct stat f__stat;
			if(stat(filename.c_str(),&f__stat)){
				ok = false;
			}
		} else {

			/* Error de entrada */
			ok = false;
		}
	}

	ok = ok && filename.size() && algoritmo;

	if (!ok) {
		cout << endl
			 << "  Uso de dodgson: "
			 << endl << endl
			 << "	dodgson {-ida,-bfs} [-all] [-final <output>] input"
			 << endl << endl;
		exit(0);
	}

	/* Leemos el perfil asociado al archivo de entrada */

	ifstream file;
	file.open(filename.c_str());
	
	string siguiente;
	int num_preferencias;

	/* Descartamos la lectura de profile y leemos el numero de candidatos */
	file >> siguiente >> num_candidatos;

	/* vector y map que permiten la traduccion entre candidatos y si posicion
	 */
	map<string,unsigned char> indice_candidatos;

	/* Leemos todos los candidatos y hacemos las respectivas
	 * asociaciones de indice
	 */
	for (int i=0; i < num_candidatos; i++){
		file >> candidatos[i];
		indice_candidatos.insert(pair<string, unsigned char>(candidatos[i],i));
	}

	file >> num_preferencias;

	Perfil *p = new Perfil();

	/* Leemos el conjunto de preferencias */

	num_votantes = 0;
	while(num_preferencias){
		int num_preferencia;
		candidato *a = new candidato[num_candidatos];

		file >> num_preferencia;

		for (int k=0; k<num_candidatos; k++){
			file >> siguiente;
			a[k] = indice_candidatos[siguiente];
		}

		Preferencia *pr = new Preferencia(num_preferencia,a);
		p->agregar_preferencia(pr);

		num_preferencias -= 1;

		num_votantes += num_preferencia;
	}

	file.close();
	
	listasoluciones resultados;
	switch (algoritmo) {
		case 1:
			resultados = BFS(p, all);
			break;
		case 2:
			//resultados = IDAestrella(p, all);
			break;
	}
	cout << "Dodgson winner:";
	
	resultados.sort();
	while (!resultados.empty()) {
		candidato s = resultados.front().first;
		cout << " " << candidatos[s];
		do {
            resultados.pop_front();
        }
		while (s == resultados.front().first);
	}
	
	cout << endl << "Num cambios elementales: " << num_cambios
			<< endl << "Nodos generados: " << num_generados
			<< endl << "Nodos expandidos: " << num_expandidos
			<< endl;
	
	return 0;
}
