#include "Casillero.h"


Casillero::Casillero(){
    this->ficha = NULL;
    this->estado = CASILLERO_DESBLOQUEADO;
    this->terreno = TIERRA;
    
    this->casillerosAdyacentes = new Casillero *** [3];
    for (int i = 0; i < 3; i++) {
        this->casillerosAdyacentes[i] = new Casillero ** [3];
        for (int j = 0; j < 3; j++) {
            this->casillerosAdyacentes[i][j] = new Casillero * [3];
        }
    }
    
}


Casillero::Casillero(Ficha * nuevaFicha){

    this->ficha = nuevaFicha;
    this->estado = CASILLERO_DESBLOQUEADO;

    this->casillerosAdyacentes = new Casillero *** [3];
    for (int i = 0; i < 3; i++) {
        this->casillerosAdyacentes[i] = new Casillero ** [3];
        for (int j = 0; j < 3; j++) {
            this->casillerosAdyacentes[i][j] = new Casillero * [3];
        }
    }
}


Casillero::~Casillero() {

    if ( this->ficha ) {
        delete this->ficha;
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            delete[] this->casillerosAdyacentes[i][j];
        }
        delete[] this->casillerosAdyacentes[i];
    }
    delete[] this->casillerosAdyacentes;
}


void Casillero::asignarCasilleroAdyacente(int x, int y, int z, Casillero * casilleroAdyacente) {
    if ((x < -1 || x > 1) || (y < -1 || y > 1) || (z < -1 || z > 1)){
        throw "Coordenadas invalidas. Se toma el mismo casillero como origen por lo tanto"
              "las coordenadas deben estan comprendidas entre -1 y 1";
    }


    this->casillerosAdyacentes[x+1][y+1][z+1] = casilleroAdyacente;
}


bool Casillero::tieneAdyacente(unsigned int x, unsigned int y, unsigned int z) {
    //Puede que este mal
    return (this->casillerosAdyacentes[x][y][z] != NULL);
}


bool Casillero::esAdyacenteLineal( Casillero * casillero ){

    Casillero * ca;
    for (int k = 0; k < 3; ++k) {
        for (int j = 0; j < 3; ++j) {
            for (int i = 0; i < 3; ++i) {
                ca = this->getAdyacente(i, j, k);
                if (ca == casillero) {
                	if ( ( k==1 && j==1 && i!=1 ) ||
                		 ( k==1 && j!=1 && i==1 ) ||
						 ( k!=1 && j==1 && i==1 ) ) {
                		return true;
                	}
                }
            }
        }
    }
    return false;
}


Casillero * Casillero::getAdyacente(unsigned int i, unsigned int j, unsigned int k) {
    Casillero * c = this->casillerosAdyacentes[i][j][k];
    return c;
}


unsigned int Casillero::getLongitudFichasIguales(unsigned int i, unsigned int j, unsigned int k){

	if (!this->tieneAdyacente(i, j, k) ||
        !this->ficha ||
        ( i==1 && j==1 && k==1 ) ){
        return 0;
    }


    Casillero * casilleroAdyacente = this->getAdyacente(i, j, k);

	try {
		if (this->tienenMismaFicha(casilleroAdyacente)){
			return (1 + casilleroAdyacente->getLongitudFichasIguales(i, j, k));
		}
	}
	catch (...) {
	}

    return 0;
}


Ficha * Casillero::getFicha(){
	if ( !(this->ficha) ) {
		throw("El casillero esta vacio, no se puede obtener una ficha.");
	}
    return this->ficha;
}


void Casillero::setFicha(Ficha * nuevaFicha) {
    if( this->ficha || this->estaBloqueado() ) {
        throw("No se puede poner una ficha en el casillero ocupado o bloqueado");
    }

    this->ficha = new Ficha(nuevaFicha);
}


Ficha * Casillero::quitarFicha() {

    if ( !(this->ficha) ) {
        throw("No hay ficha para quitar");
    }
    Ficha * ficha = this->ficha;
    this->ficha = NULL;
    return ficha;
}


bool Casillero::estaBloqueado() {
    return (this->estado == CASILLERO_BLOQUEADO);
}


void Casillero::bloquear() {
    this->estado = CASILLERO_BLOQUEADO;
}


void Casillero::desbloquear() {
    this->estado = CASILLERO_DESBLOQUEADO;
}


bool Casillero::tienenMismaFicha( Casillero * casilleroAdyacente ) {

    return ( this->ficha->esIgual( casilleroAdyacente->getFicha() ) );

}


bool Casillero::estaVacio() {

    if ( this->ficha ) {
        return false;
    }

    return true;
}