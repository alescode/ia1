#ifndef _PERFIL
	#define _PERFIL

#include <iostream>
#include <fstream>
#include "Preferencia.h"

class Perfil {
	private:
        vector<Preferencia*>* info;
        int** N;

    public:
        Perfil (int capacidad = num_votantes) ;

        ~Perfil () ;

        Perfil (const Perfil& p) ;

        void print(ostream& os) ;

        int busqueda_binaria(Preferencia* p) ;

        /* Insertar ordenado */
        int agregar_preferencia(Preferencia* p) ;

		void eliminar_preferencia(int num_preferencia) ;

        void resetear() ;

        void crear_N() ;

        void eliminar_N() ;

        void imprimir_N() ;

        //Perfil& operator= (Perfil& p) ;}

        candidato calcular_ganador_dodgson() ;

        /* Efectúa un cambio elemental entre un candidato y el siguiente
         * en la lista de preferencias indicada por num_preferencia.
         * Devuelve un nuevo perfil idéntico al original exceptuando este cambio. */
        int aplicar_cambio_elemental(candidato num_candidato, int num_preferencia) ;

		void desaplicar_cambio_elemental(candidato num_candidato, int num_preferencia);

        inline int obtener_num_preferencias(){
            return info->size();
        }

        inline int deficit(candidato x, candidato y) {
            return max(0, ((num_votantes + 2)/2) - N[x][y]); //techo de n+1/2
        }

        void obtener_N() ;

		int compare(Perfil& p);

        int funcion_de_clasificacion() ;

        double Tprima(candidato x) ;

        int h() ;

        int calcular_cambio_N(candidato a,candidato b);

		candidato obtener(candidato j, int i);

		void guardar(string filename);
		
        inline void swap_N(candidato i, candidato j){
            N[i][j]--;
            N[j][i]++;
        }
};

#endif
