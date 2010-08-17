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
class Tablero;

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

/*
 * Regresa el path de menor costo (representado como un vector de apuntadores a
 * void, que en realidad apuntan a las celdas) desde la posición actual del
 * jugador con id idJugador hasta su meta.
 */
std::vector<void*>* astar(const Tablero *t, int idJugador);
}
#endif	/* _ASTAR_HPP */

