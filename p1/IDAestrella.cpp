#include "IDAestrella.h"

using namespace std;

extern int num_candidatos;
extern int num_votantes;

extern int num_generados;
extern int num_expandidos;
extern int num_cambios;

bool todos;

int IDFS(int g, int limite, Perfil* p, list<candidato>* metas) {

    int f = g + p->h();
    if (f > limite)
        return f;

    candidato ganador = p->calcular_ganador_dodgson();

    if (ganador != NO_GANADOR) {
        num_cambios = g;
        metas->push_back(ganador);

        return f;
    }

    int preferencias = p->obtener_num_preferencias();
    int nuevo_limite;

    /* Para cada preferencia, todos sus posibles cambios
     * elementales */

    num_expandidos++;
	for (int i=0; i < preferencias; i++){
		for (int j=0; j + 1 < num_candidatos; j++){
			num_generados++;
			
			/* Aplicamos el cambio en el perfil */
			p->swap_N(p->obtener(j,i), p->obtener(j+1,i));
			int busqueda = p->aplicar_cambio_elemental(j, i);
			
			nuevo_limite = IDFS(g + 1, limite, p, metas);
			
			/* Desaplicamos el cambio en el perfil */
			p->desaplicar_cambio_elemental(j, busqueda);
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

    while (metas->empty()) {
        limite_f = IDFS(0, limite_f, perfil_inicial, metas);
    }
    //cout << limite_f << endl;
    return *metas;
}
