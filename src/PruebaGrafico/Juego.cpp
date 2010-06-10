

#include "Juego.hpp"
using namespace irr;
Juego::Juego(scene::ISceneManager* smgr,gui::IGUIEnvironment* env) {
    this->manj=new ManejadorJuego(smgr,env);
    this->manj->setEscala(5,5,5);
    this->setSalir(false);
}

Juego::Juego(const Juego& orig) {
}

Juego::~Juego() {
}
bool Juego::getSalir(){
    return this->salir;
}
void Juego::setSalir(bool valor){
    this->salir=valor;
}
ManejadorJuego* Juego::getManJ(){
    return this->manj;
}
