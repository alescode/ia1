#include "Preferencia.cpp"

class Perfil {
	private:
        vector<Preferencia*>* info;
        int** N;

    public:
        Perfil (int capacidad = num_votantes) {
            this->info = new vector<Preferencia*>;
            (this->info)->reserve(capacidad);
            N = NULL;
        }

        ~Perfil () {
//            cout << "Eliminando perfil" << endl;
            int size = (this->info)->size();
            for (int i = 0; i < size; i++)
                delete (*(this->info))[i];
            delete this->info;
            N = NULL; //No borrado
        }

        Perfil (const Perfil& p) {
            this->info = new vector<Preferencia*>;
			
			int tam = (p.info)->size();
			(this->info)->reserve(tam);

			for (int i=0; i < tam ; i++){
				info->push_back(new Preferencia(*((*(p.info))[i])));
			}
			
			//this->info = new vector<Preferencia*>(*(p.info));
            this->N = p.N;
        }

        void print(ostream& os) {
            os << "Perfil: " << endl;
            int size = (this->info)->size();
            for (int i = 0; i < size; i++)
               (*(this->info))[i]->print(cout);
        }

        /*Convencion: si el elemento esta en el vector,
         * devolver su indice.
         * Si el elemento no esta, devolver -(indice+1) */
        int busqueda_binaria(Preferencia* p) {
            vector<Preferencia*> preferencias = *(this->info);
            
            Preferencia pref = *p; //elemento a buscar

            int bajo = 0;
            int alto = preferencias.size();

            while (bajo < alto) {
                int medio = (alto + bajo)/2; //no overflow
                Preferencia valor_medio = *preferencias[medio];

                int comparacion = pref.compare(valor_medio);
                if (comparacion > 0) bajo = medio + 1;
                else if (comparacion < 0) alto = medio;
                else return medio;
            }
            return -(bajo + 1);
        }

        /* Insertar ordenado */
        void agregar_preferencia(Preferencia* p) {
            int buscar = busqueda_binaria(p);

            if (buscar >= 0) { // La preferencia existe dentro del perfil
                int num_votantes = p->obtener_votantes();
                delete p; // No se necesita almacenar una preferencia nueva

                p = (*(this->info))[buscar];
                p->crementar(num_votantes);
            }
            else { // Debe agregarse una nueva preferencia al perfil
                int insertar = -buscar-1; // Se obtiene el indice donde debe insertarse
                vector<Preferencia*>::iterator it = (this->info)->begin() + insertar;
                (this->info)->insert(it, p);
            }
        }

		inline void eliminar_preferencia(int num_preferencia) {
            vector<Preferencia*>::iterator it = (this->info)->begin() + num_preferencia;
            (this->info)->erase(it);
        }

        void resetear() {
            for (int i = 0; i < (this->info)->size(); i++) {
                (*(this->info))[i]->resetear(num_candidatos);
            }
        }

        void crear_N() {
            int mitad_votantes = num_votantes >> 1;
            vector<Preferencia*> preferencias = *(this->info);

            //N(x, y): Número de votantes que prefieren a x sobre y
            this->N = new int*[num_candidatos];
            for (int i = 0; i < num_candidatos; i++) {
                N[i] = new int[num_candidatos];
                memset(N[i], 0, num_candidatos * sizeof(int));
            }
        }

        inline void eliminar_N() {
            for (int i = 0; i < num_candidatos; i++) 
                delete [] this->N[i];
            delete this->N;
            this->N = NULL;
        }

        inline void imprimir_N() {
            for (int i = 0; i < num_candidatos; i++) {
                for (int j = 0; j < num_candidatos; j++)
                    cout << this->N[i][j] << " ";
                cout << endl;
            }
        }

        //Perfil& operator= (Perfil& p) {}

        candidato calcular_ganador_dodgson() {
            int mitad_votantes = num_votantes/2;

            for (int i = 0; i < num_candidatos; i++) { 
                int j = 0;
                while (j < num_candidatos && (i == j || this->N[i][j] > mitad_votantes))
                    j++;
                if (j == num_candidatos) {
                    return i;
                }
            }
            return NO_GANADOR;
        }

        /* Efectúa un cambio elemental entre un candidato y el siguiente
         * en la lista de preferencias indicada por num_preferencia.
         * Devuelve un nuevo perfil idéntico al original exceptuando este cambio. */
        void aplicar_cambio_elemental(candidato num_candidato, int num_preferencia) {
            Preferencia* p = (*info)[num_preferencia];
			/* Decrementamos el numero de votantes con esta preferencia */
			p->crementar(-1);

			Preferencia* unitaria = new Preferencia(*p, true);
			unitaria->cambio_elemental(num_candidato);

			/* Eliminamos la preferencia antigua si no tiene votantes */
			if (p->obtener_votantes()  == 0) {
                this->eliminar_preferencia(num_preferencia);
                delete p;
            }
			
            this->agregar_preferencia(unitaria);
        }

        int obtener_num_preferencias(){
			return info->size();
		}

        void obtener_N() {
			for (int i = 0; i < num_candidatos; i++) {
                N[i] = new int[num_candidatos];
                memset(N[i], 0, num_candidatos * sizeof(int));
            }
            
			vector<Preferencia*> preferencias = *(this->info);

            for (int i = 0; i < preferencias.size(); i++) {
                int votantes_p = preferencias[i]->obtener_votantes();

                candidato* preferencia = preferencias[i]->obtener_orden();

                for (int j = 0; j < num_candidatos; j++) {
                    for (int k = j+1; k < num_candidatos; k++) {
                        N[preferencia[j]][preferencia[k]] += votantes_p;
                    }
                }
            }
        }

		int compare(Perfil& p){
			/* Verificamos que tengan igual numero de preferencias */
			int dif = p.obtener_num_preferencias() - info->size();

			/* Comparamos cada preferencia */
			for (int i = 0; i < info->size() && !dif; i++){
				dif = (*info)[i]->compare2(*(*p.info)[i]);
			}
			return dif;
		}

        inline int deficit(candidato x, candidato y) {
            return max(0, ((num_votantes + 2)/2) - N[x][y]); //techo de n+1/2
        }

        inline double Tprima(candidato x) {
            int T = 0;
            for (candidato c = 0; c < num_candidatos; c++) {
				if (c!=x){
					T += deficit(x, c);
				}
            }
            double lg = log2(num_votantes);
            return (((double) T + lg + 1.0)/(lg + 3.0));
        }

        inline double h() {
            double min = numeric_limits<double>::infinity();
            for (candidato c = 0; c < num_candidatos; c++) {
                double t = Tprima(c);
                if (t < min)
                    min = t;
            }
            return min;
        }
        
        inline int funcion_de_clasificacion() {
			double sol = 0;
            for (candidato c = 0; c < num_candidatos; c++) {
                double t = Tprima(c);
                sol = 10*sol + t;
            }
            return (int)ceil(sol);
		}

        int calcular_cambio_N(candidato a,candidato b){
			N[a][b]--;
			N[b][a]++;
			
            int sol = funcion_de_clasificacion();
			
			N[a][b]++;
			N[b][a]--;
			
			return sol;
		}
		
		inline candidato obtener(candidato j, int i){
			return (((*this->info)[i])->obtener_orden())[j];
		}
};

/*
int main(void) {

    candidato* c = new candidato[4];
    candidato cc[] = {0, 1, 2, 3 };
    memcpy(c, cc, 4*sizeof(candidato));

    candidato* d = new candidato[4];
    candidato dd[] = {0, 3, 1, 2};
    memcpy(d, dd, 4*sizeof(candidato));

    candidato* e = new candidato[4];
    candidato ee[] = {1, 2, 0, 3};
    memcpy(e, ee, 4*sizeof(candidato));

    candidato* f = new candidato[4];
    candidato ff[] = {2, 0, 1, 3};
    memcpy(f, ff, 4*sizeof(candidato));

	candidato* g = new candidato[4];
    candidato gg[] = {3, 0, 1, 2};
    memcpy(g, gg, 4*sizeof(candidato));
	
	candidato* h = new candidato[4];
    candidato hh[] = {3, 1, 2, 0};
    memcpy(h, hh, 4*sizeof(candidato));
	
	candidato* i = new candidato[4];
    candidato ii[] = {3, 2, 0, 1};
    memcpy(i, ii, 4*sizeof(candidato));

    Preferencia* p1 = new Preferencia(2,c);
    Preferencia* p2 = new Preferencia(1,d);
    Preferencia* p3 = new Preferencia(2,e);
    Preferencia* p4 = new Preferencia(2,f);
	Preferencia* p5 = new Preferencia(1,g);
	Preferencia* p6 = new Preferencia(2,h);
	Preferencia* p7 = new Preferencia(2,i);

    Perfil* p = new Perfil();
    p->agregar_preferencia(p2);
    p->agregar_preferencia(p1);
    p->agregar_preferencia(p4);
    p->agregar_preferencia(p3);
	p->agregar_preferencia(p5);
	p->agregar_preferencia(p6);
	p->agregar_preferencia(p7);

    p->print(cout);
	p->aplicar_cambio_elemental(1,1);
	p->print(cout);
}
*/