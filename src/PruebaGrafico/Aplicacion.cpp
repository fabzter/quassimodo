
#include "Aplicacion.hpp"
#include "AnimacionEnd.hpp"


Aplicacion::Aplicacion(scene::ISceneManager* smgr,gui::IGUIEnvironment* env) {
    this->smgr=smgr;
    this->env=env;
     this->partida=new Partida(this->smgr);
    this->menu=new Menu(this->smgr,this->env,this->partida->t);
    this->aniend=new AnimacionEnd(this->partida,this->smgr);

}

Aplicacion::Aplicacion(const Aplicacion& orig) {
}

Aplicacion::~Aplicacion() {
}
void Aplicacion::setMenu(){
    delete(this->partida);
    delete(this->menu);
    this->smgr->clear();
    this->partida=new Partida(this->smgr); 
     this->menu=new Menu(this->smgr,this->env,this->partida->t);
}

void Aplicacion::setPartida(){

    this->menu->dropMenuP();
   this->partida->SetJugadores("../../bin/agenteCamina2.py",
           "../../bin/agenteCamina2.py",this->smgr,this->aniend);
   this->partida->iniciarPartida();

}
void Aplicacion::SiguienteJugada(){
   // while(this->partida->estaEnCurso()){

        try{
        this->partida->siguienteJugada(this->smgr);
        }
         catch(Scripting::ScriptMalo &e)
         {
            this->menu->MsgBox(env,e.what());
         }
         catch(Reglas::ReglasRotas &e)
         {
             this->menu->MsgBox(env,e.what());
         }

//}
     if(this->hayGanador())
      {
        this->menu->MsgBox(env,"Hay un ganador!" );
      }

}
bool Aplicacion::hayGanador(){
    return this->partida->hayGanador();
}
