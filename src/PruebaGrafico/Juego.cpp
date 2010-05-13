

#include "Juego.hpp"
using namespace irr;
Juego::Juego(scene::ISceneManager* smgr,gui::IGUIEnvironment* env) {

    this->manj=new ManejadorJuego(smgr,env);
    this->event=new EventReceiver(this->manj);
}

Juego::Juego(const Juego& orig) {
}

Juego::~Juego() {
}
EventReceiver* Juego::getEventReceiver(){
     return this->event;
 }
