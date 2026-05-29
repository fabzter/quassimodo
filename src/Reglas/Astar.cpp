#include <complex>

#include "Astar.hpp"
#include "AyudanteDeAgente.hpp"

Reglas::Mapa::Mapa(Tablero& t, int idJugador)
{
    this->init(t, idJugador);
}

Reglas::Mapa::Mapa(Tablero* t, int idJugador)
{
    this->init(*t, idJugador);
}

void Reglas::Mapa::init(Tablero& t, int idJugador)
{
    this->t = &t;
    this->pather = new micropather::MicroPather(this, 250, 6);
    this->idJugador = idJugador;
}

Reglas::Mapa::~Mapa()
{
    delete this->pather;
}

float Reglas::Mapa::LeastCostEstimate(void* stateStart, void* stateEnd)
{
    const std::vector<int>& a = ((Celda*)stateStart)->getPosicion();
    const std::vector<int>& b = ((Celda*)stateEnd)->getPosicion();

    // Weighted Manhattan distance: x-movement costs 1.3 per cell vs y-movement
    // at 1.0. Strictly non-admissible (true cost is 1 per cell either way), so
    // A* may return non-minimal paths — but the bias is intentional. Players
    // progress along the y-axis toward their meta row; this heuristic discourages
    // pointless lateral detours and keeps the pawn on its progress lane.
    return std::abs( (b.at(0) - a.at(0)) * 1.3 ) + std::abs(b.at(1) - a.at(1));
}

void Reglas::Mapa::AdjacentCost(void* state,
                                std::vector<micropather::StateCost>* adjacent)
{
    //creamos un tablero donde el jugador este en state.
    Celda* celda_actual = (Celda*)state;

    Tablero tab_copia(this->t);

    tab_copia.moverJugador(idJugador, celda_actual->getPosicion());

    //aqui van los movimientos posibles del jugador sobre la copia del Tablero.
    std::list<Reglas::Jugada> jugs_mov;

    {//parentesis para destruir al ayudante_copia
    AyudanteDeAgente ayudante_copia(tab_copia);

    //sacamos a todos sus vecinos desde state.
    jugs_mov = ayudante_copia.getMovimientosPosibles(this->idJugador);
    }//parentesis para destruir al ayudante_copia
    
    std::list<Reglas::Jugada>::iterator it_jugada;
    for(it_jugada = jugs_mov.begin(); it_jugada != jugs_mov.end(); it_jugada++)
    {
        micropather::StateCost vecino = {
            (void*)&(this->t->getCelda( it_jugada->getPosicion() )), 1
        };

        adjacent->push_back(vecino);
    }
}

void Reglas::Mapa::PrintStateInfo(void *state)
{
    Celda *c = (Celda*)state;
    Tablero tab_copia(this->t);

    tab_copia.moverJugador(this->idJugador, *c);

    std::cout << "State Info:\n" << tab_copia << std::endl;
}

micropather::MicroPather* Reglas::Mapa::getPather()
{
    return this->pather;
}

std::vector<void*>* Reglas::astar(Reglas::Tablero *t, int idJugador)
{
    Mapa mapa(t, idJugador);

    std::vector< void* >* path;
    std::vector< void* >* min_path = NULL;
    float totalCost;
    float min_cost = 99999;

    const std::vector<Reglas::Celda>& metas = t->getMetas(idJugador);

    //iteramos por todas las metas del jugador.
    std::vector<Reglas::Celda>::const_iterator meta_it;
    for(meta_it = metas.begin(); meta_it != metas.end(); meta_it++)
    {
        //si no esta libre, pues ni la consideramos
        if(!meta_it->estaLibre())
            continue;

        path = new std::vector< void* >();

        int result = mapa.getPather()->Solve( (void*)&(t->getCelda(idJugador)),
                        (void*)&(*meta_it), path, &totalCost );

        if(result == micropather::MicroPather::START_END_SAME)
        {
            result = micropather::MicroPather::SOLVED;
            totalCost = 0;
        }
        if(result == micropather::MicroPather::NO_SOLUTION)
        {
            // Meta unreachable from current position — skip it cleanly.
            // The previous code reclassified NO_SOLUTION as SOLVED with a
            // magic cost of 82 (>81-cell-board worst case), so any actual
            // SOLVED path would still win comparison; the side effect was
            // that if EVERY meta was unreachable, min_path became an empty
            // (but non-NULL) std::vector*, lying to the caller. With the
            // __astar__ wrapper's NULL guard in place (D1' fixes PR), NULL
            // is the honest signal: the wrapper translates it to an empty
            // Celda* vector and the agent's IndexError fallback fires.
            delete path;
            continue;
        }
        if(result == micropather::MicroPather::SOLVED )
        {
            if(totalCost < min_cost)
            {
                if(min_path != NULL)
                    delete min_path;
                min_cost = totalCost;
                min_path = path;
            }
            else
            {
                delete path;
                path = NULL;
            }
        }
        else
        {
            delete path;
            path = NULL;
        }
    }
    return min_path;
}
