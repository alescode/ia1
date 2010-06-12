//LINEA 56!
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
        Preferencia(int v, candidato* o) {
            this->votantes_preferencia = v;
            this->orden = o;
        }

        ~Preferencia() {
            cout << "Destruyendo " << this->votantes_preferencia << endl;
            delete [] this->orden;
        }

        void print(ostream& os, int size) {
            os << this->votantes_preferencia << endl;

            for (int i = 0; i < size; i++) {
                os << (int) this->orden[i] << " ";
            }
            os << endl;
        }

    private:
        int votantes_preferencia;
        candidato* orden;
};

class Perfil {
    public:
        Perfil (int c, int v, int capacidad) {
            this->numero_candidatos = c;
            this->numero_votantes = v;
            this->info = new vector<Preferencia>;
            (this->info)->reserve(capacidad);
        }

        ~Perfil () {
            delete this->info;
        }

        void print(ostream& os, int candidatos) {
            os << "Perfil: " << endl;
            vector<Preferencia>* preferencias = this->info;
            vector<Preferencia> interno = *preferencias;
            int size = (this->info)->size();
            for (int i = 0; i < size; i++)
               (*(this->info))[i].print(cout, candidatos);
        }

        void agregar_preferencia(Preferencia& p) {
            (this->info)->push_back(p);
        }

    private:
        int numero_candidatos;
        int numero_votantes;
        vector<Preferencia>* info;
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

    Perfil* p = new Perfil(4, 10, 4);
    p->agregar_preferencia(*p1);
    p->agregar_preferencia(*p2);
    p->agregar_preferencia(*p3);
    p->agregar_preferencia(*p4);

    p->print(cout, 4);

    cout << "HOLA" << endl; 

    p1->print(cout, 4);
    p2->print(cout, 4);
    p3->print(cout, 4);
    p4->print(cout, 4);

    delete p;
}
