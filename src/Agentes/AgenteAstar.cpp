#include "AgenteAstar.hpp"
#include <Reglas/Astar.hpp>

Agentes::AgenteAstar::AgenteAstar()
{
}

Agentes::AgenteAstar::AgenteAstar(const AgenteAstar& orig)
{
}

Agentes::AgenteAstar::~AgenteAstar()
{
}

void Agentes::AgenteAstar::iniciar(int id)
{
    this->id = id;
}

Reglas::Jugada Agentes::AgenteAstar::siguienteJugada(const Reglas::Tablero tablero,
                                            Reglas::AyudanteDeAgente &ayudante)
{
    std::vector<void*>* path = Reglas::astar((Reglas::Tablero*)&tablero, this->id);

    if(!path->empty())
    {
        Reglas::Jugada j((Reglas::Celda*)path->at(1));
        delete path;
        return j;
    }
    else
    {
        delete path;
        return ayudante.getMovimientosPosibles(this->id).front();
    }
}

void Agentes::AgenteAstar::terminar()
{
    
}

Reglas::Agente* Agentes::FabricaAstar::operator ()()
{
    return new AgenteAstar();
}