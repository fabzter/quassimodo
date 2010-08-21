
#include <irrlicht/ISceneManager.h>


#include <irrlicht/ILightSceneNode.h>
#include <irrlicht/IParticleSystemSceneNode.h>

#include "Antorcha.hpp"

using namespace irr;

Grafico::Antorcha::Antorcha(scene::ISceneManager* smgr,scene::ISceneNode* parent): Pieza(parent){

        this->radioLuz=190.0f;
        this->setVectPosicion(0,0,0);
        this->dibujaAntorcha(smgr,parent);
        

    }

Grafico::Antorcha::Antorcha(const Antorcha& orig) : Pieza(orig){
    this->nfuego=orig.nfuego;
    this->radioLuz=orig.radioLuz;
    this->posicionF=orig.posicionF;
}

Grafico::Antorcha::~Antorcha() {
    this->dropAntorcha();
}

void Grafico::Antorcha::setVectPosicion(int x, int y, int z){
       core::vector3df v;
           v.X=1,v.Y=1,v.Z=1;
        this->posicionF.X=x;// this->posiciong.X;
        this->posicionF.Y=this->posiciong.Y+(105*v.Y);
        this->posicionF.Z=z;//this->posiciong.Z;
        this->posiciong=this->posicionF;
}

void Grafico::Antorcha::dibujaAntorcha(scene::ISceneManager* smgr,scene::ISceneNode* parent)
   {
                 this->nfuego = smgr->addLightSceneNode(parent,this->posicionF,video::SColorf(1.0f, 1.0f, 1.0f,1.0f),this->radioLuz);
                
                 this->nfuego->setLightType(video::ELT_POINT);
                 //this->nfuego->enableCastShadow(true);
      
                this->setPosicionAntorcha(this->posiciong);


   }

void Grafico::Antorcha::setPosicionAntorcha(core::vector3df posicion){
      this->setPosicionAntorcha(posicion.X, posicion.Y ,posicion.Z);

   }

void Grafico::Antorcha::setPosicionAntorcha(int x, int y ,int z){
            this->setVectPosicion(x,y,z);
            this->nfuego->setPosition( this->posicionF );
         
   }

void Grafico::Antorcha::setEscalaAntorcha(int x,int y, int z){
    //this->nfuego->setScale(core::vector3df(x,y,z));
    this->setEscala(x,y,z);
    this->nfuego->setRadius(35*x);
    this->setPosicionAntorcha(this->posiciong);


}
void Grafico::Antorcha::dropAntorcha(){

    this->nfuego->removeAnimators();
    this->nfuego->removeAll();
    this->nfuego->remove();
    
}
