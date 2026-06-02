
#include "Celda.hpp"

using namespace irr;

Grafico::Celda::Celda(scene::ISceneManager* smgr,Skin* skin,scene::ISceneNode* parent):Grafico::Pieza(parent){
       
    if(skin!=NULL){
           this->mesh=skin->getCelda();
           this->celdaR=NULL;
           this->nodoA=NULL;
           this->dibuja(smgr);
           { auto* _tex = skin->getTCelda(); this->nodoA->forEachMaterial([_tex](video::SMaterial& m){ m.setTexture(0, _tex); }); }
           this->nodoA->getMaterial(0).NormalizeNormals=true;

           //this->nodoA->getMaterial(0).SpecularColor.set(0,0,0,0);
         //  this->setSombra();
    }

}

Grafico::Celda::Celda(const Celda& orig):Grafico::Pieza(orig) {
}


Grafico::Celda::~Celda() {

}

  void Grafico::Celda::setCeldaR (const Reglas::Celda *celdaR){
      this->celdaR=celdaR;
  }
  void Grafico::Celda::colocar(int x, int y, int z){
      core::vector3df v;
       if(this->nodoA!=NULL)
           v= this->nodoA->getScale();
       else
           v.X=1,v.Y=1,v.Z=1;
      x+=this->celdaR->getPosicion().at(0)* ( (this->size.X+this->size.Y)*v.X ) ;
      z+=this->celdaR->getPosicion().at(1)*( (this->size.X+this->size.Y)*v.Z );
      //y-=12;//( this->size.Y*v.Y );


      this->setPosicion(x,y,z);
  }
  void Grafico::Celda::SetEscalaCelda(int x, int y, int z){
      this->setEscala(x,y,z);
      std::cout<<"Escala celda "<<this->getEscala().X<<" , "<<this->getEscala().Y<<" , "<<this->getEscala().Z<<std::endl;

  }