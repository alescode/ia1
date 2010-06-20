#ifndef _BFS
	#define _BFS

#include <iostream>
#include <queue>
#include <list>
#include "Estado.h"
#include <sstream>
#include <cstring>

typedef list<candidato> listasoluciones;

list<candidato> BFS(Perfil *perfil_inicial, bool all);

#endif
