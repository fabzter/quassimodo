
#include "Aplicacion.hpp"


Aplicacion::Aplicacion(scene::ISceneManager* smgr,gui::IGUIEnvironment* env) {
    this->smgr=smgr;
    this->env=env;
     this->partida=new Partida(this->smgr);
    this->menu=new Menu(this->smgr,this->env,this->partida->t);

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
   this->partida->SetJugadores("../bin/agenteBarreras2.py","../bin/agenteBarreras2.py",this->smgr);
   this->partida->iniciarPartida();

}
void Aplicacion::SiguienteJugada(){
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
     if(this->hayGanador())
      {
        this->menu->MsgBox(env,"Hay un ganador!" );
      }

}
bool Aplicacion::hayGanador(){
    return this->partida->hayGanador();
}
