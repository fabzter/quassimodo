

#include "Partida.hpp"
using namespace irr;
using namespace Grafico;

Partida::Partida(scene::ISceneManager* smgr) {
    this->t=new Tablero(smgr);
    this->antorchas.reserve(4);
    this->antorchas.resize(4);
    this->b=new Barrera(smgr);
     for(std::size_t i = 0; i < this->antorchas.size(); i++){
         this->antorchas.at(i)=new Antorcha(smgr,0,0);
    }
     core::vector3df v=this->t->getPosicionTablero();
     std::cout<<"posicion tablero partida"<<v.X<<","<<v.Y<<","<<v.Z<<std::endl;
     this->ColocaAntorchas();
     this->c=new Celda(smgr);
     this->c->setPosicion(0,0,0);

}

Partida::Partida(const Partida& orig) {
}

Partida::~Partida() {
}
 void Partida::ColocaAntorchas(){

      int x,x1,z,z1,y;
    x=this->t->getPosicionTablero().X+(89*this->t->getEscala().X);
    x1=this->t->getPosicionTablero().X+( 202*this->t->getEscala().X);
    z=this->t->getPosicionTablero().Z+(66*this->t->getEscala().Z);
    z1=this->t->getPosicionTablero().Z+( 185*this->t->getEscala().Z);
    y=this->t->getPosicionTablero().Y*this->t->getEscala().Y;
     this->antorchas.at(0)->setPosicionAntorcha(x,y,z);
     this->antorchas.at(1)->setPosicionAntorcha(x1,y,z);
     this->antorchas.at(2)->setPosicionAntorcha(x1,y,z1);
     this->antorchas.at(3)->setPosicionAntorcha(x,y,z1);
 }
 void Partida::SetEscala(int x, int y, int z){
     this->t->setEscalaTablero(x,y,z);
      for(std::size_t i = 0; i < this->antorchas.size(); i++){
         this->antorchas.at(i)->setEscalaAntorcha(x,y,z);
    }
     this->ColocaAntorchas();
 }
 void Partida::SetBarrera(int x,int y){
     this->b->setPosicion( this->t->getPosicionCelda(x,y) );
 }

