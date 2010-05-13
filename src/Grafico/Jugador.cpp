

#include "Jugador.hpp"
using namespace irr;

Grafico::Jugador::Jugador(scene::ISceneManager* smgr,int num, Reglas::Agente *a,scene::IAnimationEndCallBack* callback,Skin* skin):Grafico::Pieza() ,Reglas::Jugador(num,a) {
      this->mesh=skin->getJugador1();
      this->dibuja(smgr);
      this->callback=callback;
    
}

Grafico::Jugador::Jugador(const Jugador& orig) :Grafico::Pieza(orig) ,Reglas::Jugador(orig){
}

Grafico::Jugador::~Jugador() {

}
 void Grafico::Jugador::Gira(core::vector3df giro){
     this->nodoA->setRotation(giro);
 }

 bool Grafico::Jugador::Mover(scene::ISceneManager* smgr,core::vector3df npos){

      // this->nodoA->removeAnimators();
      scene::ISceneNodeAnimator* anim =smgr->createFlyStraightAnimator(this->getPosicionEscena() ,npos,250,false,false);
      if (anim)
		{
			this->nodoA->addAnimator(anim);
			anim->drop();
      }

      this->posiciong=npos;
       this->nodoA->setAnimationEndCallback(this->callback);
      return true;
     
  }

