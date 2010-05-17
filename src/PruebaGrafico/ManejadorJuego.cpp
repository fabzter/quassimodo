

#include "ManejadorJuego.hpp"



ManejadorJuego::ManejadorJuego(scene::ISceneManager* smgr,gui::IGUIEnvironment* env) {
    this->smgr=smgr;
    this->env=env;
    this->skin=new Grafico::Skin(smgr);
    this->Agentes.resize(2);
    this->clearAgentes();
    this->hayagente=false;
     this->partida=new Partida(this->smgr,this->skin);
    this->menu=new Menu(this->smgr,this->env,this->partida->t,this->skin);
    this->aniend=new AnimacionEnd(this->partida,this->smgr);


}

ManejadorJuego::ManejadorJuego(const ManejadorJuego& orig) {
}

ManejadorJuego::~ManejadorJuego() {
}
void ManejadorJuego::setMenu(){
    delete(this->partida);
    delete(this->menu);
    this->smgr->clear();
    this->partida=new Partida(this->smgr,this->skin);
     this->menu=new Menu(this->smgr,this->env,this->partida->t,this->skin);
}

void ManejadorJuego::setPartida(){
    if(this->hayagente){
        this->menu->dropMenuP();
        this->partida->SetJugadores(this->Agentes[0],this->Agentes[1],this->smgr,this->aniend);
        this->partida->iniciarPartida();
        scene::ICameraSceneNode *cam= this->smgr->addCameraSceneNodeMaya(0,200.f,200.f,200.0f);
        cam->setTarget(core::vector3df(18.993,22.82,100.268));
        cam->setPosition(core::vector3df(49.33476,110.99,99.7382));
        //cam->bindTargetAndRotation(true);
        std::cout<<cam->getTargetAndRotationBinding()<<std::endl;
    }
    else{
         this->menu->MsgBox("No ha seleccionado agentes");
    }
    

}
bool ManejadorJuego::SiguienteJugada(){
  // while(this->partida->estaEnCurso()){
    bool curso=false;
       try{
        curso=this->partida->siguienteJugada(this->smgr);
        }
         catch(Scripting::ScriptMalo &e)
         {
            this->menu->MsgBox(e.what());
         }
         catch(Reglas::ReglasRotas &e)
         {
             this->menu->MsgBox(e.what());
         }

//}
     if(this->hayGanador())
      {
        this->menu->MsgBox("Hay un ganador!" );
      }
    return curso;

}
bool ManejadorJuego::hayGanador(){
    return this->partida->hayGanador();
}
Menu* ManejadorJuego::getMenu(){
    return this->menu;
}
void ManejadorJuego::setAgente(std::string Agente,int noAgente){

    std::cout<<"en setAgente "<<Agente<<std::endl;
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

