
#include "Celda.hpp"

using namespace irr;

Grafico::Celda::Celda(scene::ISceneManager* smgr,Skin* skin,scene::ISceneNode* parent):Grafico::Pieza(parent){
       
    if(skin!=NULL){
           this->mesh=skin->getCelda();
           this->celdaR=NULL;
           this->nodoA=NULL;
           this->smgr=smgr;
           this->dibuja(smgr);
           this->nodoA->setMaterialTexture(0,skin->getTCelda() );
           this->nodoA->getMaterial(0).NormalizeNormals=true;
           this->skin=skin;
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
      y-=12;//( this->size.Y*v.Y );


      this->setPosicion(x,y,z);
  }
  void Grafico::Celda::SetEscalaCelda(int x, int y, int z){
      this->setEscala(x,y,z);
      std::cout<<"Escala celda "<<this->getEscala().X<<" , "<<this->getEscala().Y<<" , "<<this->getEscala().Z<<std::endl;

  }
 void Grafico::Celda::ResaltarCelda(){
     scene::IParticleSystemSceneNode *ps=smgr->addParticleSystemSceneNode(false,this->nodoA,0,core::vector3df(this->size.X/2,this->size.Y,this->size.Z/2) );
     ps->setMaterialTexture(0,skin->getTAntorcha());


     scene::IParticleEmitter* em = ps->createBoxEmitter(
		core::aabbox3d<f32>(-7,0,-7,7,1,7), // tamaño del emisor
		core::vector3df(0.0f,0.035f,0.00f),   // direccion inicial
		5,100,                             // emit rate
		video::SColor(0,0,0,0),       // color obscuro
		video::SColor(0,255,255,255),       // color brillo
		800,2000,0,                         // min and max age, angle
		core::dimension2df(0.1f,0.1f),         // tamaño minimo
		core::dimension2df(this->size.X,this->size.Z) );        // taaño maximo*/
     /* em->setMinStartSize(core::dimension2df(this->size.X,this->size.Z));
     //em->setMaxStartSize(core::dimension2df(this->size.X,this->size.Z));
     em->setDirection(core::vector3df(0.0f,0.23f,0.00f));
     em->setMinStartColor(video::SColor(0,0,0,0));
     em->setMaxStartColor(video::SColor(0,255,255,255));*/
     ps->setEmitter(em);
     em->drop();
     ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
     ps->setMaterialFlag(video::EMF_LIGHTING,false);
     scene::IParticleAffector* paf =ps->createFadeOutParticleAffector(video::SColor(0,0,0,0),3000);//ps->createGravityAffector(core::vector3df(0.0f,-0.03f,0.0f),500);//ps->createScaleParticleAffector(core::dimension2df(0.5f, 0.5f));//

     ps->addAffector(paf); // same goes for the affector
     paf->drop();
 }
 void Grafico::Celda::dropResaltado(){
     this->nodoA->removeAll();
 }