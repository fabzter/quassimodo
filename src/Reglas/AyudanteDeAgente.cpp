#include <stdint.h>
#include <map>
#include <vector>

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
    this->copiar(*orig);
}

void Reglas::AyudanteDeAgente::copiar(const AyudanteDeAgente& orig)
{
    this->tablero = orig.tablero;
    this->juez = new Juez(*this->tablero);
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
    
    for(int dir = (int)NORTE; dir <= (int)OESTE; dir++)
    {
        //tratamos de obtener el hijo en dirección d, con salto.
        try
        {
            Jugada j(celda_actual.getHijo((Direccion)dir, true));
            
            this->agregarJugadaSelectivamente(numJugador, j, jugadas);
        }
        catch(SinHijo &e)
        {
            continue;
        }
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
                catch(ReglasRotas &e)
                {
                    continue;
                }
            }
        }
    }
    return jugadas;
}

void Reglas::AyudanteDeAgente::revisarReglas(Reglas::Jugada &j, int idJugador)
{
    this->juez->revisar_reglas(j, idJugador);
}

const Reglas::Celda& Reglas::AyudanteDeAgente::getCelda(int idJugador)
{
    return this->tablero->getCelda(idJugador);
}

bool Reglas::AyudanteDeAgente::hayGanador()
{
    return this->juez->hayGanador() > 0;
}

Reglas::Juez& Reglas::AyudanteDeAgente::getJuez()
{
    return *this->juez;
}
