#include "Estado.h"

Estado::Estado(Estado* a, int f, int c, int p){
    prog = a;
    fila = f;
    columna = c;
    profundidad = p;
}

Estado::~Estado() {}


Perfil* Estado::construir_padre(Perfil* inicial) {

    Perfil* nuevo = new Perfil(*inicial);
    if (this->tiene_padre()) {
		
        stack<Estado*> acciones;
        Estado* s = this;
        int num_acciones = 0;

        /* Empilamos todas las acciones necesarias para construir el
         *	perfil del padre */
        while (s->padre()) {
            acciones.push(s);
            s = s->padre();
            num_acciones++;
        }

        /* Aplicamos todas las acciones desde el perfil inicial hasta
         * el del padre. Notese que no es incluye la accion padre - this */
        while (--num_acciones) {
            s = acciones.top();
            acciones.pop();
            nuevo->aplicar_cambio_elemental(s->obtener_fila(),
                    s->obtener_columna());
        }
    }

    return nuevo;
}

/* Devuelve el perfil del estado actual aplicando
 * el cambio elemental al perfil del padre
 */
void Estado::construir_perfil(Perfil* perfil) {
    if (prog)
        perfil->aplicar_cambio_elemental(this->fila, this->columna);
}

/* true ssi *a es el padre del estado actual */
bool Estado::es_mi_padre(Estado* a) {
    return prog == a;
}

int Estado::fue_visitado(Perfil& p, vector<Estado*> &visitados,
        Perfil* inicial) {
    int izq, der;

    izq = 0;
    der = visitados.size();


    double contador_total = 0;
    double contador_no_resueltos = 0;

    while (izq != der) {
        contador_total++;

        int med = izq + ((der - izq) >> 1);

        int comparacion  = visitados[med]->clasificacion - this->clasificacion;

        if (comparacion < 0){
            izq = med + 1;
        } else if (comparacion > 0){
            der = med;
        } else {
            contador_no_resueltos++;

            Perfil* p_visitado = visitados[med]->construir_padre(inicial);
            visitados[med]->construir_perfil(p_visitado);

            comparacion = p_visitado->compare(p);

            delete p_visitado;

            if (comparacion == 0) {
                return -(izq+1);
            } else if (comparacion < 0){
                izq = med+1;
            } else {
                der = med;
            }
        }
    }
    return izq;
}

void Estado::expandir(queue<Estado*>* q, Perfil *p, int candidatos,
        vector<Estado*>*visitados, Perfil *inicial){
    int preferencias = p->obtener_num_preferencias();

    /* Para cada preferencia, todos sus posibles cambios
     * elementales */

    for (int i=0; i < preferencias; i++){
        for (int j=0; j + 1 < candidatos; j++){

            Estado* nuevo = new Estado(this, j, i, profundidad + 1);
            nuevo->clasificacion = 0;

            //	q->push(nuevo);
            //	continue;

            Perfil* perfil_nuevo = new Perfil(*p);

            perfil_nuevo->aplicar_cambio_elemental(j,i);

            nuevo->clasificacion = perfil_nuevo->calcular_cambio_N(p->obtener(j,i),p->obtener(j+1,i));

            int visitado = nuevo->fue_visitado(*perfil_nuevo, *visitados, inicial);

            if (visitado >= 0){
                visitados->insert(visitados->begin() + visitado, nuevo);
                q->push(nuevo);
            } else {
                delete nuevo;
            }

            delete perfil_nuevo;
        }
    }
}

inline void Estado::print(ostream& os) {
    os << prog			<< " "
        << int(fila)		<< " "
        << columna		<< " "
        << profundidad	<< endl;
}
