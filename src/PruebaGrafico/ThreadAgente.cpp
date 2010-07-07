
#include "ThreadAgente.hpp"

ThreadAgente::ThreadAgente(Reglas::Juez* juez,int j_turno) {
    this->juez=juez;
    this->jugador_turno=j_turno;
    this->en_curso=this->exitoso=false;
}

ThreadAgente::ThreadAgente(const ThreadAgente& orig) {
}

ThreadAgente::~ThreadAgente() {
}

void ThreadAgente::operator()(){
    this->en_curso=true;
    try{
        this->j=this->juez->siguienteJugada(this->jugador_turno);
    }
    catch(std::exception &e){
        this->error=e.what();
        this->en_curso=this->exitoso=false;
        return;
    }
    this->en_curso=false;
    this->exitoso=true;

}
bool ThreadAgente::enCurso(){
    return this->en_curso;
}
bool ThreadAgente::isExitoso(){
    return this->exitoso;
}
std::string ThreadAgente::getError(){
    return this->error;
}
Reglas::Jugada& ThreadAgente::getJugada(){
    return this->j;
}