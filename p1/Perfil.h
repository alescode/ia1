#ifndef _PERFIL
	#define _PERFIL

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

		inline void desaplicar_cambio_elemental(candidato num_candidato, int num_preferencia);

        int obtener_num_preferencias();

        void obtener_N() ;

		int compare(Perfil& p);

        int funcion_de_clasificacion() ;

        int deficit(candidato x, candidato y) ;

        double Tprima(candidato x) ;

        int h() ;

        int calcular_cambio_N(candidato a,candidato b);

		 candidato obtener(candidato j, int i);
};

#endif
