
#include "Celda.hpp"
using namespace irr;

Grafico::Celda::Celda(scene::ISceneManager* smgr):Grafico::Pieza(),Reglas::Celda() {
       this->mesh=smgr->getMesh("Texturas/Celda.3ds");
      this->dibuja(smgr);
}

Grafico::Celda::Celda(const Celda& orig) {
}

Grafico::Celda::~Celda() {
}

