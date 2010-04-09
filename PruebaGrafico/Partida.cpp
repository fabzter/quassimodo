

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
     this->jugadores.reserve(4);
    this->jugadores.resize(4);
    this->b=new Barrera(smgr);
     for(std::size_t i = 0; i < this->antorchas.size(); i++){
         this->antorchas.at(i)=new Antorcha(smgr,0,0);
    }
     core::vector3df v=this->t->getPosicionTablero();
     std::cout<<"posicion tablero partida "<<v.X<<","<<v.Y<<","<<v.Z<<std::endl;
     this->ColocaAntorchas();
     this->b->setPosicion(0,0,0);
      v=this->t->getPosicionEscena();
     std::cout<<"posicion tablero escena "<<v.X<<","<<v.Y<<","<<v.Z<<std::endl;

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
       this->jugadores.at(id)->setPosicion( this->t->getPosicionCelda( this->t->getJugador(id).getPosicion() ) );
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
        std::cout<<e.what()<<std::endl;
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
 void Partida::SetBarrera(int x,int y){
       std::vector<int> p;
       p.push_back(x);
       p.push_back(y);
       try{
        this->b->ColocaBarrera( this->t->getPosicionCelda( p),p,Reglas::NORTE  );
       }
       catch(Reglas::PiezaYaColocada e){
       }
 }
  void Partida::giraNorte(){

      this->b->giraNorte();
  }
void Partida::giraEste(){
      this->b->giraEste();
  }
 bool Partida::SetJugadores(std::string rutaAgente1,std::string rutaAgente2){

      Scripting::Manejador *m = new Scripting::Manejador(*t);
      std::vector<Reglas::Agente*> agentes;
      agentes.push_back(m->getAgente(rutaAgente1));
      agentes.push_back(m->getAgente(rutaAgente2));
      this->jugadores.push_back(new Grafico::Jugador(this->smgr,0, agentes[0]));
      this->jugadores.push_back(new Grafico::Jugador(this->smgr,1, agentes[1]));
      this->t->setJugadores(j);
 }