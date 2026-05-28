
#include <ISceneNode.h>

#include "Pieza.hpp"
using namespace irr;
using namespace std;

Grafico::Pieza::Pieza(scene::ISceneNode* parent) {
    this->mesh=NULL;
    this->posiciong=core::vector3df(0,0,0);
    this->nodoA=NULL;
    this->size.X=0, this->size.Y=0, this->size.Z=0;
    this->sombra=NULL;
    this->par=parent;
   
}

Grafico::Pieza::Pieza(const Pieza& orig) {
    this->nodoA=orig.nodoA;
    this->posiciong=orig.posiciong;
    this->mesh=orig.mesh;
}

Grafico::Pieza::~Pieza() {
     this->drop();
}

void Grafico::Pieza::setPosicion(core::vector3df posicion){
      this->setPosicion(posicion.X, posicion.Y ,posicion.Z);

}

void Grafico::Pieza::setPosicion(int x, int y ,int z){
        this->posiciong.X=x;
        this->posiciong.Y=y;
        this->posiciong.Z=z;
        this->nodoA->setPosition( this->posiciong );
}

scene::IAnimatedMeshSceneNode* Grafico::Pieza::getNodo(){
        return this->nodoA;
}

core::vector3df Grafico::Pieza::getPosicionEscena(){
    this->nodoA->updateAbsolutePosition();
        return this->nodoA->getPosition();
}

void Grafico::Pieza::dibuja(scene::ISceneManager* smgr){

    this->nodoA=smgr->addAnimatedMeshSceneNode(this->mesh );
    this->nodoA->getMaterial(0).MaterialType = video::EMT_SOLID;
    this->nodoA->getMaterial(0).Lighting = true;
    // mesh-level material flag removed — set on the scene node instead
    this->nodoA->getMaterial(0).NormalizeNormals = true;
    this->nodoA->setPosition( this->posiciong );
    this->size =this->nodoA->getBoundingBox().getExtent();

    this->nodoA->getMesh()->setHardwareMappingHint(scene::EHM_STATIC);

    this->nodoA->setAutomaticCulling(scene::EAC_FRUSTUM_BOX);
     this->nodoA->setParent(par);

}

void Grafico::Pieza::setEscala(int x, int y ,int z){

     this->nodoA->getMaterial(0).NormalizeNormals = true;
     if(this->sombra!=NULL){
         this->sombra->getMaterial(0).Lighting = false;
     }
}

void Grafico::Pieza::setMesh(scene::IAnimatedMesh* mesh){
    this->mesh=mesh;
}

core::vector3df Grafico::Pieza::getSize(){

    return this->nodoA->getMesh()->getBoundingBox().getExtent();
}

core::vector3df  Grafico::Pieza::getEscala(){
    this->nodoA->updateAbsolutePosition();
    return this->nodoA->getAbsoluteTransformation().getScale();
}

void Grafico::Pieza::drop(){
    
     if(this->sombra!=NULL){
         this->nodoA->removeChild(this->sombra);
     }
    this->par->removeChild(this->nodoA);
}

 void Grafico::Pieza::setSombra(scene::IMesh* shadowMesh){
    // No-op: IShadowVolumeSceneNode removed in IrrlichtMt
    this->sombra = NULL;
    }
scene::ISceneNode* Grafico::Pieza::getNodoSombra(){
     return this->sombra;
 }