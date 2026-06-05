
#include <vector3d.h>

#include "PartidaGrafica.hpp"
#include <Grafico/Animaciones.hpp>
using namespace irr;
using namespace Grafico;


PartidaGrafica::PartidaGrafica(scene::ISceneManager* smgr,Grafico::Skin* skin,gui::IGUIEnvironment* env,int VelAnim): Partida() {
    this->skin=skin;
    this->smgr=smgr;
    this->env=env;
    this->escala.X=1,this->escala.Y=1,this->escala.Z=1;
    this->antorchas.reserve(4);
    this->antorchas.resize(4);
    this->Barreras.reserve(20);
    this->parent=smgr->addEmptySceneNode();
    this->parent->setPosition(core::vector3df(0,0,0));
    this->t=new Tablero(smgr,this->skin,this->parent);
    this->juez = new Reglas::Juez(*t);
    this->velAnim=VelAnim;
    for(std::size_t i = 0; i < this->antorchas.size(); i++){
         this->antorchas.at(i)=new Antorcha(smgr,this->skin,this->parent);
    }
    
     core::vector3df v= this->t->getSize();
     this->ColocaAntorchas();

}

PartidaGrafica::PartidaGrafica(const PartidaGrafica& orig) {
}

PartidaGrafica::~PartidaGrafica() {

     this->dropJugadores();
     for(int i=0;i<this->antorchas.size();i++){
        delete this->antorchas.at(i);
     }
     this->dropBarreras();
     delete(this->t);
     this->parent->removeAll();
}

void PartidaGrafica::iniciarPartida()
{
    if(this->en_curso)
        throw Reglas::PartidaNoIniciada();

    for(int id = 0; id < this->t->num_jugadores; id++)
    {
        Grafico::Jugador *ju=(Grafico::Jugador*)this->jugadores.at(id);
        this->t->getJugador(id).iniciar(id);
        Reglas::Jugada j;
        j.setPosicion( this->t->getJugador(id).getPosicion() );
        this->MoverJugador(j,id);
        ju->Gira(core::vector3df(0,90-(id*180),0));
    }

    this->en_curso = true;
}

bool PartidaGrafica::siguienteJugada()
{
    return this->Siguiente(this->t);
}

void PartidaGrafica::actualizarTablero(Reglas::Jugada &j, int idJugador)
{
    if(j.getTipoDeJugada() == Reglas::MOVIMIENTO)
    {
        this->MoverJugador(j,idJugador);
    }
    else if(j.getTipoDeJugada() == Reglas::BARRERA)
    {
        this->SetBarrera(j,idJugador);
    }
}

bool PartidaGrafica::MoverJugador(Reglas::Jugada &j, int idJugador){
    Grafico::Jugador *ju=(Grafico::Jugador*)this->jugadores.at(idJugador);

    // target world pos (same mapping as before)
    core::vector3df hasta=this->t->getPosicionCelda( j.getPosicion() );
    hasta.Y+=this->t->getsizeCelda().Y*this->escala.Y;
    hasta.Z+=( ( this->t->getsizeCelda().Z*this->escala.Z ) /2);
    hasta.X+=( ( this->t->getsizeCelda().X*this->escala.X ) /2);

    if(this->en_curso){
        // current (pre-move) world pos, computed the same way from the player's current cell
        core::vector3df desde=this->t->getPosicionCelda( this->jugadores.at(idJugador)->getPosicion() );
        desde.Y+=this->t->getsizeCelda().Y*this->escala.Y;
        desde.Z+=( ( this->t->getsizeCelda().Z*this->escala.Z ) /2);
        desde.X+=( ( this->t->getsizeCelda().X*this->escala.X ) /2);

        EventoJugada ev;
        ev.tipo=EventoJugada::MOVIMIENTO; ev.jugador=idJugador;
        ev.desde=desde; ev.hasta=hasta;
        ev.numCeldas=this->t->getCeldasAMover(this->jugadores.at(idJugador)->getPosicion(), j.getPosicion());
        this->eventos.push(ev);
    } else {
        ju->setPosicion(hasta);   // initial placement (iniciarPartida): instant, no animation
    }
    this->t->moverJugador(idJugador, j.getPosicion());   // MODEL update (unchanged)
    return true;
}

 void PartidaGrafica::SetBarrera(Reglas::Jugada &j, int idJugador){
    this->Barreras.push_back(new Barrera(smgr,this->skin,this->velAnim,this->t->getNodo()));
    unsigned int idx=this->Barreras.size()-1;
    const std::vector<int> p=j.getPosicion();
    this->Barreras.at(idx)->ColocaBarrera( this->t->getPosicionCelda( p ),p,j.getDireccion(),this->smgr );
    this->t->setBarrera(idJugador, *this->Barreras.at(idx));   // MODEL update (unchanged)

    // hide until its turn in the replay
    if(this->Barreras.at(idx)->getNodo()) this->Barreras.at(idx)->getNodo()->setVisible(false);

    EventoJugada ev;
    ev.tipo=EventoJugada::BARRERA; ev.jugador=idJugador;
    ev.hasta=this->Barreras.at(idx)->getPosicionEscena();   // final slot (ColocaBarrera set it)
    ev.barreraIdx=(int)idx;
    this->eventos.push(ev);
 }

 void PartidaGrafica::ColocaAntorchas(){


     int x,x1,z,z1,y,aumento=5;
     core::vector3df v=this->t->getPosicionCelda(0,0);
    x=v.X+(-aumento-this->t->getEscala().X);
    x1=(aumento+this->t->getsizeLineaCeldas().X);
    z=v.Z+(-aumento-this->t->getEscala().Z);
    z1=(aumento+this->t->getsizeLineaCeldas().Z);
    y=v.Y+(this->t->getEscala().Y);
     this->antorchas.at(0)->setPosicionAntorcha(x,y,z);
     this->antorchas.at(1)->setPosicionAntorcha(x1,y,z);
     this->antorchas.at(2)->setPosicionAntorcha(x1,y,z1);
     this->antorchas.at(3)->setPosicionAntorcha(x,y,z1);
 }

 void PartidaGrafica::SetEscala(int x, int y, int z){
      this->escala.X=x,this->escala.Y=y,this->escala.Z=z;
      this->parent->setScale(this->escala);
     this->t->setEscalaTablero(this->escala.X,this->escala.Y,this->escala.Z);
      for(int i=0;i<this->jugadores.size();i++){
          Grafico::Jugador *ju=(Grafico::Jugador*)this->jugadores.at(i);
          ju->setEscala(this->escala.X,this->escala.Y,this->escala.Z);
      }
      for(std::size_t i = 0; i < this->antorchas.size(); i++){
         this->antorchas.at(i)->setEscalaAntorcha(this->escala.X,this->escala.Y,this->escala.Z);
    }

 }

 bool PartidaGrafica::SetJugadores(std::string rutaAgente1,std::string rutaAgente2){
     


      std::vector<Reglas::Agente*> agentes=this->getAgentes(rutaAgente1,rutaAgente2,this->t);

      this->jugadores.push_back(new Grafico::Jugador(smgr,0, agentes[0],this->skin,this->velAnim,this->parent));
      this->jugadores.push_back(new Grafico::Jugador(smgr,1, agentes[1],this->skin,this->velAnim,this->parent));

      this->t->setJugadores( this->jugadores);
      // D1-SIGTRAP class fix: a bool fn that falls off the end is UB — modern
      // clang emits brk #0x1 (runtime SIGTRAP). Failures above throw, so
      // reaching here means success.
      return true;
 }

core::vector3df PartidaGrafica::getCentro(){
     core::vector3df pos=this->t->getPosicionTablero();
     core::vector3df tam=this->t->getSize()*this->escala;
     core::vector3df cen;
     cen.X=pos.X+(tam.X/2);
     cen.Z=pos.Z+(tam.Z/2);
     cen.Y=pos.Y;
     return cen;
}
bool PartidaGrafica::animacionesEnd(){
    return !this->animador.hayBloqueante();
}

void PartidaGrafica::produceAll(){
    // Eager: run the whole match logically, filling the event queue.
    while(this->estaEnCurso()){
        this->siguienteJugada();
    }
}

void PartidaGrafica::reproducir(const EventoJugada& ev){
    const u32 DUR=350;          // ms per move (tune via vision)
    const f32 ALTURA=40.0f;     // jump peak (cells ~50u); tune via vision
    if(ev.tipo==EventoJugada::MOVIMIENTO){
        Grafico::Jugador* ju=(Grafico::Jugador*)this->jugadores.at(ev.jugador);
        f32 h=ALTURA*(ev.numCeldas>1?1.6f:1.0f);   // higher hop when jumping over
        this->animador.agregar(new Grafico::SaltoAnim(ju, ev.desde, ev.hasta, DUR, h));
    } else { // BARRERA
        Grafico::Barrera* b=this->Barreras.at(ev.barreraIdx);
        core::vector3df desde=ev.hasta + core::vector3df(0, 60, 0);  // drop in from above
        this->animador.agregar(new Grafico::DeslizarAnim(b, desde, ev.hasta, DUR));
    }
}

void PartidaGrafica::update(u32 dtMs){
    if(!this->animador.hayBloqueante() && !this->eventos.empty()){
        EventoJugada ev=this->eventos.front(); this->eventos.pop();
        this->reproducir(ev);
    }
    this->animador.update(dtMs);
}

bool PartidaGrafica::terminadoVisual(){
    return this->eventos.empty() && this->animador.vacio();
}
void PartidaGrafica::dropBarreras(){

     for(int i=0;i<this->Barreras.size();i++){
         std::cout<<"borrando la barrera : "<<i<<" de "<<this->Barreras.size()<<std::endl;
         delete( this->Barreras.at(i) );
     }
     this->Barreras.clear();
 }

void PartidaGrafica::dropJugadores() {
     for(int i=0;i<this->jugadores.size();i++){
         std::cout<<"borrando el jugador: "<<i<<" de "<<this->jugadores.size()<<std::endl;
            Grafico::Jugador *ju=(Grafico::Jugador*)this->jugadores.at(i);
            delete(ju);
      }
     this->jugadores.clear();
 }
