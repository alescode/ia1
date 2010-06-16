#include "BFS.h"

using namespace std;

extern int num_candidatos;
extern int num_votantes;
extern string final;

list<Estado> BFS(Perfil *perfil_inicial, bool all){

	queue<Estado*> *q = new queue<Estado*>();
	list<Estado> metas;
	vector<Estado*> visitados;
	
	Perfil *perfil_padre = NULL;
	Perfil *perfil_actual = NULL;
	Estado *padre_anterior = NULL;

	perfil_inicial->crear_N();

	int frontera = 0;	// Se utiliza para no expandir nodos mas alla de un goal

	/* Encolamos el estado inicial */
	Estado *n = new Estado(NULL, 0, 0 , 0);
	q->push(n);

	int iteraciones = 0;
	int expandidos = 0;

	while(!q->empty()
			&& (metas.empty()
				|| (metas.front().obtener_profundidad()
					== (*q->front()).obtener_profundidad())
				)
		)
	{	
		cout << "iter: " << iteraciones++ << endl;

		n = q->front();
		q->pop();

		/* Verificamos si el padre del nodo anterior es el mismo de este */
		if (!padre_anterior){

			padre_anterior = n->padre();
			perfil_padre = n->construir_padre(perfil_inicial);

		} else if (!n->es_mi_padre(padre_anterior)) {

			delete perfil_padre;
			perfil_padre = n->construir_padre(perfil_inicial);
			padre_anterior = n->padre();

		}

		perfil_actual = new Perfil(*perfil_padre);

		/* Construimos el perfil actual */
		if (n->tiene_padre()) {
			perfil_actual->aplicar_cambio_elemental(n->obtener_fila(),
													n->obtener_columna());
		}

		perfil_actual->obtener_N();

		if (perfil_actual->calcular_ganador_dodgson() != NO_GANADOR){
			/* Hemos llegado a un goal */
			cout << "	gano el " << (int)perfil_actual->calcular_ganador_dodgson() << endl;
			metas.push_back(*n);
			n->print(cout);

			if (!all){
				break;
			}

		} else if (metas.empty()){
			/* No expandimos si hemos conseguido una meta, sus hijos
			 * estaran siempre en un nivel mas que estos
			 */
			n->expandir(q, perfil_actual, num_candidatos,
						&visitados, perfil_inicial);
			expandidos++;
		}
		
		cout << "	nivel: " << n->obtener_profundidad() << endl;
		cout << "	cola: " << (*q).size() << endl;
		cout << "	expandidos: " << expandidos << endl;
		
		delete perfil_actual;
	}
	
	for (int i=0;i<visitados.size();i++){
		delete visitados[i];
	}
	return metas;
}
