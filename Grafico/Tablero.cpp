

#include "Tablero.hpp"
#include "Celda.hpp"
using namespace irr;

Grafico::Tablero::Tablero(scene::ISceneManager* smgr) : Pieza(), Reglas::Tablero(){

    this->mesh=smgr->getMesh("Texturas/Antorcha2.3ds");
    //le damos el tamaño vertical
    this->datos.reserve(this->size_y);
    this->datos.resize(this->size_y);

    //ahora el tamaño horizontal
    for(std::size_t i = 0; i < this->datos.size(); i++){
        this->datos.at(i).reserve(this->size_x);
        this->datos.at(i).resize(this->size_x);
    }
    this->DibujaTodo(smgr);

}

void Grafico::Tablero::DibujaTodo(scene::ISceneManager* smgr){
    //dibujamos el tablero
      this->dibuja(smgr);
      //dibujamos las celdas
        //ahora le damos su posición a cada celda...
    for(std::size_t i = 0; i < this->datos.size(); i++){
        for(std::size_t j = 0; j < this->datos.at(i).size(); j++){
            this->datos.at(i).at(j)=new Celda(smgr);
            this->datos.at(i).at(j)->setCeldaR( &this->getCelda(i,j));
           this->datos.at(i).at(j)->colocar();
        }
    }

}
Grafico::Tablero::Tablero(const Tablero& orig): Pieza(orig), Reglas::Tablero(orig) {

}


Grafico::Tablero::~Tablero() {
}

