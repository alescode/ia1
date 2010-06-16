#include "IDAestrella.h"

using namespace std;

extern int num_candidatos;
extern int num_votantes;


int IDFS(int g, int limite, Perfil* p, list<candidato>* metas) {
    int f = g + p->h();
    if (f > limite)
        return f;

    candidato ganador = p->calcular_ganador_dodgson();
    if (ganador != NO_GANADOR) {
        metas->push_back(ganador);
        return f;
    }

    int preferencias = p->obtener_num_preferencias();
    cout << preferencias << endl;
    int nuevo_limite;

    /* Para cada preferencia, todos sus posibles cambios
     * elementales */

    for (int i=0; i < preferencias; i++){
        for (int j=0; j + 1 < num_candidatos; j++){
            p->aplicar_cambio_elemental(j, i);
            p->obtener_N();

            p->print(cout);
            cout << "APLICANDO " << j << " en preferencia " << i << endl;

            nuevo_limite = IDFS(g + 1, limite, p, metas);
            p->desaplicar_cambio_elemental(j, i);

            if (!metas->empty()) {
                return nuevo_limite;
            }
        }
    }
    return nuevo_limite;
}

        list<candidato>* IDAestrella(Perfil *perfil_inicial, bool all){
            list<candidato>* metas = new list<candidato>;

            perfil_inicial->crear_N();
            perfil_inicial->obtener_N();

            int limite_f = perfil_inicial->h();

            cout << "IDA*: limite ";
            cout << limite_f << endl;

            while (true) {
                limite_f = IDFS(0, limite_f, perfil_inicial, metas);
                if (!metas->empty())
                    return metas;
            }
        }

#if 0
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
        return metas;
    }
#endif
