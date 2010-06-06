#ifndef _ASTAR_HPP
#define	_ASTAR_HPP
#include "Tablero.hpp"
#include "AyudanteDeAgente.hpp"
#include "micropather.h"
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <cmath>
#include <functional>

namespace Reglas
{
class Mapa : micropather::Graph
{
public:
    Mapa(Tablero *t, int idJugador);
    Mapa(Tablero &t, int idJugador);
    virtual ~Mapa();

    float LeastCostEstimate(void *stateStart, void *stateEnd);
    void AdjacentCost(void *state,
                            std::vector< micropather::StateCost > *adjacent);
    void PrintStateInfo(void *state);

    micropather::MicroPather* getPather();
    
private:
    void init(Tablero &t, int idJugador);
    Tablero *t;
    int idJugador;
    micropather::MicroPather* pather;
};

Jugada astar(Tablero *t, int idJugador);
}
#endif	/* _ASTAR_HPP */

