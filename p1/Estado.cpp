#ifndef _ESTADO
	#define _ESTADO

#include <cstring>
#include <stack>
#include <queue>
#include <cmath>
#include "Perfil.h"

#define NO_VISITADO -1
#define VISITADO -2


class Estado {
    private:
        int h;
        Estado*prog;
        unsigned char fila;
        int columna;
        int profundidad;

    public:
        Estado(Estado*a, int f, int c, int p){
            prog = a;
            fila = f;
            columna = c;
            profundidad = p;
        }

        ~Estado() {}

        /* Devuelve la profundidad del estado en el recorrido BFS
         * usado para no expandir nodos mas alla de la primera meta */
        int obtener_profundidad(){
            return profundidad;
        }

        /* Devuelve el padre del estado actual*/
        Estado* padre(){
            return prog;
        }

        /* Devuelve true ssi el estado tiene padre*/
        bool tiene_padre(){
            return prog != NULL;
        }

        /* Devuelve la fila sobre la cual se hizo un cambio sobre el
         * perfil del padre para obtener este estado
         */
        inline int obtener_fila(){
            return fila;
        }

        /* Devuelve la preferencias sobre la cual se hizo un cambio
         * sobre el perfil del padre para obtener este estado
         */
        inline int obtener_columna(){
            return columna;
        }

        /* Devuelve el perfil del estado padre*/
        Perfil* construir_padre(Perfil* inicial) {
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
        void construir_perfil(Perfil* perfil) {
            if (prog)
                perfil->aplicar_cambio_elemental(this->fila, this->columna);
        }

        /* true ssi*a es el padre del estado actual */
        bool es_mi_padre(Estado* a) {
            return prog == a;
        }

        int fue_visitado(Perfil& p, vector<Estado*> &visitados,
                Perfil* inicial) {
            int izq, der;

            izq = 0;
            der = visitados.size();


            double contador_total = 0;
            double contador_no_resueltos = 0;

            while (izq != der) {
                contador_total++;

                int med = izq + ((der - izq) >> 1);

                int comparacion  = visitados[med]->h - this->h;

                if (comparacion < 0){
                    izq = med + 1;
                } else if (comparacion > 0){
                    der = med;
                } else {
                    contador_no_resueltos++;
                    // SI LOS VALORES HEURISTICOS SON LOS MISMOS RECURRIR A ESTO
                    Perfil* p_visitado = visitados[med]->construir_padre(inicial);
                    visitados[med]->construir_perfil(p_visitado);

                    comparacion = p_visitado->compare(p);

                    if (comparacion == 0) {
                        //cout << "prop: " << contador_no_resueltos/contador_total << endl;
						delete p_visitado;
                        return -(izq+1);
                    } else if (comparacion < 0){
                        izq = med+1;
                    } else {
                        der = med;
                    }

                    delete p_visitado;
                }
            }


            //			cout << "prop: " <<contador_no_resueltos/contador_total << endl;

            return izq;
        }

        /* Encola todos los estados resultantes de expandir el nodo actual*/
        void expandir(queue<Estado*>* q, Perfil *p, int candidatos,
                vector<Estado*>*visitados, Perfil *inicial){
            int preferencias = p->obtener_num_preferencias();

            /* Para cada preferencia, todos sus posibles cambios
             * elementales */

            for (int i=0; i<preferencias; i++){
                for (int j=0; j + 1 < candidatos; j++){

                    Estado* nuevo = new Estado(this, j, i, profundidad + 1);
                    nuevo->h = 0;

                    
					//					q->push(nuevo);
					//					continue;

                    Perfil* perfil_nuevo = new Perfil(*p);

                    perfil_nuevo->aplicar_cambio_elemental(j,i);

                    nuevo->h = perfil_nuevo->calcular_cambio_N(p->obtener(j,i),p->obtener(j+1,i));
					
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

        void print(ostream& os) {
            os << prog			<< " "
                << int(fila)		<< " "
                << columna		<< " "
                << profundidad	<< endl;
        }

};

#endif