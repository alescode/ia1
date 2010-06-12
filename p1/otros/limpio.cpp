#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>
#include <limits>

#define MAX_CANDIDATOS 250
#define NO_GANADOR 255

using namespace std;

typedef unsigned char candidato;

/*extern */int num_candidatos = 4;
/*extern */int num_votantes = 10;

class Preferencia {
	private:
        int votantes_preferencia;
        candidato* orden;

    public:
        Preferencia(int v, candidato* o) {
            this->votantes_preferencia = v;
            this->orden = o;
        }

        //unitaria inicializa preferencia con votantes = 1
        inline Preferencia(const Preferencia& p, bool unitaria = false) {
            if (unitaria) {
                this->votantes_preferencia = 1;
            }
            else {
                this->votantes_preferencia = p.votantes_preferencia;
            }
            this->orden = new candidato[num_candidatos];

            memcpy(this->orden, p.orden, num_candidatos*sizeof(candidato));
        }

        ~Preferencia() {
            //cout << "Destruyendo " << this->votantes_preferencia << endl;
            delete [] this->orden;
        }

		void print(ostream& os) {
            os << this->votantes_preferencia << endl;

            for (int i = 0; i < num_candidatos; i++) {
                os << (int) this->orden[i] << " ";
            }
            os << endl;
        }

        void resetear(int size) {
            memset(this->orden, 0, size);
        }

        //Preferencia& operator= (Preferencia& p) {}
        
        int obtener_votantes(){
			return this->votantes_preferencia;
		}
		
		candidato* obtener_orden(){
			return this->orden;
		}

		inline void crementar(int signo){
			this->votantes_preferencia += signo;
		}
		
		void cambio_elemental(unsigned char fila){
			candidato tmp = this->orden[fila];
			this->orden[fila] = this->orden[fila+1];
			this->orden[fila+1] = tmp;
			this->votantes_preferencia = 1;
		}
		
		inline int compare(Preferencia& p){
			/* Verificamos que cada preferencia tenga igual permutacion de candidatos */
			int dif = memcmp(obtener_orden(), p.obtener_orden(), num_candidatos);

			return dif;
		}
};

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
            cout << "Eliminando perfil" << endl;
            int size = (this->info)->size();
            for (int i = 0; i < size; i++)
                delete (*(this->info))[i];
            delete this->info;
            N = NULL; //No borrado
        }

        Perfil (const Perfil& p) {
            this->info = new vector<Preferencia*>(*(p.info));
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

        candidato calcular_ganador_dogson() {
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

        int** obtener_N() {
            return this->N;
        }

		int compare(Perfil& p){
			/* Verificamos que tengan igual numero de preferencias */
			int dif = p.obtener_num_preferencias() - info->size();

			/* Comparamos cada preferencia */
			for (int i = 0; i < info->size() && !dif; i++){
				dif = (*info)[i]->compare(*(*p.info)[i]);
			}
			return dif;
		}

        inline int deficit(candidato x, candidato y) {
            return max(0, ((num_votantes + 2)/2) - N[x][y]); //techo de n+1/2
        }

        inline double Tprima(candidato x) {
            int T = 0;
            for (candidato c = 0; c != x && c < num_candidatos; c++) {
                T += deficit(x, c);
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
};

int main(void) {
    candidato* c = new candidato[4];
    candidato cc[] = {0, 2, 3, 1};
    memcpy(c, cc, 4*sizeof(candidato));

    candidato* d = new candidato[4];
    candidato dd[] = {3, 0, 2, 1};
    memcpy(d, dd, 4*sizeof(candidato));

    candidato* e = new candidato[4];
    candidato ee[] = {1, 0, 2, 3};
    memcpy(e, ee, 4*sizeof(candidato));

    candidato* f = new candidato[4];
    candidato ff[] = {1, 3, 0, 2};
    memcpy(f, ff, 4*sizeof(candidato));

    Preferencia* p1 = new Preferencia(1,c);
    Preferencia* p2 = new Preferencia(2,d);
    Preferencia* p3 = new Preferencia(3,e);
    Preferencia* p4 = new Preferencia(4,f);

    Perfil* p = new Perfil();
    p->agregar_preferencia(p2);
    p->agregar_preferencia(p1);
    p->agregar_preferencia(p4);
    p->agregar_preferencia(p3);

    p->print(cout);
    p->crear_N();
    p->imprimir_N();

    cout << p->h() << endl;
    /*for (int i = 0; i < 10; i++) {
        p->aplicar_cambio_elemental(0,0);
        p->print(cout);
    }
    for (double i = 0; i < (long long) 999999999; i+=0.0001) {
        //cout << i << " ";
        p->aplicar_cambio_elemental(0,0);
    }*/

#if 0
    cout << "PREFERENCIAS" << endl;
    Preferencia* p1 = new Preferencia(1,c);
    Preferencia* p2 = new Preferencia(2,d);
    Preferencia* p3 = new Preferencia(3,e);
    Preferencia* p4 = new Preferencia(4,f);
    
    p1->print(cout);
    cout << p1->compare(p2) << endl;
    cout << p2->compare(p1) << endl;
    cout << p2->compare(p2) << endl;
    cout << p1->compare(p1) << endl;
    cout << p2->compare(p3) << endl;
    cout << p4->compare(p1) << endl;
    cout << p2->compare(p3) << endl;
    cout << p3->compare(p1) << endl;
#endif
#if 0
    candidato* c = new candidato[4];
    candidato cc[] = {0, 2, 3, 1};
    memcpy(c, cc, 4*sizeof(candidato));

    candidato* d = new candidato[4];
    candidato dd[] = {3, 2, 0, 1};
    memcpy(d, dd, 4*sizeof(candidato));

    candidato* e = new candidato[4];
    candidato ee[] = {3, 2, 1, 0};
    memcpy(e, ee, 4*sizeof(candidato));

    Preferencia* p1 = new Preferencia(3,c);
    Preferencia* p2 = new Preferencia(5,d);
    Preferencia* p3 = new Preferencia(3,e);
#endif
#if 0
    candidato* ccc = new candidato[2];
    candidato cccc[] = {0, 1};
    memcpy(ccc, cccc, 2*sizeof(candidato));

    candidato* ddd = new candidato[2];
    candidato dddd[] = {1, 0};
    memcpy(ddd, dddd, 2*sizeof(candidato));

    Preferencia* pp1 = new Preferencia(2,ccc);
    Preferencia* pp2 = new Preferencia(1,ddd);

    Perfil* p0 = new Perfil();
    p0->agregar_preferencia(pp2);
    p0->agregar_preferencia(pp1);

    p0->aplicar_cambio_elemental(0, 0);
    p0->aplicar_cambio_elemental(0, 0);

    candidato* c = new candidato[2];
    candidato cc[] = {0, 1};
    memcpy(c, cc, 2*sizeof(candidato));

    candidato* d = new candidato[2];
    candidato dd[] = {1, 0};
    memcpy(d, dd, 2*sizeof(candidato));

    Preferencia* p1 = new Preferencia(2,c);
    Preferencia* p2 = new Preferencia(1,d);

    Perfil* p = new Perfil();
    p->agregar_preferencia(p2);
    p->agregar_preferencia(p1);

    cout << "N" << endl;
    int** N = p->obtener_N();
    Perfil::imprimir_N(N);
    Perfil::eliminar_N(N);
    
    p->print(cout);
    p->aplicar_cambio_elemental(0, 0);
    p->aplicar_cambio_elemental(0, 0);
    p->print(cout);

    cout << p->compare(*p0) << endl;
#endif
#if 0
    Perfil* q = new Perfil(*p);

    cout << "Copia de p" << endl;
    q->print(cout);

	cout << " voy a cambiar " << endl;
	q->aplicar_cambio_elemental(0,2);

	q->print(cout);

    cout << "Primer p" << endl;
    p->print(cout);

    delete p;
    delete q;
#endif
}
