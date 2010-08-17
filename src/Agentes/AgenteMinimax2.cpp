#include "AgenteMinimax2.hpp"
#include <Reglas/Astar.hpp>
#include <Reglas/Minimax.hpp>
#include <vector>

Agentes::AgenteMinimax2::AgenteMinimax2()
{
}

Agentes::AgenteMinimax2::AgenteMinimax2(const AgenteMinimax2& orig)
{
}

Agentes::AgenteMinimax2::~AgenteMinimax2()
{
}

void Agentes::AgenteMinimax2::iniciar(int id)
{
    this->id = id;
    this->id_enemigo = id == 0? 1: 0;
}

Reglas::Jugada Agentes::AgenteMinimax2::siguienteJugada(const Reglas::Tablero tablero,
                                              Reglas::AyudanteDeAgente& ayudante)
{
    using namespace Reglas;
    std::vector<void*>* path_yo = Reglas::astar(&tablero, this->id);
    std::vector<void*>* path_enemigo = Reglas::astar(&tablero, this->id_enemigo);

    int len_path_yo = path_yo->size();
    int len_path_enemigo = path_enemigo->size();

    delete path_yo;
    delete path_enemigo;

    if( (len_path_enemigo <= len_path_yo) &&
            (ayudante.getBarrerasPosibles(this->id).size() > 0) )
    {
        return minimax((Tablero*)&tablero, this->id, 0, 2, BARRERA);
    }

    try
    {
        std::vector<void*>* my_path = astar(&tablero, this->id);
        Jugada j( (Celda*)my_path->at(1) );
        delete my_path;
        
        return j;
    }
    catch(std::out_of_range)
    {
        return minimax((Tablero*)&tablero, this->id, 0, 2, MOVIMIENTO);
    }
}

Reglas::Agente* Agentes::FabricaMinimax2::operator ()()
{
    return new AgenteMinimax2();
}

void Agentes::AgenteMinimax2::terminar()
{
}