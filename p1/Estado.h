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
        int clasificacion; // Ahorra comparaciones de nodos
        Estado* prog; // Apuntador al padre
        unsigned char fila; // Candidato de ultimo cambio
        int columna; // Numero de preferencia donde se realizo el ultimo cambio
        int profundidad;

    public:
        Estado(Estado*a, int f, int c, int p);

        ~Estado();

        /* Devuelve la profundidad del estado en el recorrido BFS
         * usado para no expandir nodos mas alla de la primera meta */
        int obtener_profundidad(){
            return profundidad;
        }

        /* Devuelve el padre del estado actual*/
        inline Estado* padre(){
            return prog;
        }

        /* Devuelve true ssi el estado tiene padre*/
        inline bool tiene_padre(){
            return prog != NULL;
        }

        /* Devuelve la fila sobre la cual se hizo un cambio sobre el
         * perfil del padre para obtener este estado
         */
        inline int obtener_fila(){
            return fila;
        }

        /* Devuelve la preferencias sobre la cual se hizo un cambio
         * sobre el perfil del padre para obtener este estado */
        inline int obtener_columna(){
            return columna;
        }

        /* Devuelve el perfil del estado padre*/
        Perfil* construir_padre(Perfil* inicial);

        /* Devuelve el perfil del estado actual aplicando
         * el cambio elemental al perfil del padre
         */
        void construir_perfil(Perfil* perfil);

        /* true ssi*a es el padre del estado actual */
        bool es_mi_padre(Estado* a);

        int fue_visitado(Perfil& p, vector<Estado*> &visitados,
                Perfil* inicial);

        /* Encola todos los estados resultantes de expandir el nodo actual*/
        void expandir(queue<Estado*>* q, Perfil *p, int candidatos,
                vector<Estado*>*visitados, Perfil *inicial);

        inline void print(ostream& os) {
            os << prog			<< " "
                << int(fila)		<< " "
                << columna		<< " "
                << profundidad	<< endl;
        }
};

#endif
