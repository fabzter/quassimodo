
#include "Celda.hpp"

using namespace irr;

Grafico::Celda::Celda(scene::ISceneManager* smgr,Skin* skin):Grafico::Pieza(){
        srand ( time(NULL) );

       this->mesh=skin->getCelda();
       this->celdaR=NULL;
       this->nodoA=NULL;
      this->dibuja(smgr);

       this->nodoA->setMaterialTexture(0,skin->getTCelda() );
       this->nodoA->getMaterial(0).NormalizeNormals=true;
       //this->nodoA->addShadowVolumeSceneNode();
       /*core::matrix4 mat= this->nodoA->getMaterial(0).getTextureMatrix(0);

      // mat=mat.setTranslation(core::vector3df(rand() % 100,rand() % 100,rand() % 100));
      core::vector3df v=core::vector3df(rand() % 100,rand() % 100,rand() % 100) ;
       mat.rotateVect(v);
       //mat=mat.setTextureTranslate(rand() % 1000,rand() % 1000);
       this->nodoA->getMaterial(0).setTextureMatrix(1,mat);*/
       this->nodoA->getMaterial(0).SpecularColor.set(0,0,0,0);
      core::aabbox3d<f32> t=this->nodoA->getBoundingBox();
      this->size= t.getExtent();


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
      y+=1;//( this->size.Y*v.Y );
      this->setPosicion(x,y,z);
  }
  void Grafico::Celda::SetEscalaCelda(int x, int y, int z){

      this->setEscala(x,y,z);
        this->size=this->nodoA->getBoundingBox().getExtent();


  }