

#include "Tablero.hpp"
#include "Celda.hpp"
#include "Skin.hpp"
using namespace irr;

Grafico::Tablero::Tablero(scene::ISceneManager* smgr,Skin *skin) : Pieza(), Reglas::Tablero(){
    //le damos el tamaño vertical
    this->datos.reserve(this->size_y);
    this->datos.resize(this->size_y);

    //ahora el tamaño horizontal
    for(std::size_t i = 0; i < this->datos.size(); i++){
        this->datos.at(i).reserve(this->size_x);
        this->datos.at(i).resize(this->size_x);
    }
    this->DibujaTodo(smgr,skin);

}

void Grafico::Tablero::DibujaTodo(scene::ISceneManager* smgr,Skin *skin){
    //dibujamos el tablero
      this->mesh=skin->getTablero();
      this->dibuja(smgr);
       this->nodoA->setMaterialTexture(0, skin->getTTAblero() );
       this->nodoA->getMaterial(0).SpecularColor.set(0,0,0,0);

      //dibujamos las celdas y le damos su posición a cada celda...
    for(std::size_t i = 0; i < this->datos.size(); i++){
        for(std::size_t j = 0; j < this->datos.at(i).size(); j++){
            this->datos.at(i).at(j)=new Celda(smgr,skin);
            this->datos.at(i).at(j)->setCeldaR( &this->getCelda(j,i));
           this->datos.at(i).at(j)->colocar(0,this->size.Y,0);
        }
    }
      this->setPosicion(0,0,0);

}
Grafico::Tablero::Tablero(const Tablero& orig): Pieza(orig), Reglas::Tablero(orig) {

}
Grafico::Tablero::~Tablero(){

     for(std::size_t i = 0; i < this->datos.size(); i++){
        for(std::size_t j = 0; j < this->datos.at(i).size(); j++){
            delete( this->datos.at(i).at(j) );
        }
        this->datos.at(i).clear();
    }
     this->datos.clear();
    
}
void Grafico::Tablero::setPosicionTablero(int x,int y,int z){

     for(std::size_t i = 0; i < this->datos.size(); i++){
        for(std::size_t j = 0; j < this->datos.at(i).size(); j++){
           this->datos.at(i).at(j)->colocar(x,y+ ( (this->size.Y) *this->getEscala().Y ) ,z);
        }
    }
    this->setPosicion(x,y,z);
}

void Grafico::Tablero::setEscalaTablero(int x,int y,int z){
     core::vector3df v= this->getPosicionTablero();
    this->setEscala(x,y,z);

    for(std::size_t i = 0; i < this->datos.size(); i++){
        for(std::size_t j = 0; j < this->datos.at(i).size(); j++){
           this->datos.at(i).at(j)->SetEscalaCelda(x,y,z);
        }
    }

    this->setPosicionTablero(v.X,v.Y,v.Z);
}
 irr::core::vector3df Grafico::Tablero::getPosicionTablero(){
      return this->getPosicionEscena();
 }
  irr::core::vector3df Grafico::Tablero::getPosicionCelda(int x,int y){
      return this->datos.at(y).at(x)->getPosicionEscena();
  }
  irr::core::vector3df Grafico::Tablero::getPosicionCelda(std::vector<int> pos){
      return this->getPosicionCelda(pos.at(0),pos.at(1));
  }

  irr::core::vector3df Grafico::Tablero::getsizeLineaCeldas(){

     irr::core::vector3df v;
     v=this->getPosicionCelda(8,8);
     v.X+=( this->datos.at(0).at(0)->getSize().X*this->getEscala().X );
     v.Z+=( this->datos.at(0).at(0)->getSize().Z*this->getEscala().Z );

      return v;
  }

  irr::core::vector3df Grafico::Tablero::getsizeCelda(){
      return this->datos.at(0).at(0)->getSize();
  }