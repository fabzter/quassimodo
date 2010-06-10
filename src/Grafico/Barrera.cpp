

#include "Barrera.hpp"

using namespace irr;

Grafico::Barrera:: Barrera(scene::ISceneManager* smgr,Skin* skin) {

      this->mesh=skin->getBarrera();
     this->dibuja(smgr);
     this->nodoA->setMaterialTexture( 0, skin->getTBarrera() );
       this->nodoA->getMaterial(0).SpecularColor.set(0,0,0,0);
      // this->nodoA->addShadowVolumeSceneNode();
}

Grafico::Barrera::Barrera(const Barrera& orig) {
}

Grafico::Barrera::~Barrera() {
}

void Grafico::Barrera::giraEste(){
    if(this->nodoA->getRotation().Y==0){
        this->nodoA->setRotation(core::vector3df(0,90,0));
        core::vector3df p=this->getPosicionEscena();
        p.X+=this->size.X*this->getEscala().X;
        this->setPosicion(p);
    }
}

void Grafico::Barrera::giraNorte(){
    if(this->nodoA->getRotation().Y==90){
        this->nodoA->setRotation(core::vector3df(0,0,0));
        core::vector3df p=this->getPosicionEscena();
        // p.Z+=this->size.X;
         p.X-=this->size.X*this->getEscala().X;
        this->setPosicion(p);
    }
}
void Grafico::Barrera::ColocaBarrera(irr::core::vector3df posg, const std::vector<int>& pos, Reglas::Direccion dir){

    this->setPosicion( posg.X - (this->getSize().X*this->getEscala().X) , posg.Y, posg.Z);
    if(dir==Reglas::ESTE)
        this->giraEste();
    this->colocar(pos,dir);
}
