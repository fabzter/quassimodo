#include "Partida.hpp"
#include "Juez.hpp"

Reglas::Partida::Partida(Tablero *t)
{
    this->en_curso = this->hay_ganador = false;
    this->jugador_ganador = this->jugador_en_turno = 0;
    
    this->juez = NULL; //esto es solo para destruirlo bien!
    this->tablero = t;
    this->juez = new Juez(*t);
}

Reglas::Partida::Partida(const Partida& orig)
{
}

Reglas::Partida::~Partida()
{
    if(this->juez != NULL)
        delete this->juez;
}

void Reglas::Partida::iniciarPartida()
{
    if(this->en_curso)
        throw PartidaNoIniciada();

    for(int id = 0; id < this->tablero->num_jugadores; id++)
    {
        this->tablero->getJugador(id).iniciar(id);
    }

    this->en_curso = true;
}

bool Reglas::Partida::siguienteJugada()
{
    if(!this->en_curso)
        throw PartidaTerminada();

    this->en_curso = false; //si todo sale bien, la regresamos a en_curso = true
    //pedimos la Jugada y enviamos excepciones
    Jugada j;
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

void Reglas::Partida::actualizarTablero(Reglas::Jugada &j, int idJugador)
{
    if(j.getTipoDeJugada() == MOVIMIENTO)
    {
        this->tablero->moverJugador(idJugador, j.getPosicion());
    }
    else if(j.getTipoDeJugada() == BARRERA)
    {
        Barrera b;
        b.colocar(j.getPosicion(), j.getDireccion());
        this->tablero->setBarrera(idJugador, b);
    }
}

bool Reglas::Partida::estaEnCurso()
{
    return this->en_curso;
}