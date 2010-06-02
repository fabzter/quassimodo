/* 
 * File:   Astar.hpp
 * Author: fabrizio
 *
 * Created on 2 de junio de 2010, 11:28 AM
 */

#ifndef _ASTAR_HPP
#define	_ASTAR_HPP
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <cmath>
#include <functional>
#include "Tablero.hpp"

namespace Reglas
{
 /*Constantes para el A* NO DOCUMENTAR.*/
enum AStar
{
    F,
    H,
    NUM,
    G,
    POS,
    OPEN,
    VALID,
    PARENT
};

struct AcomodadorHeap
{
    bool operator () (std::vector<int>* first, std::vector<int>* second);
};

struct TableroEnvoltorio
{
    TableroEnvoltorio(Reglas::Tablero* t);
    Reglas::Tablero* getTablero();
    bool operator ==(const TableroEnvoltorio& o) const;
    bool operator !=(const TableroEnvoltorio& o) const;

private:
    Tablero* t;
};

struct AcomodadorMap
{
    bool operator()(const TableroEnvoltorio& first, const TableroEnvoltorio& second);
};

/* Regresa los tableros que se pueden crear moviendo al Jugador con id
 * idJugador a todos sus movimientos posibles, junto con la jugada que llevo
 * a dicho Tablero. Deja la propiedad de los apuntadores al llamador.
 */
std::list< std::pair<Reglas::TableroEnvoltorio, Reglas::Jugada*> >
neighbors(Tablero *tableroActual, int idJugador);

/*Indica si el Tablero actual es meta del jugador idJugador (es decir, si el
 * Jugador con id idJugador ha llegado a su meta)
 */
bool goal(Tablero *tableroActual, int idJugador);

/*Regresa la distancia cuadrada entre los dos puntos a y b*/
int manhattan(const std::vector<int> &a, const std::vector<int> &b);

/*Regresa el costo de mover al Jugador con id idJugador de su posicion en el
 tablero del ayudante a su posicion en el Tablero destino.*/
int cost(Tablero* actual, Tablero* destino, int idJugador);

/*Regresa el costo estimado para llevar al Jugador con id idJugador de su
 posicion en el tablero del ayudante a su meta.*/
int heuristic(Tablero *tableroActual, int idJugador);

/*Realiza la busqueda A* y regresa un par de (Tablero, Jugada) con la siguiente
 * Jugada a la que se debe mover el Jugador con id idJugador*/
std::pair<Reglas::Tablero*, Reglas::Jugada*>* astar(Tablero *start_pos,
                                                    int idJugador, int limit);

}

bool operator<(const Reglas::TableroEnvoltorio &a,
                                            const Reglas::TableroEnvoltorio &b);
#endif	/* _ASTAR_HPP */

