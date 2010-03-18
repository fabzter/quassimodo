
#include "Barrera.hpp"
using namespace irr;

Grafico::Barrera:: Barrera(scene::ISceneManager* smgr) {

      this->mesh=smgr->getMesh("Texturas/Antorcha2.3ds");
    this->dibuja(smgr);
}

Grafico::Barrera::Barrera(const Barrera& orig) {
}

Grafico::Barrera::~Barrera() {
}

