

#include "Juego.hpp"
using namespace irr;
Juego::Juego(scene::ISceneManager* smgr,gui::IGUIEnvironment* env) {

    this->manj=new ManejadorJuego(smgr,env);
    this->event=new EventReceiver(this->manj);
    this->manj->setEscala(5,5,5);
}

Juego::Juego(const Juego& orig) {
}

Juego::~Juego() {
}
EventReceiver* Juego::getEventReceiver(){
     return this->event;
 }
