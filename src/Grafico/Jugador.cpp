

#include <IAnimatedMeshSceneNode.h>

#include "Jugador.hpp"
using namespace irr;

Grafico::Jugador::Jugador(scene::ISceneManager* smgr,int num, Reglas::Agente *a,Skin* skin,int VelAnim,scene::ISceneNode* parent):Grafico::Pieza(parent) ,Reglas::Jugador(num,a) {

    if (num==0)
        this->mesh=skin->getJugador1();
    else
        this->mesh=skin->getJugador2();
      this->dibuja(smgr);
      this->getNodo()->getMaterial(0).Shininess=20.0f;
      this->velAnim=VelAnim;

}

Grafico::Jugador::Jugador(const Jugador& orig) :Grafico::Pieza(orig) ,Reglas::Jugador(orig){
}

Grafico::Jugador::~Jugador() {
}

 void Grafico::Jugador::Gira(core::vector3df giro){
     this->nodoA->setRotation(giro);
 }

 void Grafico::Jugador::Mover(scene::ISceneManager* smgr,core::vector3df npos,int numceldas){
     // D2.1 KB-D2-004: animator subsystem removed from IrrlichtMt fork.
     // Instant placement replaces the JumpAnimator arc animation.
     this->setPosicion(npos);
 }

 bool Grafico::Jugador::endAnimacion(){
     // D2.1: no pending animation with instant moves.
     return true;
 }
