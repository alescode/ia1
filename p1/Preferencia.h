
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

        void resetear(int size) ;

        //Preferencia& operator= (Preferencia& p) ;
        
        int obtener_votantes();

        candidato* obtener_orden();

		void crementar(int signo);

        void cambio_elemental(unsigned char fila);
		
        int compare(Preferencia& p);

		int compare2(Preferencia& p);
		
		void print2(ostream& os, string candidatos[250]);
};

#endif