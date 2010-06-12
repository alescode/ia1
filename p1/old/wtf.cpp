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
        vector<Preferencia>* info;

        Perfil (int c, int v) {
            this->numero_candidatos = c;
            this->numero_votantes = v;
            this->info = new vector<Preferencia>;
        }

        Perfil (const Perfil& p) {
            this->numero_candidatos = p.numero_candidatos;
            this->numero_votantes = p.numero_votantes;
            this->info = new vector<Preferencia>(*(p.info));
        }

        void print(ostream& os, int candidatos) {
            os << "Perfil: " << endl;
            vector<Preferencia> preferencias = *(this->info);
            int size = preferencias.size();
            for (int i = 0; i < size; i++)
               preferencias[i].print(cout, candidatos);
        }

        candidato calcular_ganador_dogson() {
            int mitad_votantes = this->numero_votantes >> 1;
            int candidatos = this->numero_candidatos;
            vector<Preferencia> preferencias = *(this->info);

            //N(x, y): Número de votantes que prefieren a x sobre y
            int N[MAX_CANDIDATOS][MAX_CANDIDATOS];
            memset(N, 0, sizeof(N));
            
            for (int i = 0; i < preferencias.size(); i++) {
                int votantes_p = preferencias[i].votantes_preferencia;
                candidato* preferencia = preferencias[i].orden;

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
            vector<Preferencia> h = *(this->info);
            Preferencia p = h[num_preferencia];
            int n = p.votantes_preferencia;
            candidato* c = p.orden;
            
            c[num_candidato] = 7; 
            
            //Perfil
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

    cout << "PREFERENCIAS" << endl;
    Preferencia* p1 = new Preferencia(1,c);
    Preferencia* p2 = new Preferencia(2,d);
    Preferencia* p3 = new Preferencia(3,e);
    Preferencia* p4 = new Preferencia(4,f);

    p1->print(cout, 4);
    p2->print(cout, 4);
    p3->print(cout, 4);
    p4->print(cout, 4);

    Perfil* p = new Perfil(4, 12);

    (*p->info).push_back(*p1);
    (*p->info).push_back(*p2);

    p->print(cout, 4);
#if 0
    int x = p.calcular_ganador_dogson();
    
    cout << "WINNER: " << x << endl;
    p.print(cout);

    Perfil p0 = Perfil(p);

    p0.aplicar_cambio_elemental(0, 0);

    p.print(cout);
#endif
    return(0);
}

