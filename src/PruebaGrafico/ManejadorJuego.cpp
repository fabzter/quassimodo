
#include "ManejadorJuego.hpp"


ManejadorJuego::ManejadorJuego(scene::ISceneManager* smgr,gui::IGUIEnvironment* env) {
    this->smgr=smgr;
    this->env=env;
    this->skin=new Grafico::Skin(smgr,env);
    this->Agentes.resize(2);
    this->clearAgentes();
     this->partida=new Partida(this->smgr,this->skin);
    this->mgui=new ManejadorGUI(this->smgr,this->env,this->partida->t,this->skin);
    this->aniend=new AnimacionEnd(this->partida,this->smgr);
     this->setSkinAmbiente();
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
        //cam->setPosition(core::vector3df(49.33476,110.99,99.7382));
        //cam->bindTargetAndRotation(true);
       // std::cout<<cam->getTargetAndRotationBinding()<<std::endl;
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
      scene::ICameraSceneNode *cam=this->smgr->getActiveCamera();
      core::vector3df v;
      v=cam->getTarget();
      std::cout<<"target "<<v.X<<","<<v.Y<<","<<v.Z<<std::endl;
      v=cam->getPosition();
      std::cout<<"posicion "<<v.X<<","<<v.Y<<","<<v.Z<<std::endl;
     v=cam->getRotation();
      std::cout<<"rotation "<<v.X<<","<<v.Y<<","<<v.Z<<std::endl;
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
     scene::ICameraSceneNode *cam= this->smgr->addCameraSceneNodeMaya(0,200.f,200.f,200.0f);
     cam->setTarget(core::vector3df(242.634,42.6679,597.024));
     this->smgr->setActiveCamera(cam);
     //colocamos el tope de la camara para que nosevea debajo del piso
     scene::ITriangleSelector* selector= this->smgr->createTerrainTriangleSelector(this->terrain, 0);
	this->terrain->setTriangleSelector(selector);

	// create collision response animator and attach it to the camera
	scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
		selector,cam , core::vector3df(60,100,60),
		core::vector3df(0,0,0),
		core::vector3df(0,50,0));
	selector->drop();
	cam->addAnimator(anim);
	anim->drop();

 }
 void ManejadorJuego::setCamMenu(){
     scene::ICameraSceneNode *cam= this->smgr->addCameraSceneNode();
     cam->setTarget(core::vector3df(-271.296,117.212,844.29));
     cam->setPosition(core::vector3df(-355.767,139.423,926.578));
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

 }
 void ManejadorJuego::dropSkinAmbiente(){
     this->terrain->removeAll();
     this->skydome->removeAll();
 }
