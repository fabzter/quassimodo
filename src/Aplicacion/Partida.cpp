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
     //TODO: Pedir en verdad a los agentes!
    std::vector<Reglas::Agente*> agentes;
    
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
    Reglas::Jugada j= this->juez->siguienteJugada(this->jugador_en_turno);
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