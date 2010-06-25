/*
 */

#include "Minimax.hpp"
#include "Astar.hpp"

Reglas::NodoMinimax::NodoMinimax(Tablero* t)
{
    this->tablero = t;
}

bool Reglas::NodoMinimax::estaEn(std::list<NodoMinimax*>& lista)
{
    std::list<NodoMinimax*>::iterator it_lista;
    for(it_lista = lista.begin(); it_lista != lista.end(); it_lista++)
    {
        if( *(this->tablero) == *((*it_lista)->tablero) )
        {
            return true;
        }
    }
    return false;
}

bool Reglas::CompararNodoMinimaxMIN::operator ()(NodoMinimax* a, NodoMinimax* b)
{
    return a->val < b->val;
}

void Reglas::minimax(Reglas::NodoMinimax *currentTab, int currentDepth, int maxDepth,
            Reglas::TipoDeJugada tipoJug)
{
    AyudanteDeAgente ayudanteCurrent(currentTab->tablero);
    currentTab->idEnemigo = currentTab->idJugador == 1? 0: 1;

    //si llegamos al final de la recursion o hay un ganador, evaluamos el tab.
    if( (currentDepth >= maxDepth) || (ayudanteCurrent.hayGanador() ) )
    {
        currentTab->val = evaluate(currentTab);
        return;
    }

    //si no, generamos a sus hijos.
    std::list<Jugada> jugadas;
    if(tipoJug == MOVIMIENTO)
    {
        jugadas =  ayudanteCurrent.getMovimientosPosibles(currentTab->idJugador);
    }
    else
    {
        jugadas = ayudanteCurrent.getBarrerasPosibles(currentTab->idJugador);
        //discriminamos las jugadas.
        std::list<Reglas::Jugada>::iterator it_jugs;
        for(it_jugs = jugadas.begin(); it_jugs != jugadas.end(); it_jugs++)
        {
            int posJug_y = it_jugs->getPosicion().at(1);
            int posEnem_y = 
            ayudanteCurrent.getCelda((int)currentTab->idEnemigo).getPosicion().at(1);
            if( !( 
                 (posJug_y <= (posEnem_y + 2)) && (posJug_y >= (posEnem_y - 2))
                    ) )
            {
                jugadas.erase(it_jugs);
                it_jugs--;
            }
        }
    }

    while(jugadas.size() > 0)
    {
        Jugada jugada = jugadas.front();
        jugadas.pop_front();

        NodoMinimax *tabHijo = new NodoMinimax(new Tablero(currentTab->tablero));
        tabHijo->tipoDeJugadaInicial = currentTab->tipoDeJugadaInicial;
        if(jugada.getTipoDeJugada() == MOVIMIENTO)
        {
            tabHijo->tablero->moverJugador(currentTab->idJugador, jugada.getPosicion());
        }
        else
        {
            tabHijo->tablero->setBarrera(currentTab->idJugador, Barrera(jugada) );
        }

        //revisamos que el hijo no sea igual al padre
        if( (*(tabHijo->tablero) == *(currentTab->tablero)) ||
            tabHijo->estaEn(currentTab->hijos))
        {
            delete tabHijo->tablero;
            delete tabHijo;
            continue;
        }

        tabHijo->idJugador = currentTab->idEnemigo;
        tabHijo->tipo = !currentTab->tipo;
        tabHijo->jugada = jugada;
        minimax(tabHijo, currentDepth + 1,  maxDepth, MOVIMIENTO);

        currentTab->hijos.push_back(tabHijo);
    }

    //propagamos al padre los valores de los hijos.
    if(currentTab->tipo == MIN)
    {
        currentTab->val = ( *std::min_element(currentTab->hijos.begin(),
                                            currentTab->hijos.end(),
                                            CompararNodoMinimaxMIN()) )->val;
    }
    else
    {
        currentTab->val = ( *std::max_element(currentTab->hijos.begin(),
                                            currentTab->hijos.end(),
                                            CompararNodoMinimaxMIN()) )->val;
    }

    //limpiamos
    if(currentDepth > 0)
    {
        std::list<NodoMinimax*>::iterator it_hijos;
        for(it_hijos = currentTab->hijos.begin();
                                it_hijos != currentTab->hijos.end(); it_hijos++)
        {
            delete (*it_hijos)->tablero;
            delete ((*it_hijos));
        }
    }
}

Reglas::Jugada Reglas::minimax(Reglas::Tablero *currentTab, int idJugador,
                                int currentDepth, int maxDepth,
                                TipoDeJugada tipoJug)
{
    NodoMinimax *nodoCurrent = new NodoMinimax(currentTab);
    nodoCurrent->idJugador = idJugador;
    nodoCurrent->tipo = MAX;
    nodoCurrent->tipoDeJugadaInicial = tipoJug;
    minimax(nodoCurrent, currentDepth, maxDepth, tipoJug);

    Jugada j;

    std::list<NodoMinimax*>::iterator it_hijos;
    for(it_hijos = nodoCurrent->hijos.begin();
                            it_hijos != nodoCurrent->hijos.end(); it_hijos++)
    {
        if(nodoCurrent->val == (*it_hijos)->val )
        {
            j = (*it_hijos)->jugada;
        }
        delete (*it_hijos)->tablero;
        delete ((*it_hijos));
    }

    delete nodoCurrent;

    return j;
}

float Reglas::evaluate(NodoMinimax* nodo)
{
    std::vector< void* >* astar_jugador_path = astar(nodo->tablero, nodo->idJugador);
    std::vector< void* >* astar_enemigo_path = astar(nodo->tablero, nodo->idEnemigo);

    float astar_jugador_eval = astar_jugador_path->size();
    float astar_enemigo_eval = astar_enemigo_path->size();

    float w1 = 1.0f;
    float w2 = 1.0f;

    delete astar_jugador_path;
    delete astar_enemigo_path;
    
    if(nodo->tipoDeJugadaInicial == BARRERA)
    {
        w2 = 1.5f;
    }
    else if (nodo->tipoDeJugadaInicial == MOVIMIENTO)
    {
        w1 = 1.5f;
    }
    
    return
            ( w1 * (CELLS - astar_jugador_eval) )
           - ( w2 * (CELLS - astar_enemigo_eval) )
            ;
}