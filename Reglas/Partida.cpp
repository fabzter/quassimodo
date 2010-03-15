#include "Partida.hpp"
#include "Juez.hpp"

Reglas::Partida::Partida(Tablero *t)
{
    this->en_curso = this->hay_ganador = false;
    this->jugador_ganador = this->jugador_en_turno = 0;
    
    this->juez = NULL; //esto es solo para destruirlo bien!
    this->tablero = t;
    this->juez = new Juez(*t);

    this->en_curso = true;
}

Reglas::Partida::Partida(const Partida& orig)
{
}

Reglas::Partida::~Partida()
{
    if(this->juez != NULL)
        delete this->juez;
}

bool Reglas::Partida::siguienteJugada()
{
    if(!this->en_curso)
        throw PartidaTerminada();
    //pedimos la Jugada y enviamos excepciones
    Jugada j;
    try
    {
         j = this->juez->siguienteJugada(this->jugador_en_turno);
    }
    catch(ReglasRotas &e)
    {
        this->en_curso = false;
        throw;
    }
    catch(Scripting::ScriptMalo &e)
    {
        this->en_curso = false;
        throw;
    }
    //actualizamos el Jugador en turno.
    this->jugador_en_turno =
            ++(this->jugador_en_turno) % this->tablero->num_jugadores;

    this->actualizarTablero(j, this->jugador_en_turno);

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