#include <ISceneManager.h>

#include "Antorcha.hpp"

using namespace irr;

Grafico::Antorcha::Antorcha(scene::ISceneManager* smgr,Skin *skin,scene::ISceneNode* parent): Pieza(parent){
        //this->nodoA=NULL;
        this->mesh =skin->getAntorcha();
        this->fuego=skin->getTAntorcha();
        this->radioLuz=175.0f;
        this->sombra=sombra;
        this->dibuja(smgr);
        this->setVectPosicion(0,0,0);

        this->dibujaAntorcha(smgr);
        

    }

Grafico::Antorcha::Antorcha(const Antorcha& orig) : Pieza(orig){

 /*   this->mesh->drop();
    this->fuego->drop();
    this->nfuego->drop();
    this->nodoA->drop();
    this->mesh=orig.mesh;
    this->nfuego=orig.nfuego;
    this->fuego=orig.fuego;
    this->nodoA=orig.nodoA;
    this->setVectPosicion(orig.posicion.X,orig.posicion.Y,orig.posicion.Z);*/
}

Grafico::Antorcha::~Antorcha() {
    this->dropAntorcha();
}

void Grafico::Antorcha::setVectPosicion(int x, int y, int z){
        this->posiciong.X=x;
        this->posiciong.Y=y;
        this->posiciong.Z=z;
       core::vector3df v;
       if(this->nodoA!=NULL)
           v= this->nodoA->getScale();
       else
           v.X=1,v.Y=1,v.Z=1;
        this->posicionF.X=0;// this->posiciong.X;
        this->posicionF.Y=this->posiciong.Y+(this->size.Y*v.Y);
        std::cout<<this->size.Y<<std::endl;
        std::cout<<"pos F "<<this->posicionF.Y<<std::endl;

        this->posicionF.Z=0;//this->posiciong.Z;
}

void Grafico::Antorcha::dibujaAntorcha(scene::ISceneManager* smgr)
   {
                this->nfuego = smgr->addEmptySceneNode(this->nodoA);
                
   }

void Grafico::Antorcha::setPosicionAntorcha(core::vector3df posicion){
      this->setPosicionAntorcha(posicion.X, posicion.Y ,posicion.Z);

   }

void Grafico::Antorcha::setPosicionAntorcha(int x, int y ,int z){
            this->setVectPosicion(x,y,z);
            this->setPosicion(x,y,z);
           // nfuego->setPosition( this->posicionF);
         
   }
void Grafico::Antorcha::setPosicionFuego(int x, int y ,int z){
            this->posicionF.X= x;
            this->posicionF.Y=y;
            this->posicionF.Z=z;
            if (nfuego) nfuego->setPosition( this->posicionF);
   }
core::vector3df Grafico::Antorcha::getPosicionFuego(){
          return this->posicionF;
      }
void Grafico::Antorcha::setEscalaAntorcha(int x,int y, int z){
    //this->nfuego->setScale(core::vector3df(x,y,z));
    this->setEscala(x,y,z);
    if (nfuego) nfuego->setScale(core::vector3df(35*x,35*y,35*z));
    this->setPosicionAntorcha(this->posiciong);

}
void Grafico::Antorcha::dropAntorcha(){
    if (nfuego) {
        nfuego->removeAll();
        nfuego->remove();
        nfuego = nullptr;
    }
    
}
