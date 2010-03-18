
#include "Pieza.hpp"
using namespace irr;
using namespace std;

Grafico::Pieza::Pieza() {
    this->mesh=NULL;
    this->posicion=core::vector3df(0,0,0);
    this->nodoA=NULL;
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

core::vector3df Grafico::Pieza::getPosicion(){
        return this->posicion;
    }

void Grafico::Pieza::dibuja(scene::ISceneManager* smgr){

                this->nodoA=smgr->addAnimatedMeshSceneNode( mesh );
                this->nodoA->setMaterialType(video::EMT_SOLID);
		this->nodoA->setMaterialFlag(video::EMF_LIGHTING, true);
                this->nodoA->setPosition( this->posicion );
}
