

#include "Partida.hpp"


Partida::Partida() {
    this->juez = NULL;
    this->jugadores.reserve(2);
    this->en_curso = this->hay_ganador = false;
    this->jugador_ganador = this->jugador_en_turno = 0;
 
}

Partida::Partida(const Partida& orig) {
}

Partida::~Partida() {
     if(this->juez != NULL)
        delete this->juez;
}

bool Partida::estaEnCurso()
{
    return this->en_curso;
}

bool Partida::hayGanador()
{
    return this->hay_ganador;
}
int Partida::getJugadorGanador(){

    return this->jugador_ganador;
}

std::vector<Reglas::Agente*> Partida::getAgentes(std::string rutaAgente1,std::string rutaAgente2,Reglas::Tablero* t){
     //TODO: revisar si los agentes son NULL
     Scripting::Manejador *m;
     std::vector<Reglas::Agente*> agentes;
     try{
        m= new Scripting::Manejador(*t);
     }
     catch(Scripting::Excepcion &e){
         this->errorEnAgente='0';
         delete(m);
         throw;
     }

      try{
          agentes.push_back(m->getAgente(rutaAgente1));
          
          
      }
      catch(Scripting::Excepcion &e){
          this->errorEnAgente='1';
          delete(m);
          throw;

      }
      try{
          agentes.push_back(m->getAgente(rutaAgente2));
          }
         catch(Scripting::Excepcion &e){
              this->errorEnAgente='2';
              delete(m);
              delete agentes.at(0);
              throw;

          }

      delete(m);
      return agentes;
}
 char Partida::getAgenteConError(){
     return this->errorEnAgente;
 }

bool Partida::Siguiente(Reglas::Tablero *t){

     if(!this->en_curso)
        throw Reglas::PartidaTerminada();
    this->en_curso = false; //si todo sale bien, la regresamos a en_curso = true
    //pedimos la Jugada y enviamos excepciones
    Reglas::Jugada j;
    ThreadGrafico th(this);
    boost::thread thread( boost::ref(th) );

    j=this->juez->siguienteJugada(this->jugador_en_turno);
    th.stop();
    thread.join();
    this->en_curso = true;
    this->actualizarTablero(j, this->jugador_en_turno);
    //actualizamos el Jugador en turno.
    this->jugador_en_turno =
            ++(this->jugador_en_turno) % t->num_jugadores;

    //Actualizamos Banderas:
    int idGanador = this->juez->hayGanador();
    if(idGanador >= 0)
    {
        this->hay_ganador = true;
        this->jugador_ganador = idGanador+1;
        this->en_curso = false;
    }
    return this->en_curso;
}