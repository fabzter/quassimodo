

#include <irrlicht/IAnimatedMeshSceneNode.h>
#include <irrlicht/IShadowVolumeSceneNode.h>

#include "Jugador.hpp"
using namespace irr;

Grafico::Jugador::Jugador(scene::ISceneManager* smgr,int num, Reglas::Agente *a,Skin* skin,int VelAnim):Grafico::Pieza() ,Reglas::Jugador(num,a) {

    if (num==0)
        this->mesh=skin->getJugador1();
    else
        this->mesh=skin->getJugador2();
      this->dibuja(smgr);
      //this->callback=callback;
      //this->setSombra(skin->getSombraJugador());
      this->getNodo()->getMaterial(0).Shininess=20.0f;
      this->velAnim=VelAnim;

}

Grafico::Jugador::Jugador(const Jugador& orig) :Grafico::Pieza(orig) ,Reglas::Jugador(orig){
}

Grafico::Jugador::~Jugador() {
    
    //this->nodoA->removeAnimators();

}
 void Grafico::Jugador::Gira(core::vector3df giro){
     this->nodoA->setRotation(giro);
 }

 bool Grafico::Jugador::Mover(scene::ISceneManager* smgr,core::vector3df npos){

      this->nodoA->removeAnimators();
      //new JumpAnimator(node->getPosition(),node1->getPosition(),300.0f,0.0f);
      scene::ISceneNodeAnimator* anim = new JumpAnimator(this->getPosicionEscena() ,npos,this->velAnim,0.f);
      if (anim)
		{
			this->nodoA->addAnimator(anim);

			anim->drop();
      }
      this->posiciong=npos;
      return true;
     
  }
 bool Grafico::Jugador::endAnimacion(){

     if(this->nodoA->getAnimators().empty()){
         return true;
     }
     else{
        core::list<scene::ISceneNodeAnimator*  >::ConstIterator a=this->nodoA->getAnimators().begin() ;
        scene::ISceneNodeAnimator* anim=*a;
        return anim->hasFinished();
     }
 }

