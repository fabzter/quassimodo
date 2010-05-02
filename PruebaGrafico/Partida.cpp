

#include <irrlicht/vector3d.h>

#include "Partida.hpp"
using namespace irr;
using namespace Grafico;

Partida::Partida(scene::ISceneManager* smgr) {
    this->t=new Tablero(smgr);
    this->en_curso = this->hay_ganador = false;
    this->jugador_ganador = this->jugador_en_turno = 0;
    this->juez = NULL; //esto es solo para destruirlo bien!
    //TODO: hacer el Juez Grafico!!
    this->juez = new Reglas::Juez(*t);

    this->antorchas.reserve(4);
    this->antorchas.resize(4);
     this->Barreras.reserve(20);
     this->jugadores.reserve(2);
   

     for(std::size_t i = 0; i < this->antorchas.size(); i++){
         this->antorchas.at(i)=new Antorcha(smgr,0,0);
    }
     this->ColocaAntorchas();
}

Partida::Partida(const Partida& orig) {
}

Partida::~Partida() {
     if(this->juez != NULL)
        delete this->juez;
     
     for(int i=0;i<this->antorchas.size();i++){
     
             this->antorchas.at(i)->dropAntorcha();
         if(i<this->jugadores.size()){
             Grafico::Jugador *ju=(Grafico::Jugador*)this->jugadores.at(i);
             ju->drop();
         }
     }
    for(int i=0;i<this->Barreras.size();i++){
         this->Barreras.at(i)->drop();
     }
         
     delete(this->t);
}

void Partida::iniciarPartida()
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
bool Partida::siguienteJugada(scene::ISceneManager* smgr)
{
    if(!this->en_curso)
        throw Reglas::PartidaTerminada();

    this->en_curso = false; //si todo sale bien, la regresamos a en_curso = true
    //pedimos la Jugada y enviamos excepciones
    Reglas::Jugada j;
    try{
    j = this->juez->siguienteJugada(this->jugador_en_turno);
    this->en_curso = true;
    }
    catch(Reglas::Excepcion e ){
        std::cout<<"eeror del jugador"<<this->jugador_en_turno<<" que es: "<<e.what()<<std::endl;
        return this->en_curso;
    }

    this->actualizarTablero(j, this->jugador_en_turno,smgr);
    //actualizamos el Jugador en turno.
    this->jugador_en_turno =
            ++(this->jugador_en_turno) % this->t->num_jugadores;

    //Actualizamos Banderas:
    int idGanador = this->juez->hayGanador();
    if(idGanador >= 0)
    {
        this->hay_ganador = true;
        this->jugador_ganador = idGanador;
        this->en_curso = false;
    }
    return this->en_curso;
}

void Partida::actualizarTablero(Reglas::Jugada &j, int idJugador,scene::ISceneManager* smgr)
{
    if(j.getTipoDeJugada() == Reglas::MOVIMIENTO)
    {
        MoverJugador(j,idJugador);
    }
    else if(j.getTipoDeJugada() == Reglas::BARRERA)
    {
        this->Barreras.push_back(new Barrera(smgr));
         unsigned int pos=this->Barreras.size();
        
        const std::vector<int> p=j.getPosicion();
       this->Barreras.at(pos-1)->ColocaBarrera( this->t->getPosicionCelda( p ),p,j.getDireccion()  );
        this->t->setBarrera(idJugador, *this->Barreras.at(pos-1));
    }
}
void Partida::MoverJugador(Reglas::Jugada &j, int idJugador){
    Grafico::Jugador *ju=(Grafico::Jugador*)this->jugadores.at(idJugador);
    core::vector3df p=this->t->getPosicionCelda(  j.getPosicion() )  ;
    p.Y+=this->t->getsizeCelda().Y;
    p.Z+=(this->t->getsizeCelda().Z /2);
    p.X+=(this->t->getsizeCelda().X /2);
    ju->setPosicion(p);
    this->t->moverJugador(idJugador, j.getPosicion());

}
bool Partida::estaEnCurso()
{
    return this->en_curso;
}

bool Partida::hayGanador()
{
    return this->hay_ganador;
}

 void Partida::ColocaAntorchas(){

      int x,x1,z,z1,y;
    x=this->t->getPosicionTablero().X+(this->t->getEscala().X);
    x1=this->t->getPosicionTablero().X+(this->t->getsizeLineaCeldas().X*this->t->getEscala().X);
    z=this->t->getPosicionTablero().Z+(this->t->getEscala().Z);
    z1=this->t->getPosicionTablero().Z+(this->t->getsizeLineaCeldas().Z*this->t->getEscala().Z);
    y=this->t->getPosicionTablero().Y+(this->t->getEscala().Y);
     this->antorchas.at(0)->setPosicionAntorcha(x,y,z);
     this->antorchas.at(1)->setPosicionAntorcha(x1,y,z);
     this->antorchas.at(2)->setPosicionAntorcha(x1,y,z1);
     this->antorchas.at(3)->setPosicionAntorcha(x,y,z1);
 }
 void Partida::SetEscala(int x, int y, int z){
     this->t->setEscalaTablero(x,y,z);
      for(std::size_t i = 0; i < this->antorchas.size(); i++){
         this->antorchas.at(i)->setEscalaAntorcha(x,y,z);
    }
     this->ColocaAntorchas();
 }
 bool Partida::SetJugadores(std::string rutaAgente1,std::string rutaAgente2,scene::ISceneManager* smgr){

      Scripting::Manejador *m = new Scripting::Manejador(*t);
      std::vector<Reglas::Agente*> agentes;
      agentes.push_back(m->getAgente(rutaAgente1));
      agentes.push_back(m->getAgente(rutaAgente2));
      
      this->jugadores.push_back(new Grafico::Jugador(smgr,0, agentes[0]));
     this->jugadores.push_back(new Grafico::Jugador(smgr,1, agentes[1]));
       
      this->t->setJugadores( this->jugadores);
 }
  void Partida::NuevaPartida(scene::ISceneManager* smgr){
      for(int i=0;i<this->jugadores.size();i++){
            Grafico::Jugador *ju=(Grafico::Jugador*)this->jugadores.at(i);
            ju->terminar();
             ju->drop();
      }
      delete(this->t);
      this->t=new Tablero(smgr);
  }