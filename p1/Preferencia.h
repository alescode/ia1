#ifndef _PREFERENCIA
	#define _PREFERENCIA
	#include <string>
	#include <iostream>
	#include <vector>
	#include <algorithm>
	#include <string.h>
	#include <cmath>
	#include <limits>

#define MAX_CANDIDATOS 250
#define NO_GANADOR 255

using namespace std;

typedef unsigned char candidato;

extern int num_candidatos;
extern int num_votantes;

class Preferencia {
	private:
        int votantes_preferencia;
        candidato* orden;

    public:
        Preferencia(int v, candidato* o) ;

        //unitaria inicializa preferencia con votantes = 1
        Preferencia(const Preferencia& p, bool unitaria=false) ;

        ~Preferencia() ;

		void print(ostream& os) ;

        //Preferencia& operator= (Preferencia& p) ;

        inline void resetear(int size) {
            memset(this->orden, 0, size);
        }

        //Preferencia& operator= (Preferencia& p) {}

        inline int obtener_votantes(){
            return this->votantes_preferencia;
        }

        inline candidato* obtener_orden(){
            return this->orden;
        }

        inline void crementar(int signo){
            this->votantes_preferencia += signo;
        }

        void cambio_elemental(unsigned char fila);
		
        int compare(Preferencia& p);

		int compare2(Preferencia& p);
		
		void print2(ostream& os, string candidatos[250]);
};

#endif
