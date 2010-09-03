#include "AgenteAstar.hpp"
#include <Reglas/Astar.hpp>
#include <iostream>

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
    this->id_enemigo = id == 0? 1: 0;
}

Reglas::Jugada Agentes::AgenteAstar::siguienteJugada(const Reglas::Tablero tablero,
                                            Reglas::AyudanteDeAgente &ayudante)
{
    std::vector<void*>* path_mio = Reglas::astar((Reglas::Tablero*)&tablero, this->id);
    std::vector<void*>* path_enemigo = Reglas::astar((Reglas::Tablero*)&tablero, this->id_enemigo);
    Reglas::Jugada j;

    if(!path_mio->empty())
    {
        std::list<Reglas::Jugada> barreras = ayudante.getBarrerasPosibles(this->id);
        if( !barreras.empty() && (path_enemigo->size() <= path_mio->size()) )
        {
            j.setTipoDeJugada(Reglas::BARRERA);
            j.setPosicion( barreras.back().getPosicion() );
            j.setDireccion( barreras.back().getDireccion() );
            for(std::list<Reglas::Jugada>::iterator it = barreras.begin();
                    it != barreras.end(); ++it)
            {
                Reglas::Tablero tab_copia(tablero);
                Reglas::AyudanteDeAgente ayud((Reglas::Tablero*)&tablero);

                tab_copia.setBarrera(this->id, *it);

                std::vector<void*>* path_enemigo_copia =
                    Reglas::astar((Reglas::Tablero*)&tab_copia, this->id_enemigo);
                int path_enemigo_size_copia = path_enemigo_copia->size();
                delete path_enemigo_copia;

                if(path_enemigo->size() < path_enemigo_size_copia)
                {
                    j.setDireccion(it->getDireccion());
                    j.setPosicion(it->getPosicion());
                }
            }
        }
        
        else
        {
            j.setTipoDeJugada(Reglas::MOVIMIENTO);
            j.setPosicion( ((Reglas::Celda*)path_mio->at(1))->getPosicion() );
        }
        delete path_mio;
        delete path_enemigo;
        return j;
    }
    else
    {
        delete path_mio;
        delete path_enemigo;
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