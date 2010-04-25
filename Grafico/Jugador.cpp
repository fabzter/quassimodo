

#include "Jugador.hpp"
using namespace irr;

Grafico::Jugador::Jugador(scene::ISceneManager* smgr,int num, Reglas::Agente *a):Grafico::Pieza() ,Reglas::Jugador(num,a) {
        this->mesh=smgr->getMesh("Texturas/Mono.3ds");
      this->dibuja(smgr);
}

Grafico::Jugador::Jugador(const Jugador& orig) :Grafico::Pieza(orig) ,Reglas::Jugador(orig){
}

Grafico::Jugador::~Jugador() {

}
 void Grafico::Jugador::Gira(core::vector3df giro){
     this->nodoA->setRotation(giro);
 }

