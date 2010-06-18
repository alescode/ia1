#ifndef _IDAestrella
	#define _IDAestrella

#include <iostream>
#include <queue>
#include <list>
#include "Estado.h"

typedef struct Cambio {
    unsigned char fila;
    int columna;
};

list<candidato> IDAestrella(Perfil *perfil_inicial, bool all);

#endif
