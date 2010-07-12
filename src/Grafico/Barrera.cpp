

#include "Barrera.hpp"

using namespace irr;

Grafico::Barrera:: Barrera(scene::ISceneManager* smgr,Skin* skin):Grafico::Pieza(),Reglas::Barrera() {

      this->mesh=skin->getBarrera();
     this->dibuja(smgr);
     this->nodoA->setMaterialTexture( 0, skin->getTBarrera() );
     this->nodoA->getMaterial(0).SpecularColor.set(0,0,0,0);
     //  this->setSombra();
}

Grafico::Barrera::Barrera(const Barrera& orig):Grafico::Pieza(orig),Reglas::Barrera(orig) {
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
        this->nodoA->setRotation(core::vector3df(0,0,0));
        core::vector3df p=this->getPosicionEscena();
        // p.Z+=this->size.X;
         p.X-=this->size.X*this->getEscala().X;
        this->setPosicion(p);
    }
}
void Grafico::Barrera::ColocaBarrera(irr::core::vector3df posg, const std::vector<int>& pos, Reglas::Direccion dir,scene::ISceneManager* smgr){

    core::vector3df pos_final=core::vector3df( posg.X-(this->getSize().X*this->getEscala().X) , posg.Y, posg.Z );
    core::vector3df pos_ini=pos_final;
    if(dir==Reglas::ESTE)
        pos_final=pos_ini=this->giraEste(pos_final);
    //poneos a la posicion inicial debajo del tablero.
    pos_ini.Y-=( ( this->getSize().Y*this->getEscala().Y ) +5 );
    
    this->nodoA->removeAnimators();
    scene::ISceneNodeAnimator* anim =smgr->createFlyStraightAnimator(pos_ini ,pos_final,300,false,false);
    if (anim)
    {
        this->nodoA->addAnimator(anim);
	anim->drop();
    }
    this->posiciong=pos_final;
    //this->setPosicion( posg.X-(this->getSize().X*this->getEscala().X) , posg.Y, posg.Z);
    this->colocar(pos,dir);
}

bool Grafico::Barrera::endAnimacion(){

     core::list<scene::ISceneNodeAnimator*  >::ConstIterator a=this->nodoA->getAnimators().begin() ;
     scene::ISceneNodeAnimator* anim=*a;
     if(anim==NULL||anim==0){
         return true;
     }
     else{
        return anim->hasFinished();
     }
 }
