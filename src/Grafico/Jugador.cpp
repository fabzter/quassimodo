
#include <IAnimatedMeshSceneNode.h>

#include "Jugador.hpp"
using namespace irr;

Grafico::Jugador::Jugador(scene::ISceneManager *smgr, int num,
                          Reglas::Agente *a, Skin *skin, int VelAnim,
                          scene::ISceneNode *parent)
    : Grafico::Pieza(parent), Reglas::Jugador(num, a) {

  if (num == 0)
    this->mesh = skin->getJugador1();
  else
    this->mesh = skin->getJugador2();
  this->dibuja(smgr);
  this->getNodo()->getMaterial(0).Shininess = 20.0f;
  this->velAnim = VelAnim;
  this->anim = nullptr;
}

Grafico::Jugador::Jugador(const Jugador &orig)
    : Grafico::Pieza(orig), Reglas::Jugador(orig) {
  this->anim = nullptr;
}

Grafico::Jugador::~Jugador() {
  if (this->anim)
    delete this->anim;
}

void Grafico::Jugador::Gira(core::vector3df giro) {
  this->nodoA->setRotation(giro);
}

void Grafico::Jugador::Mover(scene::ISceneManager *smgr, core::vector3df npos,
                             int numceldas) {
  if (this->anim) {
    delete this->anim;
    this->anim = nullptr;
  }
  this->anim = new JumpAnimator(this->getPosicionEscena(), npos, this->velAnim,
                                static_cast<f32>(numceldas), this->nodoA);
  this->posiciong = npos;
}

bool Grafico::Jugador::endAnimacion(u32 timeMs) {
  if (!this->anim)
    return true;
  this->anim->update(timeMs);
  return this->anim->hasFinished();
}
