#include "ManejadorJuego.hpp"


ManejadorJuego::ManejadorJuego(scene::ISceneManager* smgr,gui::IGUIEnvironment* env, Grafico::Skin* skin,int VelAnim,bool grafico) {
    this->grafico=grafico;
    this->smgr=smgr;
    this->env=env;
    this->skin=skin;
    this->terrain==NULL;
    this->skydome=NULL;
    this->Agentes.resize(2);
    this->velAnim=VelAnim;
    this->init();
    if(grafico)
      this->setMenu();

    
}

ManejadorJuego::ManejadorJuego(const ManejadorJuego& orig) {
}

ManejadorJuego::~ManejadorJuego() {  
    delete(this->partida);
    delete(this->mgui);
    if(this->grafico){
        //delete(this->aniend);
        this->dropSkinAmbiente();
    }
}
void ManejadorJuego::init(){

    this->clearAgentes();
    this->pausa=false;
    this->partidainiciada=false;
    if(this->grafico){
        this->partida=new PartidaGrafica(this->smgr,this->skin,this->env,this->velAnim);
        PartidaGrafica *p= (PartidaGrafica*)this->partida;
        this->mgui=new Grafico::ManejadorGUI(this->smgr,this->env,p->t,this->skin,this->grafico);
        this->setSkinAmbiente();
        this->cam=0;
        this->setEscala(5,5,5);
    }
    else{
        this->partida=new PartidaConsola();
        this->mgui=new Grafico::ManejadorGUI(this->smgr,this->env,NULL,this->skin,this->grafico);
    }

}
char ManejadorJuego::setMenu(){
    

    if(this->grafico){
        this->setCamMenu();

    }
    return this->mgui->setMenu(this->grafico);

}

bool ManejadorJuego::setPartida(){
    if(this->hayagente){

        try{
            this->partida->SetJugadores(this->Agentes[0],this->Agentes[1]);
        }
        catch (Scripting::ScriptMalo &e)
        {
            std::stringstream msj;
            msj<<"Error al cargar el script del Agente: "<<this->partida->getAgenteConError()<<std::endl<<e.what();
            this->mgui->MsgBox(msj.str().c_str(),this->grafico,BOK_ERROR);
            return this->partidainiciada;
        }

        try{
            
            this->partida->iniciarPartida();
            this->partidainiciada=true;
            if(this->grafico){
                this->mgui->dropMenu();
                this->setCamJuego();
                this->mgui->setMenuPartida();
            }
            return this->partidainiciada;
        }

         catch (Scripting::ScriptMalo &e)
          {
              this->mgui->MsgBox(e.what(),this->grafico,BOK_ERROR);
              return this->partidainiciada;
          }
    }
    else{
         this->mgui->MsgBox("No ha seleccionado agentes",true, BOK_ERROR);
         return this->partidainiciada;
    }

}

 void ManejadorJuego::setEscala(int x,int y,int z){
     PartidaGrafica *p= (PartidaGrafica*)this->partida;
     p->SetEscala(x,y,z);
     this->mgui->setEscala(x,y,z);
 }

bool ManejadorJuego::SiguienteJugada(){
    bool curso=false;
       try{
        curso=this->partida->siguienteJugada();
        }
         catch(std::exception &e)
         {
            this->mgui->MsgBox(e.what(),this->grafico,BOK_ERROR);
            if(!this->grafico)
                throw;
         }

     if(this->hayGanador() && this->grafico )
      {
        this->mgui->MsgBox(this->getMsjGanador(),this->grafico );
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
    return this->setPartida();
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
 void ManejadorJuego::CambiaTextoAgnt(int bAgente){
     std::string nom= this->SplitNombre( this->Agentes[bAgente] ) ;
      std::cout<<"nom  " <<nom<<std::endl;
     this->mgui->SetTextBtnAngt(bAgente ,nom );

 }

std::string ManejadorJuego::SplitNombre (std::string str)
{
  size_t found;
   std::cout<<"en find  " <<str<<std::endl;
  found=str.find_last_of("/\\");
  std::cout<< str.substr(found+1) <<std::endl;
  return str.substr(found+1) ;

}

 void ManejadorJuego::setCamJuego(){
   
     this->setObjetivoCam();
   
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
     this->dropCamera();
    this->cam= this->smgr->addCameraSceneNode();
     this->cam->setTarget(core::vector3df(-344.395,170.816,333.796));
     this->cam->setPosition(core::vector3df(-357.9,173,352.904));
    this->smgr->setActiveCamera(this->cam);
 }

 void ManejadorJuego::setSkinAmbiente(){

        this->skydome=this->smgr->addSkyDomeSceneNode( this->skin->getTSkydome() );
        this->terrain =   this->smgr->addTerrainSceneNode(this->skin->getheightMapFile(),
		0,-1,core::vector3df(-4200.f, -80.f, -3000.f),		// position
		core::vector3df(0.f, 0.f, 0.f),		// rotation
		core::vector3df(12.0f, 0.5f, 12.0f),	// scale
		video::SColor ( 255, 255, 255, 255 ),	// vertexColor
		5,					// maxLOD
		scene::ETPS_17,				// patchSize
		4					// smoothFactor
		);
	
	this->terrain->setMaterialTexture( 0,this->skin->getTTerrain() );
        this->terrain->setMaterialFlag(video::EMF_LIGHTING, true);
       this->terrain->scaleTexture(1.0f, 20.0f);


 }

 void ManejadorJuego::dropSkinAmbiente(){
     this->terrain->removeAll();
     this->terrain->remove();
     this->skydome->removeAll();
     this->skydome->remove();
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

bool ManejadorJuego::SetAgentesConsola(bool ambos){
    this->setAgente(this->getManejadorGUI()->getPath(false,1), 0);
    if(ambos)
        this->setAgente(this->getManejadorGUI()->getPath(false,2), 1);
    else
        this->setAgente("../../bin/agenteBarreras2.py", 1);

    return this->setPartida();

}
void ManejadorJuego::imprimeTableroConsola(){
     PartidaConsola *p= (PartidaConsola*)this->partida;
     p->impimeTablero();
    //std::cout<<std::cout.tellp()<<std::endl;
    std::cin.get();
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
        PartidaGrafica *p= (PartidaGrafica*)this->partida;
        if( p->animacionesEnd() ){
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

void ManejadorJuego::setObjetivoCam(){
    this->dropCamera();
     this->cam = smgr->addCameraSceneNode();
	IAnimatorCameraTokayo* anm = new TokayoCamera(2,2,2);
        anm->setRotationNumbers(270,57) ;
        anm->setZoom(602);
	this->cam->addAnimator(anm);
	anm->drop();
    PartidaGrafica *p= (PartidaGrafica*)this->partida;
    core::vector3df v=p->getCentro();
    core::vector3df t=core::vector3df(-48.275,57.6925,-63.6251);
    cam->setTarget(t+v);
     
    this->smgr->setActiveCamera(cam);   
}