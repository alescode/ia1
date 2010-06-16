#include "Preferencia.h"

Preferencia::Preferencia(int v, candidato* o) {
    this->votantes_preferencia = v;
    this->orden = o;
}

//unitaria inicializa preferencia con votantes = 1
Preferencia::Preferencia(const Preferencia& p, bool unitaria) {
    if (unitaria) {
        this->votantes_preferencia = 1;
    }
    else {
        this->votantes_preferencia = p.votantes_preferencia;
    }
    this->orden = new candidato[num_candidatos];

    memcpy(this->orden, p.orden, num_candidatos*sizeof(candidato));
}

Preferencia::~Preferencia() {
    //cout << "Destruyendo " << this->votantes_preferencia << endl;
    delete [] this->orden;
}

void Preferencia::print(ostream& os) {
    os << this->votantes_preferencia;

    for (int i = 0; i < num_candidatos; i++) {
        os << " " << (int) this->orden[i];
    }
    os << endl;
}

void Preferencia::print2(ostream& os, string candidatos[250]) {
    os << this->votantes_preferencia;

    for (int i = 0; i < num_candidatos; i++) {
        os << " " << candidatos[(int) this->orden[i]];
    }
    os << endl;
}

void Preferencia::resetear(int size) {
    memset(this->orden, 0, size);
}

//Preferencia& operator= (Preferencia& p) {}

int Preferencia::obtener_votantes(){
    return this->votantes_preferencia;
}

candidato* Preferencia::obtener_orden(){
    return this->orden;
}

 void Preferencia::crementar(int signo){
    this->votantes_preferencia += signo;
}

void Preferencia::cambio_elemental(unsigned char fila){
    candidato tmp = this->orden[fila];
    this->orden[fila] = this->orden[fila+1];
    this->orden[fila+1] = tmp;
    this->votantes_preferencia = 1;
}

int Preferencia::compare(Preferencia& p){
    /* Verificamos que cada preferencia tenga igual permutacion de candidatos */
    int dif = memcmp(obtener_orden(), p.obtener_orden(), num_candidatos);

    return dif;
}

int Preferencia::compare2(Preferencia& p){
    int dif = this->votantes_preferencia - p.votantes_preferencia;

    if (dif) return dif;

    /* Verificamos que cada preferencia tenga igual permutacion de candidatos */
    dif = memcmp(obtener_orden(), p.obtener_orden(), num_candidatos);

    return dif;
}
