
#include <irrlicht/irrList.h>

#include "ManejadorJuego.hpp"


ManejadorJuego::ManejadorJuego(scene::ISceneManager* smgr,gui::IGUIEnvironment* env) {
    this->smgr=smgr;
    this->env=env;
    this->skin=new Grafico::Skin(smgr,env);
    this->terrain==NULL;
    this->Agentes.resize(2);
    this->clearAgentes();
     this->partida=new Partida(this->smgr,this->skin);
    this->mgui=new ManejadorGUI(this->smgr,this->env,this->partida->t,this->skin);
    this->aniend=new AnimacionEnd(this->partida,this->smgr);
    this->setSkinAmbiente();
     this->cam=NULL;
    this->setCamMenu();

}

ManejadorJuego::ManejadorJuego(const ManejadorJuego& orig) {
}

ManejadorJuego::~ManejadorJuego() {
}
void ManejadorJuego::setMenu(){
    delete(this->partida);
    delete(this->mgui);
    this->smgr->clear();
    this->partida=new Partida(this->smgr,this->skin);
     this->mgui=new ManejadorGUI(this->smgr,this->env,this->partida->t,this->skin);
    
     this->setCamMenu();
}

void ManejadorJuego::setPartida(){
    if(this->hayagente){
        this->mgui->dropMenu();
        this->partida->SetJugadores(this->Agentes[0],this->Agentes[1],this->smgr,this->aniend);
        this->partida->iniciarPartida();
         this->setCamJuego();
         this->mgui->setMenuPartida();
     
    }
    else{
         this->mgui->MsgBox("No ha seleccionado agentes");
    }
    

}
 void ManejadorJuego::setEscala(int x,int y,int z){
     this->partida->SetEscala(x,y,z);
     this->mgui->setEscala(x,y,z);
 }
bool ManejadorJuego::SiguienteJugada(){
  // while(this->partida->estaEnCurso()){
    bool curso=false;
       try{
        curso=this->partida->siguienteJugada(this->smgr);
        }
         catch(Scripting::ScriptMalo &e)
         {
            this->mgui->MsgBox(e.what());
         }
         catch(Reglas::ReglasRotas &e)
         {
             this->mgui->MsgBox(e.what());
         }

//}
     if(this->hayGanador())
      {
        this->mgui->MsgBox("Hay un ganador!" );
      }
    return curso;

}
bool ManejadorJuego::hayGanador(){
    return this->partida->hayGanador();
}
ManejadorGUI* ManejadorJuego::getManejadorGUI(){
    return this->mgui;
}
void ManejadorJuego::setAgente(std::string Agente,int noAgente){

    this->Agentes[noAgente]=Agente;
    if(this->Agentes[0]!="" && this->Agentes[1]!="")
        this->hayagente=true;
}
void ManejadorJuego::clearAgentes(){
    this->Agentes[0]="";
    this->Agentes[1]="";
    this->hayagente=false;
}
  void  ManejadorJuego::quick(){
       this->Agentes[0]="../../bin/agenteBarreras2.py";
    this->Agentes[1]="../../bin/agenteBarreras2.py";
    this->hayagente=true;
    this->setPartida();
  }
  void ManejadorJuego::printCam(){
      //scene::ICameraSceneNode *cam=this->smgr->getActiveCamera();
      core::vector3df v;
      v=cam->getTarget();
      std::cout<<"target "<<v.X<<","<<v.Y<<","<<v.Z<<std::endl;
      v=cam->getPosition();
      std::cout<<"posicion "<<v.X<<","<<v.Y<<","<<v.Z<<std::endl;
     v=cam->getRotation();
      std::cout<<"rotation "<<v.X<<","<<v.Y<<","<<v.Z<<std::endl;
      std::cout<<"lejania "<<cam->getFarValue()<<std::endl;
      std::cout<<"cercania "<<cam->getNearValue()<<std::endl;
       std::cout<<"inner "<<luna->getLightData().InnerCone<<std::endl;
        std::cout<<"outer "<<luna->getLightData().OuterCone<<std::endl;
       //std::cout<<"matriz "<<cam->getAbsoluteTransformation()<<std::endl;

  }
 void ManejadorJuego::CambiaTextoAgnt(int bAgente){

     this->mgui->SetTextBtnAngt(bAgente , this->SplitNombre( this->Agentes.at(bAgente) ) );

 }
std::string ManejadorJuego::SplitNombre (std::string str)
{
  size_t found;
  found=str.find_last_of("/\\");
  return str.substr(found+1) ;

}
 void ManejadorJuego::setCamJuego(){
     if(cam!=NULL)
         cam->remove();
     cam = smgr->addCameraSceneNode();
	IAnimatorCameraTokayo* anm = new TokayoCamera(2,2,2);
        anm->setRotationNumbers(270,57) ;
        anm->setZoom(602);
	cam->addAnimator(anm);
	anm->drop();
     
     core::vector3df v=this->partida->getCentro();
     std::cout<<"centro "<<v.X<<","<<v.Y<<","<<v.Z<<std::endl;
     cam->setTarget(core::vector3df(-47.5287,32.6925,-63.6243));
     this->smgr->setActiveCamera(cam);
   
     //colocamos el tope de la camara para que nosevea debajo del piso
     if(this->terrain!=NULL){
         scene::ITriangleSelector* selector= this->smgr->createTerrainTriangleSelector(this->terrain, 0);
            this->terrain->setTriangleSelector(selector);

            // create collision response animator and attach it to the camera
            scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
                    selector,cam , core::vector3df(10,50,10),
                    core::vector3df(0,0,0),
                    core::vector3df(0,50,0));
            selector->drop();
            cam->addAnimator(anim);
            anim->drop();
     }

 }
 void ManejadorJuego::setCamMenu(){
     if(cam!=NULL)
         cam->remove();
     cam= this->smgr->addCameraSceneNode();
     cam->setTarget(core::vector3df(-344.395,170.816,333.796));
     cam->setPosition(core::vector3df(-357.9,173,352.904));
    this->smgr->setActiveCamera(cam);
 }
 void ManejadorJuego::setSkinAmbiente(){

         this->skydome=this->smgr->addSkyDomeSceneNode( this->skin->getTSkydome() );
         this->terrain =  this->smgr->addTerrainSceneNode(this->skin->getHTerrain(),
		0,-1,core::vector3df(-3000.f, -80.f, -3000.f),		// position
		core::vector3df(0.f, 0.f, 0.f),		// rotation
		core::vector3df(12.0f, 0.5f, 12.0f),	// scale
		video::SColor ( 255, 255, 255, 255 ),	// vertexColor
		5,					// maxLOD
		scene::ETPS_17,				// patchSize
		4					// smoothFactor
		);
	this->terrain->setMaterialFlag(video::EMF_LIGHTING, true);
	this->terrain->setMaterialTexture( 0,this->skin->getTTerrain() );
        this->terrain->scaleTexture(1.0f, 20.0f);
        //this->smgr->setAmbientLight(video::SColor(10,10,10,10));
       
        /* luna= smgr-> addLightSceneNode(0,core::vector3df(200,200,100),video::SColorf(1.0f, 1.0f, 1.0f,1.0f),600);
       luna->setLightType(video::ELT_SPOT);
       //luna->getLightData().InnerCone=-30;
        //luna->getLightData().OuterCone=120;
        scene::ISceneNode *l=this->smgr->addSphereSceneNode(20,16,luna,-1,core::vector3df(0,0,0));
        l->setMaterialFlag(video::EMF_LIGHTING, false);*/

 }
 void ManejadorJuego::aumentaAngulo(bool inner){
     if(inner)
           luna->getLightData().InnerCone+=1;
     else
          luna->getLightData().OuterCone+=1;
 }
 void ManejadorJuego::disminuyeAngulo(bool inner){
      if(inner)
           luna->getLightData().InnerCone-=1;
     else
          luna->getLightData().OuterCone-=1;
 }
 void ManejadorJuego::dropSkinAmbiente(){
     this->terrain->removeAll();
     this->skydome->removeAll();
 }
 void ManejadorJuego::cambiaVistaJuego(int vista){

   core::list<scene::ISceneNodeAnimator*  >::ConstIterator a=cam->getAnimators().begin() ;

  IAnimatorCameraTokayo* anm = (IAnimatorCameraTokayo*) *a;
    anm->setZoom(602);
     switch(vista){
         case 1:
             anm->setRotationNumbers(1260,45);
             break;
         case 2:
            anm->setRotationNumbers(1078,45);
             break;
        case 3:
             anm->setRotationNumbers(1170,45);
             break;
         case 4:
             anm->setRotationNumbers(1350,45);
             break;

     }

 }
