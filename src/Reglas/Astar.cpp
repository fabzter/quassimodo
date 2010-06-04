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

    return std::abs( (b.at(0) - a.at(0)) * 1.2 ) + std::abs(b.at(1) - a.at(1));
}

void Reglas::Mapa::AdjacentCost(void* state,
                                std::vector<micropather::StateCost>* adjacent)
{
    //creamos un tablero donde el jugador este en state.
    Celda* celda_actual = (Celda*)state;

    Tablero tab_copia(this->t);

    tab_copia.moverJugador(idJugador, celda_actual->getPosicion());

    AyudanteDeAgente ayudante_copia(tab_copia);

    //sacamos a todos sus vecinos desde state.
    std::list<Reglas::Jugada> jugs_mov =
                        ayudante_copia.getMovimientosPosibles(this->idJugador);

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

    std::cout << tab_copia << std::endl;
}

micropather::MicroPather* Reglas::Mapa::getPather()
{
    return this->pather;
}

Reglas::Jugada astar(Reglas::Tablero *t, int idJugador)
{
    Reglas::Mapa mapa(t, idJugador);

    std::vector< void* > path;
    float totalCost;

    int result = mapa.getPather()->Solve( (void*)&(t->getCelda(idJugador)),
                        (void*)&(t->getMetas(idJugador).at(1)), &path,
                        &totalCost );

    if(result == micropather::MicroPather::SOLVED)
    {
        std::cout << "Llego aqui" << std::endl;
        return Reglas::Jugada( *((Reglas::Celda*)(path.at(1))) );
    }
}