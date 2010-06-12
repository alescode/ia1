#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string.h>

#define MAX_CANDIDATOS 250
#define NO_GANADOR 255

using namespace std;

typedef unsigned char candidato;

class Preferencia {
    public:
        int votantes_preferencia;
        candidato* orden;

        Preferencia(int v, candidato* o) {
            this->votantes_preferencia = v;
            this->orden = o;
        }

        ~Preferencia() {
            cout << "Destruyendo " << this->votantes_preferencia << endl;
            delete [] this->orden;
        }

        Preferencia(const Preferencia& p, int num_candidatos) {
            candidato* fuente = p.orden;
            this->votantes_preferencia = p.votantes_preferencia;

            this->orden = new candidato[num_candidatos];
            memcpy(this->orden, p.orden, num_candidatos * sizeof(unsigned char));
        }

#if 0
        /* Asumiendo que los tamaños de this y p son iguales */
        bool operator< (Preferencia& p) {
            int size = sizeof(this->orden);

            for (int i = 0; i < size; i++) {
                candidato cand_this = this->orden[i];
                candidato cand_p = p.orden[i];
                if (cand_this < cand_p) return true;
                else if (cand_this > cand_p) return false;
            }
            return false;
        }
#endif

        void print(ostream& os, int size) {
            os << this->votantes_preferencia << endl;

            for (int i = 0; i < size; i++) {
                os << (int) this->orden[i] << " ";
            }
            os << endl;
        }
};

class Perfil {
    public:
        int numero_candidatos;
        int numero_votantes;
        vector<Preferencia*>* info;

        Perfil (int c, int v) {
            this->numero_candidatos = c;
            this->numero_votantes = v;
            this->info = new vector<Preferencia*>;
        }

        Perfil (const Perfil& p) {
            this->numero_candidatos = p.numero_candidatos;
            this->numero_votantes = p.numero_votantes;
            this->info = new vector<Preferencia*>(*(p.info));
        }

        void agregar_preferencia(Preferencia* p) {
             (*this->info).push_back(p);
        }

        void print(ostream& os, int candidatos) {
            os << "Perfil: " << endl;
            vector<Preferencia*> preferencias = *(this->info);
            int size = preferencias.size();
            for (int i = 0; i < size; i++)
               preferencias[i]->print(cout, candidatos);
        }

        candidato calcular_ganador_dogson() {
            int mitad_votantes = this->numero_votantes >> 1;
            int candidatos = this->numero_candidatos;
            vector<Preferencia*> preferencias = *(this->info);

            //N(x, y): Número de votantes que prefieren a x sobre y
            int N[MAX_CANDIDATOS][MAX_CANDIDATOS];
            memset(N, 0, sizeof(N));
            
            for (int i = 0; i < preferencias.size(); i++) {
                int votantes_p = preferencias[i]->votantes_preferencia;
                candidato* preferencia = preferencias[i]->orden;

                for (int j = 0; j < candidatos; j++) {
                    for (int k = j+1; k < candidatos; k++) {
                        N[preferencia[j]][preferencia[k]] += votantes_p;
                    }
                }
            }
            
            cout << "MATRIZ " << endl;
            for (int i = 0; i < candidatos; i++) {
                for (int j = 0; j < candidatos; j++) {
                    cout << N[i][j] << " ";
                }
                cout << endl;
            }

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
			bool es_vacia = (--(*info)[num_preferencia]->votantes_preferencia  == 0);

			candidato *orden = new candidato[numero_candidatos];
			memcpy(orden, (*info)[num_preferencia]->orden, numero_candidatos);

			/* Realizamos el cambio elemental */
			unsigned char tmp = orden[num_candidato];
			orden[num_candidato] = orden[num_candidato+1];
			orden[num_candidato+1] = tmp;

			
			/* Verificamos si la preferencia ya se encuentra en memoria */
			int existe = -1;
			for (int i=0; i<(*info).size() && existe == -1;i++){
				if (!memcmp(orden, (*info)[i]->orden, numero_candidatos)){
					existe = i;
				}
			}

			/* Agregamos la nueva preferencia */
			if (existe != -1){
				(*info)[existe]->votantes_preferencia++;
			} else {
				Preferencia *p1 = new Preferencia(1,orden);
				(*info).push_back(p1);
			}

			/* Eliminamos la posible preferencia vacia */
			if (es_vacia){
				//((*info)[num_preferencia])->Preferencia::~Preferencia();
				(*info)[num_preferencia] = (*info)[(*info).size()-1];
				(*info).pop_back();
			}
        }
};

int main(){
	Perfil p(4,10);
	candidato c1[4] = {0,1,2,3};
	candidato c2[4] = {2,1,0,3};
	candidato c3[4] = {1,0,2,3};
	
	Preferencia *p1 = new Preferencia(6,c1);
	Preferencia *p2 = new Preferencia(3,c2);
	Preferencia *p3 = new Preferencia(1,c3);
	
	p.agregar_preferencia(p1);
	p.agregar_preferencia(p2);
	p.agregar_preferencia(p3);
	
	p.print(cout,4);
	cout << endl;
	p.aplicar_cambio_elemental(0,1);
	p.print(cout,4);
	p.aplicar_cambio_elemental(0,2);
	p.print(cout,4);
	p.aplicar_cambio_elemental(0,2);
	p.print(cout,4);
	p.aplicar_cambio_elemental(2,1);
	p.print(cout,4);
	p.aplicar_cambio_elemental(0,0);
	p.aplicar_cambio_elemental(1,3);
	p.aplicar_cambio_elemental(0,3);
	p.print(cout,4);
	return 0;
}