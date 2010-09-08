#include <irrlicht/SMaterial.h>
#include <irrlicht/SMaterialLayer.h>
#include <irrlicht/IAnimatedMeshSceneNode.h>
#include <irrlicht/IMesh.h>
#include <irrlicht/IAnimatedMesh.h>
#include <irrlicht/ISceneNode.h>
#include <irrlicht/SColor.h>

#include "Barrera.hpp"

using namespace irr;

Grafico::Barrera:: Barrera(scene::ISceneManager* smgr,Skin* skin,int VelAnim,scene::ISceneNode* parent,int idJugador):Grafico::Pieza(parent),Reglas::Barrera() {
    if(idJugador==0)
        this->mesh=skin->getBarrera1();
    else
        this->mesh=skin->getBarrera2();
     this->dibuja(smgr);
     if(idJugador==0){
         this->nodoA->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
  
     }
     this->velAnim=VelAnim;
     //  this->setSombra();
}

Grafico::Barrera::Barrera(const Barrera& orig):Grafico::Pieza(orig),Reglas::Barrera(orig) {
    this->velAnim=orig.velAnim;
}

Grafico::Barrera::~Barrera() {
    
}

core::vector3df Grafico::Barrera::giraEste(core::vector3df pos){

    if(this->nodoA->getRotation().Y==0){
        this->nodoA->setRotation(core::vector3df(0,90,0));
        core::vector3df p=pos;
        p.X+=this->size.X*this->getEscala().X;
        return p;
    }
}

void Grafico::Barrera::giraNorte(){
    if(this->nodoA->getRotation().Y==90){
        core::vector3df p=this->getPosicionEscena();
        this->nodoA->setRotation(core::vector3df(0,0,0));
        
        // p.X-=this->size.X*this->getEscala().X;
        this->setPosicion(p);
    }
}
void Grafico::Barrera::ColocaBarrera(irr::core::vector3df posg, const std::vector<int>& pos, Reglas::Direccion dir,scene::ISceneManager* smgr,bool animacion){

    core::vector3df pos_final=core::vector3df( posg.X-(this->getSize().X*this->getEscala().X) , posg.Y, posg.Z );
    core::vector3df pos_ini=pos_final;
    if(dir==Reglas::ESTE)
        pos_final=pos_ini=this->giraEste(pos_final);
    //poneos a la posicion inicial debajo del tablero.
    pos_ini.Y-=( ( this->getSize().Y*this->getEscala().Y ) +5 );
    if(animacion){
    this->nodoA->removeAnimators();
    scene::ISceneNodeAnimator* anim =smgr->createFlyStraightAnimator(pos_ini ,pos_final,this->velAnim,false,false);
    if (anim)
    {
        this->nodoA->addAnimator(anim);
	anim->drop();
    }
    this->posiciong=pos_final;
    }
    else{
        this->setPosicion(pos_final);
    }
    //this->setPosicion( posg.X-(this->getSize().X*this->getEscala().X) , posg.Y, posg.Z);
    this->colocar(pos,dir);
}

bool Grafico::Barrera::endAnimacion(){

     if(this->nodoA->getAnimators().empty()){
         return true;
     }
     else{
        core::list<scene::ISceneNodeAnimator*  >::ConstIterator a=this->nodoA->getAnimators().begin() ;
        scene::ISceneNodeAnimator* anim=*a;
        return anim->hasFinished();
     }
 }
