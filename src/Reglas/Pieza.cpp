#include <vector>
#include <iosfwd>

#include "Pieza.hpp"
#include "Jugada.hpp"

Reglas::Pieza::Pieza()
{
  this->esta_colocado = false;
  
  this->posicion.resize(2);
  this->posicion.at(0) = -1;
  this->posicion.at(1) = -1;
}

Reglas::Pieza::Pieza(Jugada &j)
{
    this->esta_colocado = false;
    this->posicion = j.getPosicion();
}

Reglas::Pieza::Pieza(const Pieza& orig)
{
    this->posicion.resize(2);
    this->posicion = orig.posicion;
    this->esta_colocado = orig.esta_colocado;
}

Reglas::Pieza::~Pieza()
{
}

bool Reglas::Pieza::estaColocado() const
{
    return this->esta_colocado;
}

void Reglas::Pieza::colocar(int x, int y)
{
    std::vector<int> pos;
    pos.resize(2);
    pos.at(0) = x;
    pos.at(1) = y;

    this->colocar(pos);
}
void Reglas::Pieza::colocar(const std::vector<int> &pos)
{
    std::ostringstream strs;

    if(pos.size() != 2)
        throw ParametrosMalos(); //TODO: Agregar info del error.
    if(this->esta_colocado)
    {
        strs << "Se intento colocar una Pieza que ya estaba colocada en: (" <<
                pos.at(0) << ',' << pos.at(1) << ").";
        throw PiezaYaColocada(strs.str());
    }
    if(pos.at(0) >= this->size_x || pos.at(1) >= this->size_y )
    {
        strs << "Se intento colocar la Pieza en una posicion invalida: (" <<
                pos.at(0) << ',' << pos.at(1) << ").";
        throw PiezaFueraDelTablero(strs.str());
    }
        

    this->posicion = pos;
    this->esta_colocado = true;
}

const std::vector<int> &Reglas::Pieza::getPosicion() const
{
    return this->posicion;
}

bool Reglas::Pieza::operator ==(const Pieza& otro) const
{
    return this->posicion.at(0) == otro.posicion.at(0) &&
            this->posicion.at(1) == otro.posicion.at(1);
}

bool Reglas::Pieza::operator !=(const Pieza& otro) const
{
    return !(*this == otro);
}