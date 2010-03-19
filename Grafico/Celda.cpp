
#include "Celda.hpp"
using namespace irr;

Grafico::Celda::Celda(scene::ISceneManager* smgr):Grafico::Pieza(){
       this->mesh=smgr->getMesh("Texturas/Celda.3ds");
       this->celdaR=NULL;
      this->dibuja(smgr);
      core::aabbox3d<f32> t=this->nodoA->getBoundingBox();
      this->size= t.getExtent();

     
}

Grafico::Celda::Celda(const Celda& orig):Grafico::Pieza(orig) {
}


Grafico::Celda::~Celda() {
}

core::vector3df Grafico::Celda::getSize(){
    return this->size;
}

  void Grafico::Celda::setCeldaR (const Reglas::Celda *celdaR){
      this->celdaR=celdaR;
  }
  void Grafico::Celda::colocar(){
      int x=0,y=0,z=0;
      x=this->celdaR->getPosicion().at(0)*(this->size.X+this->size.Y);
      z=this->celdaR->getPosicion().at(1)*(this->size.X+this->size.Y);
      y=this->size.Y;

      this->setPosicion(x,y,z);
  }