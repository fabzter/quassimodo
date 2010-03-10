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
    this->setPosicion(orig.posicion.at(0), orig.posicion.at(1));
    this->tipo = orig.tipo;
}

Reglas::Jugada::~Jugada()
{
}

void Reglas::Jugada::setPosicion(int x, int y)
{
    if(this->posicion.size() != 2){
        this->posicion.resize(2);
    }
    
    this->posicion[0] = x;
    this->posicion[1] = y;
}

void Reglas::Jugada::setPosicion(const std::vector<int>& pos)
{
    if(pos.size() != 2)
    {
        throw ParametrosMalos();//TODO: info del error.
    }
    this->setPosicion(pos.at(0), pos.at(1));
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
