#ifndef _BFS
	#define _BFS

#include <iostream>
#include <queue>
#include <list>
#include "Estado.h"
#include <sstream>
#include <cstring>

typedef list<pair<candidato, Estado> > listasoluciones;

listasoluciones BFS(Perfil *perfil_inicial, bool all);

#endif
