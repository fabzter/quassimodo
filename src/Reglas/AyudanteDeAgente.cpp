/*
 */

#include <stdint.h>
#include <map>

#include "AyudanteDeAgente.hpp"

Reglas::AyudanteDeAgente::AyudanteDeAgente(Tablero &t)
{
    this->init(t);
}

Reglas::AyudanteDeAgente::AyudanteDeAgente(Tablero *t)
{
    this->init(*t);
}

void Reglas::AyudanteDeAgente::init(Tablero& t)
{
    this->tablero = &t;
    this->juez = new Juez(t);
}

Reglas::AyudanteDeAgente::AyudanteDeAgente(const AyudanteDeAgente& orig)
{
    this->copiar(orig);
}

Reglas::AyudanteDeAgente::AyudanteDeAgente(const AyudanteDeAgente* orig)
{
    this->copiar(orig);
}

void Reglas::AyudanteDeAgente::copiar(const AyudanteDeAgente& orig)
{
    this->tablero = orig.tablero;
    this->juez = orig.juez;
}

Reglas::AyudanteDeAgente::~AyudanteDeAgente()
{
    delete this->juez;
}

std::list<Reglas::Jugada>
Reglas::AyudanteDeAgente::getMovimientosPosibles(int numJugador)
{
    std::list<Jugada> jugadas;
    const Celda &celda_actual = this->tablero->getCelda(numJugador);
    const std::vector<int> &pos_actual = celda_actual.getPosicion();
    
    for(int dir = (int)NORTE; dir <= (int)OESTE; dir++)
    {
        //tratamos de obtener la celda justo adelante en direccion dir
        try
        {
            Jugada j(this->tablero->getCelda(
                pos_actual.at(0) + this->getDeltaX((Direccion) dir),
                pos_actual.at(1) + this->getDeltaY((Direccion) dir)
                ) );
            
            this->agregarJugadaSelectivamente(numJugador, j, jugadas);
        }
        catch(std::out_of_range &e){/*ups!*/}
        //intentamos con el que está a la derecha de el anterior que sacamos...
        try
        {
            Jugada j(this->tablero->getCelda(
                pos_actual.at(0) + this->getDeltaX((Direccion) dir) +
                    this->getDeltaX((Direccion) (dir + 1) ),
                pos_actual.at(1) + this->getDeltaY((Direccion) dir) +
                    this->getDeltaX((Direccion) (dir + 1) )
                ) );

            this->agregarJugadaSelectivamente(numJugador, j, jugadas);
        }
        catch(std::out_of_range &e){/*ups!*/}
        //intentamos con que esta uno más adelante del primero que sacamos...
        try
        {
            Jugada j(this->tablero->getCelda(
                pos_actual.at(0) + this->getDeltaX((Direccion) dir)*2,
                pos_actual.at(1) + this->getDeltaY((Direccion) dir)*2
                ) );

            this->agregarJugadaSelectivamente(numJugador, j, jugadas);
        }
        catch(std::out_of_range &e){/*ups!*/}
    }

    return jugadas;
}

int Reglas::AyudanteDeAgente::getDeltaX(Direccion d)
{
    if(d == ESTE)
        return 1;
    if(d == OESTE)
        return -1;
    return 0;
}

int Reglas::AyudanteDeAgente::getDeltaY(Direccion d)
{
    if(d == NORTE)
        return 1;
    if(d == SUR)
        return -1;
    return 0;
}

void Reglas::AyudanteDeAgente::agregarJugadaSelectivamente(int numJugador,
                                                           Jugada &j,
                                                           std::list<Jugada> &js)
{
    try
    {
        this->juez->revisar_reglas(j, numJugador);
    }
    catch(ReglasRotas &e)
    {
        return;
    }
    js.push_back(j);
}

std::list<Reglas::Jugada>
Reglas::AyudanteDeAgente::getBarrerasPosibles(int numJugador)
{
    std::list<Jugada> jugadas;
    if(this->tablero->getJugador(numJugador).getBarrerasDisponibles() == 0)
    {
        return jugadas;
    }

    Jugada j;
    j.setTipoDeJugada(BARRERA);
    for(int y = 0; y < Tablero::size_y; y++)
    {
        for(int x = 0; x < Tablero::size_x; x++)
        {
            j.setPosicion(x, y);
            for(int dir = (int)NORTE; dir <= (int)ESTE; dir++)
            {
                j.setDireccion((Direccion)dir);
                try
                {
                    this->juez->revisar_reglas(j, numJugador);
                    jugadas.push_back(j);
                }
                catch(ReglasRotas &e){/*jiji*/}
            }
        }
    }
    return jugadas;
}

/*Metodos del A* */
std::pair<Reglas::Tablero*, Reglas::Jugada*>*  //pinche monstruosidad.
Reglas::AyudanteDeAgente::astar(int idJugador, int limit)
{
    //creamos el nodo de inicio
    int nums = 0;
    int start_g = 0;
    int start_h = this->heuristic(idJugador);
    Tablero *start_pos = this->tablero;
    int start_array[] = { start_g + start_h, start_h, nums++, start_g,
    (int)start_pos, true, true, NULL };
    std::vector<int> start(start_array, start_array + sizeof(start_array) /
                                                                sizeof(int) );

    //track all nodes so far.
    std::map<Tablero*, std::vector<int> > nodes;
    nodes.insert( std::pair<Tablero *, std::vector<int> >(start_pos, start) );

    //mantenemos un heap de los nodos.
    std::vector< std::vector<int> > heap;
    heap.push_back(start);
    std::push_heap(heap.begin(), heap.end(), acomodadorHeap);

    //Track the best path found so far.
    std::vector<int> best(start);

    while(heap.size() > 0)
    {
        //sacamos el siguiente nodo del heap
        std::vector<int> current = heap.at(0);
        std::pop_heap(heap.begin(), heap.end(), acomodadorHeap);
        AyudanteDeAgente ayudante_current((Tablero*)current.at(POS));
        //y lo marcamos como no abierto.
        current.at(OPEN) = false;

        //alcanzamos la meta?
        if(ayudante_current.goal(idJugador))
        {
            best = current;
        }

        //visitamos a los vecinos del nodo actual.
        std::list< std::pair<Tablero*, Jugada*> > neighbors_current =
                                        ayudante_current.neighbors(idJugador);

        std::list< std::pair<Tablero*, Jugada*> >::iterator vecino;
        for(vecino = neighbors_current.begin();
                                vecino != neighbors_current.end(); vecino++)
        {
            Tablero *neighbor_pos = vecino->first;
            int neighbor_g = current.at(G) +
                                ayudante_current.cost(vecino->first, idJugador);
            AyudanteDeAgente ayudante_neighbor(neighbor_pos);

            std::vector<int> neighbor;
            try
            {
                neighbor = nodes.at(neighbor_pos);
            }
            catch(std::out_of_range &e){}

            if(neighbor.size() == 0)
            {
                //limitamos la busqueda.
                if(nodes.size() >= limit)
                {
                    continue;
                }

                //encontramos un nuevo nodo
                int neighbor_h = ayudante_neighbor.heuristic(idJugador);

                int neighbor_array[] = { neighbor_g + neighbor_h, neighbor_h,
                                        nums++, neighbor_g, (int)neighbor_pos,
                                        true, true, current.at(POS) };
                std::vector<int> neighbor(neighbor_array, neighbor_array +
                                            sizeof(neighbor_array)/sizeof(int));

                nodes[neighbor_pos] = neighbor;

                heap.push_back(neighbor);
                std::push_heap(heap.begin(), heap.end(), acomodadorHeap);

                if(neighbor_h < best.at(H))
                {
                    //nos aproximamos a la meta
                    best = neighbor;
                }
            }
            else if(neighbor_g < neighbor.at(G))
            {
                //hemos encontrado el mejor camino al vecino
                if(neighbor.at(OPEN))
                {
                    //El vecino ya esta abierto. Encontrarlo y actualizarlo
                    //seria una operacion de complejidad linear.
                    //En lugar de eso lo marcamos como invalido y hacemos una
                    //copia actualizada de él.
                    neighbor.at(VALID) = false;
                    
                }
            }
        }

    }

}

std::list< std::pair<Reglas::Tablero*, Reglas::Jugada*> >
Reglas::AyudanteDeAgente::neighbors(int idJugador)
{
    std::list< std::pair<Tablero*, Jugada*> > vecinos;

    //para cada movimiento posible del jugador...
    std::list<Jugada>::iterator it;
    std::list<Jugada> movimientos =
                                this->getMovimientosPosibles(idJugador);

    for(it = movimientos.begin(); it != movimientos.end(); it++)
    {
        //creamos un tablero con dicho mivimiento
        Tablero *tab = new Tablero(this->tablero);
        tab->moverJugador(idJugador, it->getPosicion());

        //lo guardamos en la lista junto con su jugada
        vecinos.push_back(std::pair<Tablero*, Jugada*>(tab, new Jugada(*it) ) );
    }

    return vecinos;
}

bool Reglas::AyudanteDeAgente::goal(int idJugador)
{
    return (this->juez->hayGanador() >= 0) &&
            (this->juez->hayGanador() == idJugador);
}

int Reglas::AyudanteDeAgente::manhattan(const std::vector<int> &a,
                                                    const std::vector<int> &b)
{
    return abs( (b.at(0) - a.at(0)) * 1.2 ) + abs( b.at(1) - a.at(1) );
}

int Reglas::AyudanteDeAgente::cost(Reglas::Tablero* destino, int idJugador)
{
    return this->manhattan(this->tablero->getCelda(idJugador).getPosicion(),
                            destino->getCelda(idJugador).getPosicion());
}

int Reglas::AyudanteDeAgente::heuristic(int idJugador)
{
    std::vector<int> pos_1 = this->tablero->getCelda(idJugador).getPosicion();

    //iteramos todas las metas.
    int min = 50000;
    const std::vector<Celda> &metas = this->tablero->getMetas(idJugador);
    for(int i = 0; i < metas.size(); i++)
    {
        int val = this->manhattan(pos_1, metas.at(i).getPosicion());
        if(val < min)
            min = val;
    }
    return min;
}

