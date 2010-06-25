
#include <irrlicht/ISceneNode.h>

#include "Pieza.hpp"
using namespace irr;
using namespace std;

Grafico::Pieza::Pieza() {
    this->mesh=NULL;
    this->posiciong=core::vector3df(0,0,0);
    this->nodoA=NULL;
    this->size.X=0, this->size.Y=0, this->size.Z=0;
    this->sombra=NULL;
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
        return this->nodoA->getPosition();
}

void Grafico::Pieza::dibuja(scene::ISceneManager* smgr){
                
                this->nodoA=smgr->addAnimatedMeshSceneNode(this->mesh );
                this->nodoA->setMaterialType(video::EMT_DETAIL_MAP);
               this->nodoA->setMaterialFlag(video::EMF_LIGHTING, true);
                this->mesh->setMaterialFlag(video::EMF_NORMALIZE_NORMALS,true);
                this->nodoA->setPosition( this->posiciong );
                 this->size =this->nodoA->getBoundingBox().getExtent();
}

void Grafico::Pieza::setEscala(int x, int y ,int z){

     this->nodoA->setScale(core::vector3df(x,y,z));
     this->nodoA->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
     if(this->sombra!=NULL){
     this->sombra->setScale( core::vector3df(x,y,z) );
     this->sombra->setMaterialFlag(video::EMF_LIGHTING, false);
     }
}

void Grafico::Pieza::setMesh(scene::IAnimatedMesh* mesh){
    this->mesh=mesh;
}

core::vector3df Grafico::Pieza::getSize(){
    return this->size;
}

core::vector3df  Grafico::Pieza::getEscala(){
    return this->nodoA->getScale();
}

void Grafico::Pieza::drop(){
    
     if(this->sombra!=NULL){
       //this->sombra->removeAll();
         this->sombra->remove();
         this->sombra=NULL;
     }
      this->nodoA->remove();
}

 void Grafico::Pieza::setSombra(scene::IMesh* shadowMesh){
    this->sombra=this->nodoA->addShadowVolumeSceneNode(shadowMesh,-1,false);
  
  
 }
scene::IShadowVolumeSceneNode* Grafico::Pieza::getNodoSombra(){
     return this->sombra;
 }