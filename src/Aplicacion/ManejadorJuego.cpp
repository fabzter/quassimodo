#include "ManejadorJuego.hpp"

ManejadorJuego::ManejadorJuego(scene::ISceneManager* smgr,
        gui::IGUIEnvironment* env, Grafico::Skin* skin,int VelAnim)
{
    this->smgr=smgr;
    this->env=env;
    this->skin=skin;
    this->terrain==NULL; 
    this->skydome=NULL;
    this->Agentes.resize(2);
    this->velAnim=VelAnim;
    this->init();
    if(smgr!=NULL)
        this->setMenu();
}

ManejadorJuego::ManejadorJuego(const ManejadorJuego& orig) {
}

ManejadorJuego::~ManejadorJuego() {
    if(this->smgr!=NULL){
        delete(this->partida);
        this->dropSkinAmbiente();}
    delete(this->mgui);     
    
    
}
void ManejadorJuego::init(){

    this->clearAgentes();
    this->pausa=false;
    botonesJugador=this->partidainiciada=false;
    this->haciendoJugada=true;
    if(this->smgr!=NULL){
        this->partida=new PartidaGrafica(this->smgr,this->skin,this->env,this->velAnim);
        this->mgui=new Grafico::ManejadorGUI(this->smgr,this->env,this->partida->t,this->skin);
        this->setSkinAmbiente();
        this->cam=0;
    }
    else{
        this->mgui=new Grafico::ManejadorGUI(this->smgr,this->env,NULL,this->skin);
    }
        


}
void ManejadorJuego::setMenu(){
    this->setCamMenu();
    this->mgui->setMenu();

}

bool ManejadorJuego::setPartida(bool Humanos){
    if(this->hayagente){

        this->partida->SetJugadores(this->Agentes[0],this->Agentes[1],Humanos);

        this->partida->iniciarPartida();
        this->partidainiciada=true;
        this->mgui->dropMenu();
        this->setCamJuego();
        this->mgui->setMenuPartida();
        
        return this->partidainiciada;
    }
    else{
         this->mgui->MsgBox("No ha seleccionado agentes",true, BOK_ERROR);
         return this->partidainiciada;
    }

}

 void ManejadorJuego::setEscala(int x,int y,int z){
     this->partida->SetEscala(x,y,z);
     this->mgui->setEscala(x,y,z);
 }

bool ManejadorJuego::SiguienteJugada(){
    bool curso=false;
       try{
        curso=this->partida->siguienteJugada();
        }
         catch(std::exception &e)
         {
            this->mgui->MsgBox(e.what(),true,BOK_ERROR);
         }

     if(this->hayGanador() )
      {
        this->mgui->MsgBox(this->getMsjGanador(),true );
      }
    return curso;
}

bool ManejadorJuego::hayGanador(){
    return this->partida->hayGanador();
}

bool ManejadorJuego::enCurso(){
    return this->partida->estaEnCurso();
}

Grafico::ManejadorGUI* ManejadorJuego::getManejadorGUI(){
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

  bool  ManejadorJuego::quick(){
    this->Agentes[0]="./bin/agente_astar.py";
    this->Agentes[1]="./bin/agente_astar.py";
    this->hayagente=true;
    return this->setPartida(false);
  }
  void ManejadorJuego::printCam(){

      core::vector3df v;
      v=cam->getTarget();
      std::cout<<"target "<<v.X<<","<<v.Y<<","<<v.Z<<std::endl;
      v=cam->getPosition();
      std::cout<<"posicion "<<v.X<<","<<v.Y<<","<<v.Z<<std::endl;
     v=cam->getRotation();
      std::cout<<"rotation "<<v.X<<","<<v.Y<<","<<v.Z<<std::endl;
     

  }


 void ManejadorJuego::setCamJuego(){
   
     this->setObjetivoCam();
   
     //colocamos el tope de la camara para que nosevea debajo del piso
     if(this->terrain!=NULL){
         scene::ITriangleSelector* selector= this->smgr->createTriangleSelectorFromBoundingBox(this->terrain);
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
     this->dropCamera();
    this->cam= this->smgr->addCameraSceneNode();
     this->cam->setTarget(core::vector3df(282.469,116.59,275.255));
     this->cam->setPosition(core::vector3df(-43.4172,258.423,692.37));
    this->smgr->setActiveCamera(this->cam);
    this->cam->setAutomaticCulling(scene::EAC_FRUSTUM_BOX);
 }

 void ManejadorJuego::setSkinAmbiente(){

        this->skydome=this->smgr->addSkyDomeSceneNode( this->skin->getTSkydome(),16,8,1,1.3,400.f );
        this->skydome->setName("CIELO");
       /* this->terrain =   this->smgr->addTerrainSceneNode(this->skin->getheightMapFile(),
		0,-1,core::vector3df(-4200.f, -80.f, -3000.f),		// position
		core::vector3df(0.f, 0.f, 0.f),		// rotation
		core::vector3df(12.0f, 0.5f, 12.0f),	// scale
		video::SColor ( 255, 255, 255, 255 ),	// vertexColor
		5,					// maxLOD
		scene::ETPS_33,				// patchSize
		2					// smoothFactor
		);
	
        this->terrain->setMaterialTexture( 0,this->skin->getTTerrain() );
       this->terrain->setMaterialFlag(video::EMF_LIGHTING, true);
       this->terrain->scaleTexture(1.0f, 1.0f);*/

       scene::IAnimatedMesh* mesh = smgr->addHillPlaneMesh( "myHill",
		core::dimension2d<f32>(60,60),
		core::dimension2d<u32>(90,90), 0, 0,
		core::dimension2d<f32>(0,0),
		core::dimension2d<f32>(40,40));

	this->terrain = smgr->addWaterSurfaceSceneNode(mesh->getMesh(0), 5.0f, 300.0f, 100.0f);
	this->terrain->setPosition(core::vector3df(0,30,0));
	this->terrain->setMaterialTexture(0,skin-> getTTerrain());

    this->terrain->setMaterialType(video::EMT_REFLECTION_2_LAYER);
       this->terrain->setName("TERRENO");
       this->smgr->setAmbientLight(video::SColorf(1.0,1.0,1.0,1.0));

 }

 void ManejadorJuego::dropSkinAmbiente(){
     this->terrain->removeAll();
     this->terrain->remove();
     this->skydome->removeAll();
     this->skydome->remove();
     this->dropCamera();
     this->terrain=NULL;
     this->skydome=NULL;
 }

 void ManejadorJuego::cambiaVistaJuego(int vista){
     this->setCamJuego();
   core::list<scene::ISceneNodeAnimator*  >::ConstIterator a=cam->getAnimators().begin() ;
  IAnimatorCameraTokayo* anm = (IAnimatorCameraTokayo*) *a;
     
     switch(vista){
         case 1:
             anm->setRotationNumbers(1260,45);
             break;
         case 2:
            anm->setRotationNumbers(1078,45);
             break;
        case 3:
             anm->setRotationNumbers(1350,45);
             break;
         case 4:
             anm->setRotationNumbers(1170,45);
             break;   

     }
  
 }

 bool ManejadorJuego::getSalir(){
    return this->salir;
}

void ManejadorJuego::setSalir(bool valor){
    this->salir=valor;
}

void ManejadorJuego::dropCamera(){
    if(this->cam!=0){
        this->cam->removeAnimators();
        this->cam->removeAll();
        this->cam->remove();
        this->cam=0;
    }
}
const char* ManejadorJuego::getMsjGanador(){
    std::stringstream msj;
    msj<<"Gana el Jugador "<<this->partida->getJugadorGanador();
    return msj.str().c_str();
}

void ManejadorJuego::despachaJugada(){

    if(this->partida->estaEnCurso()&& !this->pausa){      
        if( this->partida->animacionesEnd() && !this->JugadorPreparaJugada() ){
            this->SiguienteJugada();
        }
    }
}

void ManejadorJuego::Pausar(){

    if( this->partida->estaEnCurso() ){
        this->pausa=!this->pausa;
        this->mgui->Pausar(this->pausa);
    }
}
void ManejadorJuego::AgntVsMkn(){

    this->mgui->AgntVSAgnt( false, this->partida->getNombresAgentes() );
}
bool ManejadorJuego::JugadorPreparaJugada(){
    
    bool isHumano=this->partida->JugadorIsHumano( this->partida->getJugadorEnTurno() );
    if( isHumano  ){
        if(this->partida->HaciendoJugada()){
            this->haciendoJugada=true;
            if(!this->botonesJugador){
                this->mgui->setBotonesJugador();
                this->botonesJugador=true;
            }
        }

    }
    else{
        this->haciendoJugada=false;
       // this->partida->dropOpcionesMover();
        if(this->botonesJugador){
            this->mgui->dropBotonesJugador();
            this->botonesJugador=false;
        }
    }


    return this->haciendoJugada;
    
}
bool ManejadorJuego::estaHaciendoJugada(){
    return this->haciendoJugada;
}
void ManejadorJuego::setOpcionesMover(){
    this->partida->setOpcionesMover();
}
int ManejadorJuego::ChecaJugada(core::position2d<s32>& pos,bool movimiento,int Direccion){
    return this->partida->ChecaJugada(pos,movimiento,Direccion);
}

void ManejadorJuego::setJugada(int celda,bool movimiento,int Direccion){
    this->partida->setJugada(celda,movimiento,Direccion);
    this->haciendoJugada=false;
}

void ManejadorJuego::setObjetivoCam(){
    this->dropCamera();
    this->cam = smgr->addCameraSceneNode();
    IAnimatorCameraTokayo* anm = new TokayoCamera(2,2,2);
    anm->setRotationNumbers(270,57) ;
    anm->setZoom(602);
    this->cam->addAnimator(anm);
    this->cam->setAutomaticCulling(scene::EAC_FRUSTUM_BOX);
    anm->drop();
    core::vector3df v=this->partida->getCentro();
    core::vector3df t=core::vector3df(-48.275,57.6925,-63.6251);
    cam->setTarget(t+v);
    this->cam->setAutomaticCulling(scene::EAC_FRUSTUM_BOX);
    this->smgr->setActiveCamera(cam);   
}
