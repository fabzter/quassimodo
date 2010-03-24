
#include "Pieza.hpp"
using namespace irr;
using namespace std;

Grafico::Pieza::Pieza() {
    this->mesh=NULL;
    this->posicion=core::vector3df(0,0,0);
    this->nodoA=NULL;
    this->size.X=0, this->size.Y=0, this->size.Z=0;
}

Grafico::Pieza::Pieza(const Pieza& orig) {
    this->nodoA=orig.nodoA;
    this->posicion=orig.posicion;
    this->mesh=orig.mesh;
}

Grafico::Pieza::~Pieza() {

}

   void Grafico::Pieza::setPosicion(core::vector3df posicion){
      this->setPosicion(posicion.X, posicion.Y ,posicion.Z);

   }

    void Grafico::Pieza::setPosicion(int x, int y ,int z){
        this->posicion.X=x;
        this->posicion.Y=y;
        this->posicion.Z=z;
        this->nodoA->setPosition( this->posicion );
           
   }

    scene::IAnimatedMeshSceneNode* Grafico::Pieza::getNodo(){
        return this->nodoA;
    }

core::vector3df Grafico::Pieza::getPosicionEscena(){
        return this->posicion;
    }

void Grafico::Pieza::dibuja(scene::ISceneManager* smgr){

                this->nodoA=smgr->addAnimatedMeshSceneNode( mesh );
                this->nodoA->setMaterialType(video::EMT_SOLID);
		this->nodoA->setMaterialFlag(video::EMF_LIGHTING, true);
                this->nodoA->setPosition( this->posicion );
}
void Grafico::Pieza::setEscala(int x, int y ,int z){
     this->nodoA->setScale(core::vector3df(x,y,z));
     this->nodoA->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
}
core::vector3df Grafico::Pieza::getSize(){
    return this->size;
}
core::vector3df  Grafico::Pieza::getEscala(){
    return this->nodoA->getScale();
}
