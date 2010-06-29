/**
 */

#include "Jugador.hpp"
#include "Juez.hpp"
#include "Agente.hpp"

Reglas::Jugador::Jugador(int num, Reglas::Agente *a) : Pieza()
{
    this->num = num;
    this->barreras_disponibles = Jugador::barreras_iniciales;
    this->agente = a;
}

Reglas::Jugador::Jugador(const Jugador& orig) : Pieza(orig)
{
    this->num = orig.num;
    this->barreras_disponibles = orig.barreras_disponibles;
    this->agente = orig.agente;
}

Reglas::Jugador::~Jugador()
{
    this->terminar();
    delete( this->agente );
}

void Reglas::Jugador::mover(const std::vector<int> &nuevaPos)
{
    if(!this->esta_colocado)
        throw JugadorNoColocado(); //TODO: agregar info del error.
    if(nuevaPos.size() != 2)
        throw ParametrosMalos();

    //FIXME: llamar al constructor copia del padre!
    this->posicion = nuevaPos;
}

int Reglas::Jugador::getBarrerasDisponibles() const
{
    return this->barreras_disponibles;
}

void Reglas::Jugador::decBarrerasDisponibles()
{
    if(this->barreras_disponibles <= 0)
        throw JugadorSinBarreras();

    this->barreras_disponibles--;
}

void Reglas::Jugador::incBarrerasDisponibles()
{
    this->barreras_disponibles++;
}

int Reglas::Jugador::getIdentificador() const
{
    return this->num;
}

void Reglas::Jugador::iniciar(int id) const
{
    this->agente->iniciar(this->num);
}

Reglas::Jugada Reglas::Jugador::siguienteJugada() const
{
    return this->agente->siguienteJugada();
}

void Reglas::Jugador::terminar() const
{
    this->agente->terminar();
}