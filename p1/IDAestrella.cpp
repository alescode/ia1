#include "IDAestrella.h"

using namespace std;

extern int num_candidatos;
extern int num_votantes;

extern int num_generados;
extern int num_expandidos;
extern int num_cambios;
extern string final;
extern string candidatos[MAX_CANDIDATOS];
extern int no_memorizar;

int veces_ganadas[MAX_CANDIDATOS] = {0};

bool todos;

int IDFS(int g, int limite, Perfil* p, list<candidato>* metas, list<Cambio*>* visitados, Perfil* inicial) {

    int f = g + p->h();
    if (f > limite) {
        return f;
    }

    candidato ganador = p->calcular_ganador_dodgson();

    if (ganador != NO_GANADOR) {
        num_cambios = g;
        metas->push_back(ganador);

        if (final.size() != 0) {
            /* Impresion del perfil ganador,
             * se realiza en este nivel para no
             * tener que recalcular el perfil */ 
            stringstream nombre_archivo;
            nombre_archivo << candidatos[ganador];
            nombre_archivo << veces_ganadas[ganador];
            veces_ganadas[ganador]++;
            nombre_archivo << "-" << final;

            p->guardar(nombre_archivo.str());
        }
        return f;
    }

	//cout << visitados->next() << endl;
	//inicial->print(cout);
	
    int preferencias = p->obtener_num_preferencias();
    int nuevo_limite = -1;

    /* Para cada preferencia, todos sus posibles cambios
     * elementales */

    num_expandidos++;
	for (int i=0; i < preferencias; i++){
		for (unsigned char j = 0; j + 1 < num_candidatos; j++){

			if (!visitados->empty()
				 &&
				 (visitados->back())->fila == j
				 &&
				 (visitados->back())->columna == i){
				continue;
			}
			
			p->swap_N(p->obtener(j,i), p->obtener(j+1,i));
			int busqueda = p->aplicar_cambio_elemental(j, i);
			
			if (no_memorizar) {
				num_generados++;
				
				Cambio* nuevo_cambio = new Cambio();
				nuevo_cambio->fila = busqueda;
				nuevo_cambio->columna = i;
				visitados->push_back(nuevo_cambio);

				if (nuevo_limite == -1){
					nuevo_limite = IDFS(g + 1, limite, p, metas, visitados, inicial);
				} else {
					int nuevo = IDFS(g + 1, limite, p, metas, visitados, inicial);
					nuevo_limite = (nuevo<nuevo_limite)?nuevo:nuevo_limite;
				}
			
				delete visitados->back();
				visitados->pop_back();
				
			} else {
				
				/* Comparar con los nodos visitados ancestros */
				Perfil* s = new Perfil(*inicial);
				
				bool iguales = false;
				if (s->compare(*p) == 0){
					iguales = true;
				} else {
					list<Cambio*>::iterator it;

					for ( it=visitados->begin() ; it != visitados->end() && !iguales; it++ ){
						s->aplicar_cambio_elemental((*it)->fila, (*it)->columna);
						iguales = (s->compare(*p)==0);
					}
				}
				delete s;
				
				if (!iguales) {
					num_generados++;
					
					Cambio* nuevo_cambio = new Cambio();
					nuevo_cambio->fila = j;
					nuevo_cambio->columna = i;
					visitados->push_back(nuevo_cambio);

					if (nuevo_limite == -1){
						nuevo_limite = IDFS(g + 1, limite, p, metas, visitados, inicial);
					} else {
						int nuevo = IDFS(g + 1, limite, p, metas, visitados, inicial);
						nuevo_limite = (nuevo<nuevo_limite)?nuevo:nuevo_limite;
					}
				
					delete visitados->back();
					visitados->pop_back();
				}
			}
			
			p->aplicar_cambio_elemental(j, busqueda);
			p->swap_N(p->obtener(j+1,i), p->obtener(j,i));

			if (!todos && !metas->empty()) {
				return nuevo_limite;
			}
		}
	}
	return nuevo_limite;
}

list<candidato> IDAestrella(Perfil *perfil_inicial, bool all){
	todos = all;
	
    list<candidato>* metas = new list<candidato>;

    perfil_inicial->crear_N();
    perfil_inicial->obtener_N();

    int limite_f = perfil_inicial->h();
    list<Cambio*>* visitados = new list<Cambio*>;
	
	Perfil *p = new Perfil(*perfil_inicial);
	
    while (metas->empty()) {
        limite_f = IDFS(0, limite_f, p, metas, visitados, perfil_inicial);
    }

    return *metas;
}
