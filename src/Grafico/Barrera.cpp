

#include "Barrera.hpp"

using namespace irr;

Grafico::Barrera::Barrera(scene::ISceneManager *smgr, Skin *skin, int VelAnim,
                          scene::ISceneNode *parent)
    : Grafico::Pieza(parent), Reglas::Barrera() {

  this->mesh = skin->getBarrera();
  this->dibuja(smgr);
  this->nodoA->getMaterial(0).setTexture(0, skin->getTBarrera());
  this->nodoA->getMaterial(0).SpecularColor.set(0, 0, 0, 0);
  this->velAnim = VelAnim;
  //  this->setSombra();
}

Grafico::Barrera::Barrera(const Barrera &orig)
    : Grafico::Pieza(orig), Reglas::Barrera(orig) {}

Grafico::Barrera::~Barrera() {}

core::vector3df Grafico::Barrera::giraEste(core::vector3df pos) {

  if (this->nodoA->getRotation().Y == 0) {
    this->nodoA->setRotation(core::vector3df(0, 90, 0));
    core::vector3df p = pos;
    p.X += this->size.X * this->getEscala().X;
    return p;
  }
  return pos;
}

void Grafico::Barrera::giraNorte() {
  if (this->nodoA->getRotation().Y == 90) {
    this->nodoA->setRotation(core::vector3df(0, 0, 0));
    core::vector3df p = this->getPosicionEscena();
    p.X -= this->size.X * this->getEscala().X;
    this->setPosicion(p);
  }
}

void Grafico::Barrera::ColocaBarrera(irr::core::vector3df posg,
                                     const std::vector<int> &pos,
                                     Reglas::Direccion dir,
                                     scene::ISceneManager *smgr) {

  core::vector3df pos_final = core::vector3df(
      posg.X - (this->getSize().X * this->getEscala().X), posg.Y, posg.Z);
  if (dir == Reglas::ESTE)
    pos_final = this->giraEste(pos_final);

  // D2: IrrlichtMt removed scene node animators. Set position directly.
  this->nodoA->setPosition(pos_final);
  this->posiciong = pos_final;
  Reglas::Barrera::colocar(pos, dir);
}

bool Grafico::Barrera::endAnimacion() {
  // D2: IrrlichtMt removed scene node animators. Always finished.
  return true;
}
