

#include "Tablero.hpp"
#include "Celda.hpp"
using namespace irr;

Grafico::Tablero::Tablero(scene::ISceneManager* smgr) : Pieza(), Reglas::Tablero(){

    this->mesh=smgr->getMesh("Texturas/Tablero4.3ds");
    //le damos el tamaño vertical
    this->datos.reserve(this->size_y);
    this->datos.resize(this->size_y);

    //ahora el tamaño horizontal
    for(std::size_t i = 0; i < this->datos.size(); i++){
        this->datos.at(i).reserve(this->size_x);
        this->datos.at(i).resize(this->size_x);
    }
    this->DibujaTodo(smgr);

    core::vector3df v=this->getPosicionEscena();

    std::cout<<"Posicion Tablero"<<v.X<<","<<v.Y<<","<<v.Z<<std::endl;
    v= this->datos.at(0).at(0)->getPosicionEscena();
   // this->setPosicion(v.X,v.Y-this->size.Y,v.Z);
    std::cout<<"Posicion celda (0,0) "<<v.X<<","<<v.Y<<","<<v.Z<<std::endl;

    //this->size =this->nodoA->getBoundingBox().getExtent();
    std::cout<<"Tamaño tablero: "<<this->size.X<<","<<this->size.Y<<","<<this->size.Z<<std::endl;


}

void Grafico::Tablero::DibujaTodo(scene::ISceneManager* smgr){
    //dibujamos el tablero
      this->dibuja(smgr);
      this->size =this->nodoA->getBoundingBox().getExtent();
    scene::IMesh* tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(
			this->nodoA->getMesh());
     // this->nodoA->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
       this->nodoA->setMaterialTexture(0,smgr->getVideoDriver()->getTexture( "Texturas/arena1.jpg" ) );
       this->nodoA->getMaterial(0).SpecularColor.set(0,0,0,0);

		//this->nodoA->setMaterialFlag(video::EMF_FOG_ENABLE, true);
		//this->nodoA->setMaterialType(video::EMT_PARALLAX_MAP_SOLID);
		// adjust height for parallax effect
		//this->nodoA->getMaterial(0).MaterialTypeParam = 0.035f;

		// drop mesh because we created it with a create.. call.
		tangentMesh->drop();
      //dibujamos las celdas
        //ahora le damos su posición a cada celda...
    for(std::size_t i = 0; i < this->datos.size(); i++){
        for(std::size_t j = 0; j < this->datos.at(i).size(); j++){
            this->datos.at(i).at(j)=new Celda(smgr);
            this->datos.at(i).at(j)->setCeldaR( &this->getCelda(j,i));
           this->datos.at(i).at(j)->colocar(0,this->size.Y-2,0);
        }
    }
      this->setPosicion(0,0,0);

}
Grafico::Tablero::Tablero(const Tablero& orig): Pieza(orig), Reglas::Tablero(orig) {

}


Grafico::Tablero::~Tablero() {
}
void Grafico::Tablero::setPosicionTablero(int x,int y,int z){

     for(std::size_t i = 0; i < this->datos.size(); i++){
        for(std::size_t j = 0; j < this->datos.at(i).size(); j++){
           this->datos.at(i).at(j)->colocar(x,y+this->size.Y-2,z);
        }
    }
     core::vector3df v= this->datos.at(8).at(8)->getPosicionEscena();
    this->setPosicion(v.X,v.Y-this->size.Y,v.Z);
}

void Grafico::Tablero::setEscalaTablero(int x,int y,int z){
     core::vector3df v= this->datos.at(0).at(0)->getPosicionEscena();
    this->setEscala(x,y,z);

    for(std::size_t i = 0; i < this->datos.size(); i++){
        for(std::size_t j = 0; j < this->datos.at(i).size(); j++){
           this->datos.at(i).at(j)->SetEscalaCelda(x,y,z);
        }
    }

    this->setPosicionTablero(0,0,0);
}
 irr::core::vector3df Grafico::Tablero::getPosicionTablero(){
      return this->datos.at(0).at(0)->getPosicionEscena();
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
     v.X+=this->datos.at(0).at(0)->getSize().X;
     v.Z+=this->datos.at(0).at(0)->getSize().Z;
      return v;
  }

  irr::core::vector3df Grafico::Tablero::getsizeCelda(){
      return this->datos.at(0).at(0)->getSize();
  }