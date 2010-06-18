
#include <irrlicht/irrList.h>

#include "ManejadorJuego.hpp"


ManejadorJuego::ManejadorJuego(scene::ISceneManager* smgr,gui::IGUIEnvironment* env,bool grafico) {
    this->grafico=grafico;
    this->smgr=smgr;
    this->env=env;
     this->skin=new Grafico::Skin(this->smgr,this->env);
    this->terrain==NULL;
    this->Agentes.resize(2);
    this->init();
    this->setMenu();

    
}

ManejadorJuego::ManejadorJuego(const ManejadorJuego& orig) {
}

ManejadorJuego::~ManejadorJuego() {
}
void ManejadorJuego::init(){

    this->clearAgentes();
   
    this->partida=new Partida(this->smgr,this->skin);
    this->mgui=new ManejadorGUI(this->smgr,this->env,this->partida->t,this->skin,this->grafico);
    this->partidainiciada=false;
    if(this->grafico){
        this->aniend=new AnimacionEnd(this->partida,this->smgr);
        this->setSkinAmbiente();
        this->cam=NULL;
        this->setEscala(5,5,5);
    }

}
char ManejadorJuego::setMenu(){
    
    if(this->partidainiciada)
    {
        delete(this->partida);   
        delete(this->mgui);
        delete(this->aniend);
        //delete(this->skin);
        this->smgr->clear();
        this->env->clear();
        this->init();
    }
    if(this->grafico){
        this->setCamMenu();

    }
    return this->mgui->setMenu(this->grafico);

}

void ManejadorJuego::setPartida(){
    if(this->hayagente){

        if(this->grafico){
            this->mgui->dropMenu();
            this->setCamJuego();
            this->mgui->setMenuPartida();
        }

        try{
            this->partida->SetJugadores(this->Agentes[0],this->Agentes[1],this->smgr,this->aniend);
            this->partida->iniciarPartida();
            this->partidainiciada=true;
        }

      catch (Scripting::ScriptMalo &e)
      {
          this->mgui->MsgBox(e.what(),this->grafico,BOK_ERROR);
      }
    }
    else{
         this->mgui->MsgBox("No ha seleccionado agentes",true,BOK_ADVERTENCIA);
    }

}

 void ManejadorJuego::setEscala(int x,int y,int z){
     this->partida->SetEscala(x,y,z);
     this->mgui->setEscala(x,y,z);
 }

bool ManejadorJuego::SiguienteJugada(){
    bool curso=false;
       try{
        curso=this->partida->siguienteJugada(this->smgr);
        }
         catch(Scripting::ScriptMalo &e)
         {
            this->mgui->MsgBox(e.what(),this->grafico,BOK_ERROR);
            
         }
         catch(Reglas::ReglasRotas &e)
         {
             this->mgui->MsgBox(e.what(),this->grafico,BOK_ERROR);
            
         }

     if(this->hayGanador())
      {
        this->mgui->MsgBox("Hay un ganador!",this->grafico );
      }
    return curso;

}

bool ManejadorJuego::hayGanador(){
    return this->partida->hayGanador();
}

bool ManejadorJuego::enCurso(){
    return this->partida->estaEnCurso();
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
    /* if(this->cam!=NULL)
         this->cam->remove();*/
    this->cam= this->smgr->addCameraSceneNode();
     this->cam->setTarget(core::vector3df(-344.395,170.816,333.796));
     this->cam->setPosition(core::vector3df(-357.9,173,352.904));
    this->smgr->setActiveCamera(this->cam);
 }
 void ManejadorJuego::setSkinAmbiente(){

         this->skydome=this->smgr->addSkyDomeSceneNode( this->skin->getTSkydome() );
         this->terrain =  this->skin->getterrain();
	
	this->terrain->setMaterialTexture( 0,this->skin->getTTerrain() );
        this->terrain->setMaterialFlag(video::EMF_LIGHTING, true);
        this->terrain->scaleTexture(1.0f, 20.0f);


 }
 void ManejadorJuego::dropSkinAmbiente(){
     this->terrain->removeAll();
     this->skydome->removeAll();
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
             anm->setRotationNumbers(1170,45);
             break;
         case 4:
             anm->setRotationNumbers(1350,45);
             break;
    

     }
  
 }
 bool ManejadorJuego::getSalir(){
    return this->salir;
}
void ManejadorJuego::setSalir(bool valor){
    this->salir=valor;
}
void ManejadorJuego::SetAgentesConsola(bool ambos){
    this->setAgente(this->getManejadorGUI()->getPath(false,1), 0);
    if(ambos)
        this->setAgente(this->getManejadorGUI()->getPath(false,2), 1);
    else
        this->setAgente("../../bin/agenteBarreras2.py", 1);
    this->setPartida();

}
void ManejadorJuego::imprimeTableroConsola(){
    this->partida->impimeTablero();
    std::cin.get();
}

void ManejadorJuego::setObjetivoCam(){
    //-47.5287,32.6925,-63.6243
      if(this->cam!=NULL)
         this->cam->remove();
     this->cam = smgr->addCameraSceneNode();
	IAnimatorCameraTokayo* anm = new TokayoCamera(2,2,2);
        anm->setRotationNumbers(270,57) ;
        anm->setZoom(602);
	this->cam->addAnimator(anm);
	anm->drop();

     core::vector3df v=this->partida->getCentro();
    core::vector3df t=core::vector3df(-48.275,57.6925,-63.6251);
     cam->setTarget(t+v);
     
     this->smgr->setActiveCamera(cam);

    
}