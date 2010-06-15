#include "BFS.cpp"

using namespace std;

extern int num_candidatos;
extern int num_votantes;

list<Estado> IDAestrella(Perfil *perfil_inicial, bool all){
	priority_queue<Estado*> *q = new priority_queue<Estado*>();

	list<Estado> metas;
	vector<Estado*> visitados;
	
	Perfil *perfil_padre = NULL;
	Perfil *perfil_actual = NULL;
	Estado *padre_anterior = NULL;

	perfil_inicial->crear_N();

    int limite_f = perfil_inicial->h();

    cout << "IDA*: limite ";
    cout << limite_f << endl;
#if 0
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

		/* Construimos el perfil actual */
		if (!padre_anterior){
			padre_anterior = n->padre();
			perfil_padre = n->construir_padre(perfil_inicial);
		} else if (!n->es_mi_padre(padre_anterior)) {
			delete perfil_padre;
			perfil_padre = n->construir_padre(perfil_inicial);
			padre_anterior = n->padre();
		}

		perfil_actual = new Perfil(*perfil_padre);

		if (n->tiene_padre()) {
			/*
			Estado* s = n;
			while(s){
				cout << s << ": ";
				s->print(cout);
				s = s->padre();
			}
			*/
			perfil_actual->aplicar_cambio_elemental(n->obtener_fila(),
													n->obtener_columna());
		}

		/* Verificamos si hemos llegado a un goal, si no expandimos */
		perfil_actual->obtener_N();

		if (perfil_actual->calcular_ganador_dodgson() != NO_GANADOR){

			cout << "	gano el " << (int)perfil_actual->calcular_ganador_dodgson() << endl;
			metas.push_back(*n);
			n->print(cout);
			if (!all){
				break;
			}
		} else if (metas.empty()){
			n->expandir(q, perfil_actual, num_candidatos, &visitados, perfil_inicial);
			expandidos++;
		}

		cout << "	nivel: " << n->obtener_profundidad() << endl;
		cout << "	cola: " << (*q).size() << endl;
		cout << "	expandidos: " << expandidos << endl;
		delete perfil_actual;
	}
#endif
	return metas;
}
