
#include <irrlicht/ISceneManager.h>


#include <irrlicht/ILightSceneNode.h>
#include <irrlicht/IParticleSystemSceneNode.h>

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
        this->posicionF.Z=0;//this->posiciong.Z;
}

void Grafico::Antorcha::dibujaAntorcha(scene::ISceneManager* smgr)
   {
                 this->nfuego = smgr->addLightSceneNode(this->nodoA,this->posicionF,video::SColorf(1.0f, 1.0f, 1.0f,1.0f),this->radioLuz*this->getEscala().X);
                
                 this->nfuego->setLightType(video::ELT_POINT);
                 //this->nfuego->enableCastShadow(true);
                 //hacemos el fuego
        	this->ps =smgr->addParticleSystemSceneNode(false,nfuego);
                scene::IParticleEmitter* em = ps->createBoxEmitter(
		core::aabbox3d<f32>(-2,0,-2,2,1,2), // tamaño del emisor
		core::vector3df(0.0f,0.075f,0.00f),   // direccion inicial
		100,100,                             // emit rate
		video::SColor(0,0,0,0),       // color obscuro
		video::SColor(0,255,255,255),       // color brillo
		800,2000,0,                         // min and max age, angle
		core::dimension2df(5.f,5.f),         // tamaño minimo
		core::dimension2df(35.f,35.f) );        // taaño maximo

                ps->setEmitter(em); // this grabs the emitter
                em->drop(); // so we can drop it here without deleting it

                scene::IParticleAffector* paf = ps->createFadeOutParticleAffector();

                this->ps->addAffector(paf); // same goes for the affector
                paf->drop();

                this->ps->setMaterialFlag(video::EMF_LIGHTING,false);
                this->ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
                this->ps->setMaterialTexture(0,this->fuego);
                this->ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
                this->ps->getMaterial(0).NormalizeNormals=true;

                this->setPosicionAntorcha(this->posiciong);


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
            nfuego->setPosition( this->posicionF);
   }
core::vector3df Grafico::Antorcha::getPosicionFuego(){
          return this->posicionF;
      }
void Grafico::Antorcha::setEscalaAntorcha(int x,int y, int z){
    //this->nfuego->setScale(core::vector3df(x,y,z));
    this->setEscala(x,y,z);
    this->nfuego->setRadius(35*x);
    this->setPosicionAntorcha(this->posiciong);
     scene::IParticleEmitter* em=this->ps->getEmitter();
     em->setMinStartSize( core::dimension2df( 1.f*x , 1.f*z ) );
     em->setMaxStartSize( core::dimension2df( 7.f*x , 7.f*z) );
     em->setDirection(core::vector3df(0.0f,0.015f*y,0.00f));

}
void Grafico::Antorcha::dropAntorcha(){
    this->nfuego->removeAnimators();
    this->nfuego->removeAll();
    this->nfuego->remove();
    
}
