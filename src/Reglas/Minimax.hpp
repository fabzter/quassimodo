#ifndef _MINIMAX_HPP
#define	_MINIMAX_HPP

#include "Jugada.hpp"
#include "Tablero.hpp"
#include "AyudanteDeAgente.hpp"
#include <list>

namespace Reglas
{
    
const bool MAX = true;
const bool MIN = false;
const char CELLS = 81;

struct NodoMinimax
{
    NodoMinimax(Tablero *t);

    bool tipo; //MIN O MAX
    TipoDeJugada tipoDeJugadaInicial; //el tipo de la Jugada que se pidio desde el inicio.
    char idJugador; //jugador al que le toca tirar sobre el tablero.
    char idEnemigo; 
    Jugada jugada; //la jugada que llevó a este estado.
    float val; //su estimacion.
    std::list<NodoMinimax*> hijos;

    bool estaEn(std::list<NodoMinimax*> &lista);

    Tablero *tablero;
};

struct CompararNodoMinimaxMIN
{
    bool operator()(NodoMinimax* a, NodoMinimax* b);
};

struct EliminarJugadas
{
    EliminarJugadas(AyudanteDeAgente &a, int idEnemigo);
    EliminarJugadas(const EliminarJugadas &other);
    bool operator()(Jugada &j);

private:
    AyudanteDeAgente *ayudante;
    int id_enemigo;
};

void minimax(NodoMinimax *currentTab, int currentDepth, int maxDepth,
                TipoDeJugada tipoJug);

Jugada minimax(Reglas::Tablero *currentTab, int idJugador, int currentDepth,
                int maxDepth, TipoDeJugada tipoJug);

float evaluate(NodoMinimax *nodo);

}

#endif	/* _MINIMAX_HPP */
