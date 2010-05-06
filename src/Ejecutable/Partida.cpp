#include "Partida.hpp"
#include <Reglas/Juez.hpp>

Partida::Partida(Reglas::Tablero *t)
{
    this->en_curso = this->hay_ganador = false;
    this->jugador_ganador = this->jugador_en_turno = 0;
    
    this->juez = NULL; //esto es solo para destruirlo bien!
    this->tablero = t;
    this->juez = new Reglas::Juez(*t);
}

Partida::Partida(const Partida& orig)
{
}

Partida::~Partida()
{
    if(this->juez != NULL)
        delete this->juez;
}

void Partida::iniciarPartida()
{
    if(this->en_curso)
        throw Reglas::PartidaNoIniciada();

    for(int id = 0; id < this->tablero->num_jugadores; id++)
    {
        this->tablero->getJugador(id).iniciar(id);
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
    j = this->juez->siguienteJugada(this->jugador_en_turno);
    this->en_curso = true;

    this->actualizarTablero(j, this->jugador_en_turno);
    //actualizamos el Jugador en turno.
    this->jugador_en_turno =
            ++(this->jugador_en_turno) % this->tablero->num_jugadores;

    //Actualizamos Banderas:
    int idGanador = this->juez->hayGanador();
    if(idGanador >= 0)
    {
        this->hay_ganador = true;
        this->jugador_ganador = idGanador;
        this->en_curso = false;
    }
}

void Partida::actualizarTablero(Reglas::Jugada &j, int idJugador)
{
    if(j.getTipoDeJugada() == Reglas::MOVIMIENTO)
    {
        this->tablero->moverJugador(idJugador, j.getPosicion());
    }
    else if(j.getTipoDeJugada() == Reglas::BARRERA)
    {
        Reglas::Barrera b;
        b.colocar(j.getPosicion(), j.getDireccion());
        this->tablero->setBarrera(idJugador, b);
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