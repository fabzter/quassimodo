

#include "Tablero.hpp"
using namespace irr;

Grafico::Tablero::Tablero(scene::ISceneManager* smgr) : Pieza(), Reglas::Tablero(){

    this->mesh=smgr->getMesh("Texturas/Antorcha2.3ds");
    this->dibuja(smgr);

}

Grafico::Tablero::Tablero(const Tablero& orig): Pieza(orig), Reglas::Tablero(orig) {

}


Grafico::Tablero::~Tablero() {
}

