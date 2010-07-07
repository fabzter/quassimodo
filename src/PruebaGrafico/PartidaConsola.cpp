
#include "PartidaConsola.hpp"

PartidaConsola::PartidaConsola():Partida() {

    this->t=new Reglas::Tablero();
    this->juez = new Reglas::Juez(*this->t);
}

PartidaConsola::PartidaConsola(const PartidaConsola& orig): Partida(orig) {
}

PartidaConsola::~PartidaConsola() {
    delete this->t;
}
void PartidaConsola::iniciarPartida(){

    if(this->en_curso)
        throw Reglas::PartidaNoIniciada();

    for(int id = 0; id < this->t->num_jugadores; id++)
    {
        this->t->getJugador(id).iniciar(id);
    }

    this->en_curso = true;
}

bool PartidaConsola::SetJugadores(std::string rutaAgente1,std::string rutaAgente2){

     std::vector<Reglas::Agente*> agentes=this->getAgentes(rutaAgente1,rutaAgente2,this->t);

      this->jugadores.push_back( new Reglas::Jugador(0, agentes[0]) );
      this->jugadores.push_back( new Reglas::Jugador(1, agentes[1]) );

      this->t->setJugadores( this->jugadores);
}
void PartidaConsola::CreaBarraProgreso(){

}

void PartidaConsola::AumentaBarraProgreso(){

}

void PartidaConsola::EliminaBarraProgreso(){

}
bool PartidaConsola::siguienteJugada()
{
    return this->Siguiente(this->t);
}
void PartidaConsola::actualizarTablero(Reglas::Jugada &j, int idJugador){

    if(j.getTipoDeJugada() == Reglas::MOVIMIENTO)
    {
        this->t->moverJugador(idJugador, j.getPosicion());
    }
    else if(j.getTipoDeJugada() == Reglas::BARRERA)
    {
        Reglas::Barrera b;
        b.colocar(j.getPosicion(), j.getDireccion());
        this->t->setBarrera(idJugador, b);
    }
}
void PartidaConsola::impimeTablero(){

    std::cout<<*this->t<<std::endl;
}
