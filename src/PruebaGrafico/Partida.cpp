

#include <irrlicht/vector3d.h>

#include "Partida.hpp"
using namespace irr;
using namespace Grafico;

Partida::Partida(scene::ISceneManager* smgr,Grafico::Skin* skin) {
    this->skin=skin;
    this->t=new Tablero(smgr,this->skin);
    this->en_curso = this->hay_ganador = false;
    this->jugador_ganador = this->jugador_en_turno = 0;
    this->juez = NULL; //esto es solo para destruirlo bien!
    this->juez = new Reglas::Juez(*t);
    this->escala.X=1,this->escala.Y=1,this->escala.Z=1;
    this->antorchas.reserve(4);
    this->antorchas.resize(4);
    this->Barreras.reserve(20);
    this->jugadores.reserve(2);
   

     for(std::size_t i = 0; i < this->antorchas.size(); i++){
         this->antorchas.at(i)=new Antorcha(smgr,0,0,this->skin);
    }
       core::vector3df v= this->t->getSize();
     this->t->setPosicionTablero(-v.X/2,0,(v.Z/2)-15);
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
        this->MoverJugador(j,id,NULL);
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
        throw;
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
        this->MoverJugador(j,idJugador,smgr);
    }
    else if(j.getTipoDeJugada() == Reglas::BARRERA)
    {
        this->SetBarrera(j,idJugador,smgr);
    }
}

bool Partida::MoverJugador(Reglas::Jugada &j, int idJugador,scene::ISceneManager* smgr){
    Grafico::Jugador *ju=(Grafico::Jugador*)this->jugadores.at(idJugador);
    core::vector3df p=this->t->getPosicionCelda(  j.getPosicion() )  ;
    p.Y+=this->t->getsizeCelda().Y*this->escala.Y;
    p.Z+=( ( this->t->getsizeCelda().Z*this->escala.Z ) /2);
    p.X+=( ( this->t->getsizeCelda().X*this->escala.X ) /2);
    bool termino=true;
    if(smgr!=NULL)
        termino=ju->Mover(smgr,p);
    else
        ju->setPosicion(p);
    this->t->moverJugador(idJugador, j.getPosicion());
    return termino;
}

 void Partida::SetBarrera(Reglas::Jugada &j, int idJugador,scene::ISceneManager* smgr){

     this->Barreras.push_back(new Barrera(smgr,this->skin));
         unsigned int pos=this->Barreras.size();
        const std::vector<int> p=j.getPosicion();
        
       this->Barreras.at(pos-1)->ColocaBarrera( this->t->getPosicionCelda( p ),p,j.getDireccion()  );
        this->t->setBarrera(idJugador, *this->Barreras.at(pos-1));
        this->Barreras.at(pos-1)->setEscala(this->escala.X,this->escala.Y,this->escala.Z);
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

 void Partida::SetEscala(int x, int y, int z){
      this->escala.X=x,this->escala.Y=y,this->escala.Z=z;
     this->t->setEscalaTablero(this->escala.X,this->escala.Y,this->escala.Z);
      for(int i=0;i<this->jugadores.size();i++){
          Grafico::Jugador *ju=(Grafico::Jugador*)this->jugadores.at(i);
          ju->setEscala(this->escala.X,this->escala.Y,this->escala.Z);
      }

      for(std::size_t i = 0; i < this->antorchas.size(); i++){
         this->antorchas.at(i)->setEscalaAntorcha(this->escala.X,this->escala.Y,this->escala.Z);
    }
      core::vector3df v= this->t->getSize();
      this->t->setPosicionTablero(-(v.X*x) /2,0,-( (v.Z*z) /2));
       std::cout<<"posicion t "<< (v.X*x) /2<<" "<<( (v.Z*z) /2)-15<<std::endl;
     this->ColocaAntorchas();
 }

 bool Partida::SetJugadores(std::string rutaAgente1,std::string rutaAgente2,scene::ISceneManager* smgr,scene::IAnimationEndCallBack* callback){

      Scripting::Manejador *m = new Scripting::Manejador(*t);
      std::vector<Reglas::Agente*> agentes;
      agentes.push_back(m->getAgente(rutaAgente1));
      agentes.push_back(m->getAgente(rutaAgente2));
      
      this->jugadores.push_back(new Grafico::Jugador(smgr,0, agentes[0],callback,this->skin));
     this->jugadores.push_back(new Grafico::Jugador(smgr,1, agentes[1],callback,this->skin));
       
      this->t->setJugadores( this->jugadores);
      this->SetEscala(this->escala.X,this->escala.Y,this->escala.Z);
 }

void Partida::NuevaPartida(scene::ISceneManager* smgr){
      for(int i=0;i<this->jugadores.size();i++){
            Grafico::Jugador *ju=(Grafico::Jugador*)this->jugadores.at(i);
            ju->terminar();
             ju->drop();
      }
      delete(this->t);
      this->t=new Tablero(smgr,this->skin);
      this->jugadores.clear();
}

core::vector3df Partida::getCentro(){
     core::vector3df pos=this->t->getPosicionTablero();
     core::vector3df tam=this->t->getSize()*this->escala;
     core::vector3df cen;
     cen.X=pos.X+(tam.X/2);
     cen.Z=pos.Z+(tam.Z/2);
     cen.Y=pos.Y;
     return cen;
}