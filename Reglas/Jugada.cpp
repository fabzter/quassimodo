/**
 */

#include "Jugada.hpp"

Reglas::Jugada::Jugada()
{
  this->direccion = NORTE;
  this->setPosicion(0,0);
  this->tipo = MOVIMIENTO;
}

Reglas::Jugada::Jugada(const Jugada& orig)
{
    this->direccion = orig.direccion;
    this->setPosicion(orig.posicion);
    this->tipo = orig.tipo;
}

Reglas::Jugada::~Jugada()
{
}

void Reglas::Jugada::setPosicion(int x, int y)
{
    this->posicion.clear();
    
    this->posicion.push_back(x);
    this->posicion.push_back(y);
}

void Reglas::Jugada::setPosicion(const std::vector<int>& pos)
{
    this->posicion = pos;
}

const std::vector<int>& Reglas::Jugada::getPosicion() const
{
    return this->posicion;
}
void Reglas::Jugada::setDireccion(Direccion d)
{

     this->direccion = d;
}
Reglas::Direccion Reglas::Jugada::getDireccion() const
{
     return this->direccion;
}
void Reglas::Jugada::setTipoDeJugada(TipoDeJugada t)
{
      this->tipo = t;
 }
Reglas::TipoDeJugada Reglas::Jugada::getTipoDeJugada() const
{
      return this->tipo;
}
