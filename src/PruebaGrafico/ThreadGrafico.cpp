
#include "ThreadGrafico.hpp"
#include"Partida.hpp"

ThreadGrafico::ThreadGrafico(Partida* partida) {
    this->partida=partida;
    this->corriendo=true;
    this->partida->creaBarra();
}

ThreadGrafico::ThreadGrafico(const ThreadGrafico& orig) {
}

ThreadGrafico::~ThreadGrafico() {
    if(this->corriendo)
        this->stop();
}
void ThreadGrafico::operator()(){

    while(this->corriendo){
        this->partida->AumentaBarra();
    }

}
void ThreadGrafico::stop(){
    this->corriendo=false;
    this->partida->dropBarra();
}