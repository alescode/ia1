#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string.h>

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
        Preferencia(int v, candidato* o) {
            this->votantes_preferencia = v;
            this->orden = o;
        }

        inline Preferencia(const Preferencia& p) {
            this->votantes_preferencia = p.votantes_preferencia;
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

        bool operator< (Preferencia& p) {
            for (int i = 0; i < num_candidatos; i++) {
                candidato cand_this = this->orden[i];
                candidato cand_p = p.orden[i];
                if (cand_this < cand_p) return true;
                else if (cand_this > cand_p) return false;
            }
            return false;
        }

        //Preferencia& operator= (Preferencia& p) {}
        
        int obtener_votantes(){
			return this->votantes_preferencia;
		}
		
		candidato* obtener_orden(){
			return this->orden;
		}

		void crementar(int signo){
			this->votantes_preferencia += signo;
		}
		
		void cambio_elemental(unsigned char fila){
			candidato tmp = this->orden[fila];
			this->orden[fila] = this->orden[fila+1];
			this->orden[fila+1] = tmp;
			this->votantes_preferencia = 1;
		}
};

class Perfil {
	private:
        vector<Preferencia>* info;

    public:
        Perfil (int capacidad = num_votantes) {
            this->info = new vector<Preferencia>;
            (this->info)->reserve(capacidad);
        }

        ~Perfil () {
            delete this->info;
        }

        Perfil (const Perfil& p) {
            this->info = new vector<Preferencia>(*(p.info));
        }

        void print(ostream& os) {
            os << "Perfil: " << endl;
            int size = (this->info)->size();
            for (int i = 0; i < size; i++)
               (*(this->info))[i].print(cout);
        }

        void agregar_preferencia(Preferencia& p) {
            (this->info)->push_back(p);
        }

		void eliminar_preferencia() {
            (this->info)->pop_back();
        }

        void resetear() {
            for (int i = 0; i < (this->info)->size(); i++) {
                (*(this->info))[i].resetear(num_candidatos);
            }
        }

        //Perfil& operator= (Perfil& p) {}

        candidato calcular_ganador_dogson() {
            int mitad_votantes = num_votantes >> 1;
            int candidatos = num_candidatos;
            vector<Preferencia> preferencias = *(this->info);

            //N(x, y): Número de votantes que prefieren a x sobre y
            int N[MAX_CANDIDATOS][MAX_CANDIDATOS];
            memset(N, 0, sizeof(N));
            
            for (int i = 0; i < preferencias.size(); i++) {
                int votantes_p = preferencias[i].obtener_votantes();

                candidato* preferencia = preferencias[i].obtener_orden();

                for (int j = 0; j < candidatos; j++) {
                    for (int k = j+1; k < candidatos; k++) {
                        N[preferencia[j]][preferencia[k]] += votantes_p;
                    }
                }
            }
            /*
            cout << "num_votantes " << num_votantes << endl;
            cout << "MATRIZ " << endl;
            for (int i = 0; i < candidatos; i++) {
                for (int j = 0; j < candidatos; j++) {
                    cout << N[i][j] << " ";
                }
                cout << endl;
            }*/	

            for (int i = 0; i < candidatos; i++) { 
                int j = 0;
                while (j < candidatos && (i == j || N[i][j] > mitad_votantes))
                    j++;
                if (j == candidatos)
                    return i;
            }
            return NO_GANADOR;
        }

        /* Efectúa un cambio elemental entre un candidato y el siguiente
         * en la lista de preferencias indicada por num_preferencia.
         * Devuelve un nuevo perfil idéntico al original exceptuando este cambio. */
        void aplicar_cambio_elemental(candidato num_candidato, int num_preferencia) {

			/* Decrementamos el numero de votantes con esta preferencia */
			(*info)[num_preferencia].crementar(-1);

			bool es_vacia = ((*info)[num_preferencia].obtener_votantes()  == 0);

			Preferencia* p1 = new Preferencia((*info)[num_preferencia]);

			p1->cambio_elemental(num_candidato);

			/* Verificamos si la preferencia ya se encuentra en memoria */
			int existe = -1;

			for (int i=0; i<(*info).size() && existe == -1;i++){
				if (!memcmp(p1->obtener_orden(), (*info)[i].obtener_orden(), num_candidatos)){
					existe = i;
				}
			}

			/* Agregamos la nueva preferencia */
			if (existe != -1){
				(*info)[existe].crementar(1);
				delete p1;
			} else {
				this->agregar_preferencia(*p1);
			}

			/* Eliminamos la posible preferencia vacia */
			if (es_vacia){
				//((*info)[num_preferencia])->Preferencia::~Preferencia();
				//delete &(*info)[num_preferencia];
				(*info)[num_preferencia] = *(new Preferencia((*info)[(*info).size()-1]));
				this->eliminar_preferencia();
			}
        }
        
        int obtener_num_preferencias(){
			return info->size();
		}
};

#if 0
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

    cout << "PREFERENCIAS" << endl;
    Preferencia* p1 = new Preferencia(1,c);
    Preferencia* p2 = new Preferencia(2,d);
    Preferencia* p3 = new Preferencia(3,e);
    Preferencia* p4 = new Preferencia(4,f);

    Perfil* p = new Perfil();
    p->agregar_preferencia(*p1);
    p->agregar_preferencia(*p2);
    p->agregar_preferencia(*p3);
    p->agregar_preferencia(*p4);

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
}
#endif
