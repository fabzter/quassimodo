/* 
 * File:   Astar.cpp
 * Author: fabrizio
 * 
 * Created on 2 de junio de 2010, 11:28 AM
 */

#include "Astar.hpp"
#include "AyudanteDeAgente.hpp"

bool Reglas::AcomodadorHeap::operator ()
(std::vector<int>* first, std::vector<int>* second)
{
    return first->at(0) > second->at(0);
}

Reglas::TableroEnvoltorio::TableroEnvoltorio(Reglas::Tablero* t)
{
    this->t = t;
}

Reglas::Tablero* Reglas::TableroEnvoltorio::getTablero()
{
    return this->t;
}

bool Reglas::TableroEnvoltorio::operator == (const TableroEnvoltorio& o) const
{
    return *(this->t) == *(o.t);
}

bool Reglas::TableroEnvoltorio::operator != (const TableroEnvoltorio& o) const
{
    return *(this->t) != *(o.t);
}

bool Reglas::AcomodadorMap::operator()
(const TableroEnvoltorio& first, const TableroEnvoltorio& second)
{
    return first < second;
}


bool operator<(const Reglas::TableroEnvoltorio &a,
        const Reglas::TableroEnvoltorio &b)
{
    Reglas::TableroEnvoltorio *a_ = (Reglas::TableroEnvoltorio*)&a;
    Reglas::TableroEnvoltorio *b_ = (Reglas::TableroEnvoltorio*)&b;
    return a_->getTablero() < b_->getTablero();
}

std::list< std::pair<Reglas::TableroEnvoltorio, Reglas::Jugada*> >
Reglas::neighbors(Tablero* tableroActual, int idJugador)
{
    AyudanteDeAgente ayudante(tableroActual);

    std::list< std::pair<TableroEnvoltorio, Jugada*> > vecinos;

    //para cada movimiento posible del jugador...
    std::list<Jugada>::iterator it;
    std::list<Jugada> movimientos =
                                ayudante.getMovimientosPosibles(idJugador);

    for(it = movimientos.begin(); it != movimientos.end(); it++)
    {
        //creamos un tablero con dicho mivimiento
        Tablero *tab = new Tablero(tableroActual);
        tab->moverJugador(idJugador, it->getPosicion());

        //lo guardamos en la lista junto con su jugada
        vecinos.push_back(
        std::pair<TableroEnvoltorio, Jugada*>(TableroEnvoltorio(tab),
                                                            new Jugada(*it) ) );
    }

    return vecinos;
}

bool Reglas::goal(Tablero* tableroActual, int idJugador)
{
    AyudanteDeAgente ayudante(tableroActual);

    int hayGanador = ayudante.getJuez().hayGanador();

    return (hayGanador >= 0) && (hayGanador == idJugador);
}

int Reglas::manhattan(const std::vector<int>& a, const std::vector<int>& b)
{
    return std::abs( (b.at(0) - a.at(0)) * 1.2 ) + std::abs( b.at(1) - a.at(1) );
}

int Reglas::cost(Tablero* actual, Tablero* destino, int idJugador)
{
    return manhattan(actual->getCelda(idJugador).getPosicion(),
                            destino->getCelda(idJugador).getPosicion());
}

int Reglas::heuristic(Tablero* tableroActual, int idJugador)
{
    const std::vector<int> &pos_1 = tableroActual->getCelda(idJugador).getPosicion();

    //iteramos todas las metas.
    int min = 50000;
    const std::vector<Celda> &metas = tableroActual->getMetas(idJugador);
    for(int i = 0; i < metas.size(); i++)
    {
        int val = manhattan(pos_1, metas.at(i).getPosicion());
        if(val < min)
            min = val;
    }
    return min;
}

std::pair<Reglas::Tablero*, Reglas::Jugada*>*
Reglas::astar(Reglas::Tablero *start_pos, int idJugador, int limit)
{
    using namespace Reglas;
    //creamos el nodo de inicio
    int nums = 0;
    int start_g = 0;
    int start_h = heuristic(start_pos, idJugador);
    int start_array[] = { start_g + start_h, start_h, nums++, start_g,
    (int)start_pos, true, true, NULL };
    std::vector<int> *start = new std::vector<int>(start_array,
                                            start_array + sizeof(start_array) /
                                                                sizeof(int) );

    //track all nodes so far.
    std::map<TableroEnvoltorio, std::vector<int>*, AcomodadorMap> nodes;
    nodes.insert( std::pair<TableroEnvoltorio, std::vector<int>* >
                                        (TableroEnvoltorio(start_pos), start) );

    //mantenemos un heap de los nodos.
    std::vector< std::vector<int>* > heap;
    heap.push_back(start);
    std::push_heap(heap.begin(), heap.end(), AcomodadorHeap());

    //Track the best path found so far.
    std::vector<int> *best = start;

    while(heap.size() > 0)
    {
        //sacamos el siguiente nodo del heap
        std::vector<int> *current = heap.at(0);
        std::pop_heap(heap.begin(), heap.end(), AcomodadorHeap());

        //y lo marcamos como no abierto.
        current->at(OPEN) = false;

        //alcanzamos la meta?
        if(goal((Tablero*)current->at(POS), idJugador))
        {
            best = current;
            break;
        }

        //visitamos a los vecinos del nodo actual.
        std::list< std::pair<TableroEnvoltorio, Jugada*> > neighbors_current =
                                neighbors((Tablero*)current->at(POS), idJugador);

        std::list< std::pair<TableroEnvoltorio, Jugada*> >::iterator vecino;
        for(vecino = neighbors_current.begin();
                                vecino != neighbors_current.end(); vecino++)
        {
            Tablero *neighbor_pos = vecino->first.getTablero();
            int neighbor_g = current->at(G) +
                    cost((Tablero*)current->at(POS), neighbor_pos, idJugador);

            std::vector<int> *neighbor = NULL;
            try
            {
                neighbor = nodes.at(TableroEnvoltorio(neighbor_pos));
            }
            catch(std::out_of_range &e){}

            if(neighbor == NULL)
            {
                //limitamos la busqueda.
                if(nodes.size() >= limit)
                {
                    continue;
                }

                //encontramos un nuevo nodo
                int neighbor_h = heuristic(neighbor_pos, idJugador);

                int neighbor_array[] = { neighbor_g + neighbor_h, neighbor_h,
                                        nums++, neighbor_g, (int)neighbor_pos,
                                        true, true, current->at(POS) };
                neighbor = new std::vector<int>(neighbor_array,
                        neighbor_array + sizeof(neighbor_array)/sizeof(int));

                nodes[TableroEnvoltorio(neighbor_pos)] = neighbor;

                heap.push_back(neighbor);
                std::push_heap(heap.begin(), heap.end(), AcomodadorHeap());

                if(neighbor_h < best->at(H))
                {
                    //nos aproximamos a la meta
                    best = neighbor;
                }
            }
            else if(neighbor_g < neighbor->at(G))
            {
                //hemos encontrado el mejor camino al vecino
                if(neighbor->at(OPEN))
                {
                    //El vecino ya esta abierto. Encontrarlo y actualizarlo
                    //seria una operacion de complejidad linear.
                    //En lugar de eso lo marcamos como invalido y hacemos una
                    //copia actualizada de él.
                    neighbor->at(VALID) = false;
                    std::vector<int> *neighbor_ = new std::vector<int>(*neighbor);
                    neighbor = neighbor_;
                    nodes[TableroEnvoltorio(neighbor_pos)] = neighbor;
                    neighbor->at(F) = neighbor_g + neighbor->at(H);
                    neighbor->at(NUM) = nums++;
                    neighbor->at(G) = neighbor_g;
                    neighbor->at(VALID) = true;
                    neighbor->at(PARENT) = current->at(POS);

                    heap.push_back(neighbor);
                    std::push_heap(heap.begin(), heap.end(), AcomodadorHeap());
                }
                else
                {
                    //re-abrimos al vecino.
                    neighbor->at(F) = neighbor_g + neighbor->at(H);
                    neighbor->at(G) = neighbor_g;
                    neighbor->at(PARENT) = current->at(POS);
                    neighbor->at(OPEN) = true;

                    heap.push_back(neighbor);
                    std::push_heap(heap.begin(), heap.end(), AcomodadorHeap());
                }
            }
        }
        //botamos a los nodos invalidos al inicio del heap.
        while( (heap.size() > 0) && !heap.at(0)->at(VALID) )
        {
            std::pop_heap(heap.begin(), heap.end(), AcomodadorHeap());
        }
    }
    //buscamos al primer vecino de la posicion actual
    std::vector<int> *current = best;
    std::vector<int> *previous = best;
    while((Tablero*)current->at(PARENT) != NULL)
    {
        previous = current;
        current = nodes.at( TableroEnvoltorio( ((Tablero*)current->at(PARENT)) ) );
    }

     //limpiamos
    std::map<TableroEnvoltorio, std::vector<int>* >::iterator it;
    for(it = nodes.begin(); it != nodes.end(); it++)
    {
        if(it->second != previous)
        {
            delete ((TableroEnvoltorio*)&(it->first))->getTablero();
            delete it->second;
        }
    }

    //obtenemos la jugada...
    Tablero *t = (Tablero*)previous->at(POS);
    Jugada *j = new Jugada(t->getCelda(idJugador));

    return new std::pair<Reglas::Tablero*, Reglas::Jugada*>(t, j);
}