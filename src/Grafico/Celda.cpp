
#include "Celda.hpp"

using namespace irr;

Grafico::Celda::Celda(scene::ISceneManager* smgr,Skin* skin,scene::ISceneNode* parent):Grafico::Pieza(parent){
       
    if(skin!=NULL){
           this->mesh=skin->getCelda();
           this->celdaR=NULL;
           this->nodoA=NULL;
           this->smgr=smgr;
           this->dibuja(smgr);
           this->nodoA->setMaterialType(video::EMT_TRANSPARENT_REFLECTION_2_LAYER);
           //this->nodoA->setMaterialTexture(0,skin->getTCelda() );
           //this->nodoA->getMaterial(0).NormalizeNormals=true;
           this->skin=skin;
    }

}

Grafico::Celda::Celda(const Celda& orig):Grafico::Pieza(orig) {

    this->mesh=orig.mesh;
    this->celdaR=orig.celdaR;
    this->smgr=orig.smgr;
    this->skin=orig.skin;
    
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
      this->nodoA->setName("CELDA");
      this->nodoA->setID((this->celdaR->getPosicion().at(0)*10)+this->celdaR->getPosicion().at(1));
  
      x+=this->celdaR->getPosicion().at(0)* ( (this->size.X+this->size.Y)*v.X ) ;
      z+=this->celdaR->getPosicion().at(1)*( (this->size.X+this->size.Y)*v.Z );
      y-=12;
      
      this->setPosicion(x,y,z);
  }
void Grafico::Celda::SetEscalaCelda(int x, int y, int z){
    this->setEscala(x,y,z);
}
 void Grafico::Celda::ResaltarCelda(){

     /*ps=smgr->addParticleSystemSceneNode(false,this->nodoA,0,core::vector3df(this->size.X/2,this->size.Y,this->size.Z/2) );
     ps->setMaterialTexture(0,skin->getTAntorcha());

     scene::IParticleEmitter* em = ps->createBoxEmitter(
		core::aabbox3d<f32>(-7,0,-7,7,1,7), // tamaño del emisor
		core::vector3df(0.0f,0.035f,0.00f),   // direccion inicial
		5,100,                             // emit rate
		video::SColor(0,255,0,0),       // color obscuro
		video::SColor(0,255,0,0),       // color brillo
		800,2000,0,                         // min and max edad, angulo
		core::dimension2df(0.1f,0.1f),         // tamaño minimo
		core::dimension2df(this->size.X,this->size.Z) );        // tamaño maximo
     ps->setEmitter(em);
     em->drop();
     ps->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
     ps->setMaterialFlag(video::EMF_LIGHTING,false);
     scene::IParticleAffector* paf =ps->createFadeOutParticleAffector(video::SColor(0,0,0,0),4000);
     ps->addAffector(paf); 
     paf->drop();*/
     this->nodoA->setMaterialType(video::EMT_SOLID);
     this->nodoA->getMaterial(0).EmissiveColor=video::SColor(255,255,0,0);
     
     //this->nodoA->setMaterialFlag(video::EMF_LIGHTING,false);
     //this->nodoA->getMaterial(0).DiffuseColor=video::SColor(0,255,0,0);

 }
 void Grafico::Celda::dropResaltado(){
     this->nodoA->removeAll();
     this->nodoA->setMaterialType(video::EMT_TRANSPARENT_REFLECTION_2_LAYER);
     this->nodoA->getMaterial(0).EmissiveColor=video::SColor(0,0,0,0);
 }
