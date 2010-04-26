

#include <irrlicht/vector3d.h>

#include "Partida.hpp"
using namespace irr;
using namespace Grafico;

Partida::Partida(scene::ISceneManager* smgr) {
    this->t=new Tablero(smgr);
    this->en_curso = this->hay_ganador = false;
    this->jugador_ganador = this->jugador_en_turno = 0;
    this->smgr=smgr;
    this->juez = NULL; //esto es solo para destruirlo bien!
    //TODO: hacer el Juez Grafico!!
    this->juez = new Reglas::Juez(*t);

    this->antorchas.reserve(4);
    this->antorchas.resize(4);
     this->Barreras.reserve(20);
      this->Barreras.resize(20);
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
}

void Partida::iniciarPartida()
{
    if(this->en_curso)
        throw Reglas::PartidaNoIniciada();

    for(int id = 0; id < this->t->num_jugadores; id++)
    {
        this->t->getJugador(id).iniciar(id);
        Grafico::Jugador *j=(Grafico::Jugador*)this->jugadores.at(id);
        core::vector3df p=this->t->getPosicionCelda( this->t->getJugador(id).getPosicion())  ;
        p.Y+=this->t->getsizeCelda().Y;
        //j->Gira(core::vector3df(0,90-(id*180),0));
         j->setPosicion(p);
         
    }

    this->en_curso = true;
}
bool Partida::siguienteJugada()
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

    this->actualizarTablero(j, this->jugador_en_turno);
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

void Partida::actualizarTablero(Reglas::Jugada &j, int idJugador)
{
    if(j.getTipoDeJugada() == Reglas::MOVIMIENTO)
    {
         Grafico::Jugador *ju=(Grafico::Jugador*)this->jugadores.at(idJugador);
        core::vector3df p=this->t->getPosicionCelda(  j.getPosicion() )  ;
        p.Y+=this->t->getsizeCelda().Y;
         ju->setPosicion(p);
        this->t->moverJugador(idJugador, j.getPosicion());
    }
    else if(j.getTipoDeJugada() == Reglas::BARRERA)
    {
         unsigned int pos=this->t->getBarrerasColocadas().size();
        this->Barreras.at(pos) =new Barrera(this->smgr);
        const std::vector<int> p=j.getPosicion();
        this->Barreras.at(pos)->ColocaBarrera( this->t->getPosicionCelda( p ),p,j.getDireccion()  );
        this->t->setBarrera(idJugador, *this->Barreras.at(pos));
    }
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
 bool Partida::SetJugadores(std::string rutaAgente1,std::string rutaAgente2){

      Scripting::Manejador *m = new Scripting::Manejador(*t);
      std::vector<Reglas::Agente*> agentes;
      agentes.push_back(m->getAgente(rutaAgente1));
      agentes.push_back(m->getAgente(rutaAgente2));
      
      this->jugadores.push_back(new Grafico::Jugador(this->smgr,0, agentes[0]));
     this->jugadores.push_back(new Grafico::Jugador(this->smgr,1, agentes[1]));
       
      this->t->setJugadores( this->jugadores);
 }