#include "IDAestrella.h"

using namespace std;

extern int num_candidatos;
extern int num_votantes;

extern int num_generados;
extern int num_expandidos;
extern int num_cambios;

bool alll;

int IDFS(int g, int limite, Perfil* p, list<candidato>* metas) {
    int f = g + p->h();
    if (f > limite)
        return f;

    candidato ganador = p->calcular_ganador_dodgson();
    if (ganador != NO_GANADOR) {
        cout << "GANADOR" << endl;
        cout << g << endl;
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
            int busqueda = p->aplicar_cambio_elemental(j, i);
            p->obtener_N();
            num_generados++;

            //p->print(cout);
            //cout << "APLICANDO " << j << " en preferencia " << i << endl;

            nuevo_limite = IDFS(g + 1, limite, p, metas);

            p->desaplicar_cambio_elemental(j, busqueda);

<<<<<<< HEAD
            /*
            if (!metas->empty()) {
=======
            if (!alll && !metas->empty()) {
>>>>>>> 885e9c5bdbcd6f7a7765c61909c20cb9d9258be2
                return nuevo_limite;
            }*/
        }
    }
    return nuevo_limite;
}

list<candidato> IDAestrella(Perfil *perfil_inicial, bool all){
	alll = all;
	
    list<candidato>* metas = new list<candidato>;

    perfil_inicial->crear_N();
    perfil_inicial->obtener_N();
        perfil_inicial->imprimir_N();

    int limite_f = perfil_inicial->h();

    while (metas->empty()) {
        cout << limite_f << endl;
        limite_f = IDFS(0, limite_f, perfil_inicial, metas);
    }
    cout << limite_f << endl;
    return *metas;
}
