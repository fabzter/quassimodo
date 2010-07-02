

#include <irrlicht/IAnimatedMeshSceneNode.h>
#include <irrlicht/IShadowVolumeSceneNode.h>

#include "Jugador.hpp"
using namespace irr;

Grafico::Jugador::Jugador(scene::ISceneManager* smgr,int num, Reglas::Agente *a,scene::IAnimationEndCallBack* callback,Skin* skin):Grafico::Pieza() ,Reglas::Jugador(num,a) {

    if (num==0)
        this->mesh=skin->getJugador1();
    else
        this->mesh=skin->getJugador2();
      this->dibuja(smgr);
      //this->callback=callback;
      this->setSombra(skin->getSombraJugador());
      this->getNodo()->getMaterial(0).Shininess=20.0f;

}

Grafico::Jugador::Jugador(const Jugador& orig) :Grafico::Pieza(orig) ,Reglas::Jugador(orig){
}

Grafico::Jugador::~Jugador() {
    
   // delete this->callback;
    //this->nodoA->removeAnimators();

}
 void Grafico::Jugador::Gira(core::vector3df giro){
     this->nodoA->setRotation(giro);
 }

 bool Grafico::Jugador::Mover(scene::ISceneManager* smgr,core::vector3df npos){

      // this->nodoA->removeAnimators();
    /*  scene::ISceneNodeAnimator* anim =smgr->createFlyStraightAnimator(this->getPosicionEscena() ,npos,250,false,false);
      if (anim)
		{
			this->nodoA->addAnimator(anim);
			anim->drop();
      }*/
      this->setPosicion(npos);
     /*core::list<scene::ISceneNodeAnimator*  >::ConstIterator a=this->sombra->getAnimators().begin() ;
     scene::ISceneNodeAnimatorCollisionResponse* anm = (scene::ISceneNodeAnimatorCollisionResponse*) *a;
    anm->setTargetNode(this->sombra);*/
      //this->posiciong=npos;
     //  this->nodoA->setAnimationEndCallback(this->callback);
      return true;
     
  }

