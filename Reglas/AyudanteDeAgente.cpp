/*
 */

#include "AyudanteDeAgente.hpp"

Reglas::AyudanteDeAgente::AyudanteDeAgente(Tablero &t)
{
    this->tablero = &t;
    this->juez = new Juez(t);
}

Reglas::AyudanteDeAgente::AyudanteDeAgente(const AyudanteDeAgente& orig)
{
    this->tablero = orig.tablero;
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
    int deltaX, deltaY;
    
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