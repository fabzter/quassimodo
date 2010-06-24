
#include <irrlicht/ISceneManager.h>


#include <irrlicht/ILightSceneNode.h>
#include <irrlicht/IParticleSystemSceneNode.h>

#include "Antorcha.hpp"

using namespace irr;

Grafico::Antorcha::Antorcha(scene::ISceneManager* smgr,int x, int z,Skin *skin,bool sombra): Pieza(){
        //this->nodoA=NULL;
        this->mesh =skin->getAntorcha();
        this->fuego=skin->getTAntorcha();
        this->radioLuz=35.0f;
        this->sombra=sombra;
        this->dibuja(smgr);
        this->setVectPosicion(x,0,z);

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


        this->posicionF.X= this->posiciong.X;
        this->posicionF.Y=this->posiciong.Y+(this->size.Y*v.Y);
        this->posicionF.Z=this->posiciong.Z;
}

void Grafico::Antorcha::dibujaAntorcha(scene::ISceneManager* smgr)
   {
                 this->nfuego = smgr->addLightSceneNode(0,this->posicionF,video::SColorf(1.0f, 1.0f, 1.0f,1.0f),this->radioLuz*this->getEscala().X);
                 this->nfuego->setLightType(video::ELT_POINT);
                         //hacemos el fuego
        	this->ps =	smgr->addParticleSystemSceneNode(false,nfuego);
                scene::IParticleEmitter* em = ps->createBoxEmitter(
		core::aabbox3d<f32>(-2,0,-2,2,1,2), // tamaño del emisor
		core::vector3df(0.0f,0.015f,0.00f),   // direccion inicial
		80,100,                             // emit rate
		video::SColor(0,0,0,0),       // color obscuro
		video::SColor(0,255,255,255),       // color brillo
		800,2000,0,                         // min and max age, angle
		core::dimension2df(2.f,2.f),         // tamaño minimo
		core::dimension2df(7.f,7.f) );        // taaño maximo

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
              
             
                if(this->sombra)
                {
                  // this->nodoA->addShadowVolumeSceneNode();
                  /* this->nfuego=  smgr->addVolumeLightSceneNode(0, -1,
                                    32,                              // Subdivisions on U axis
                                    32,                              // Subdivisions on V axis
                                    video::SColor(0, 255, 255, 255), // foot color
                                    video::SColor(0, 0, 0, 0));      // tail color*/
                }
                this->setPosicionAntorcha(this->posiciong);


   }

   void Grafico::Antorcha::setPosicionAntorcha(core::vector3df posicion){
      this->setPosicionAntorcha(posicion.X, posicion.Y ,posicion.Z);

   }

    void Grafico::Antorcha::setPosicionAntorcha(int x, int y ,int z){
            this->setVectPosicion(x,y,z);
            this->setPosicion(x,y,z);
            nfuego->setPosition( this->posicionF);
         
   }
      void Grafico::Antorcha::setPosicionFuego(int x, int y ,int z){
            //this->setVectPosicion(x,y,z);
          //  this->setPosicion(x,y,z);
            this->posicionF.X= x;
            this->posicionF.Y=y;
            this->posicionF.Z=z;
            nfuego->setPosition( this->posicionF);
   }
      core::vector3df Grafico::Antorcha::getPosicionFuego(){
          return this->posicionF;
      }
void Grafico::Antorcha::setEscalaAntorcha(int x,int y, int z){
    this->nfuego->setScale(core::vector3df(x,y,z));
    this->setEscala(x,y,z);
    this->nfuego->setRadius(this->radioLuz*x);
    this->setPosicionAntorcha(this->posiciong);
     scene::IParticleEmitter* em=this->ps->getEmitter();
     em->setMinStartSize( core::dimension2df( 2.f*x , 2.f*z ) );
     em->setMaxStartSize( core::dimension2df( 7.f*x , 7.f*z) );
     em->setDirection(core::vector3df(0.0f,0.015f*y,0.00f));

}
void Grafico::Antorcha::dropAntorcha(){
    this->nfuego->removeAnimators();
    this->nfuego->removeAll();
    this->nfuego->remove();
}
