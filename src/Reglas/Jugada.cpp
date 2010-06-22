/**
 */

#include "Jugada.hpp"
#include "Pieza.hpp"
#include "Barrera.hpp"
#include "Celda.hpp"
Reglas::Jugada::Jugada()
{
  this->direccion = NORTE;
  this->setPosicion(0,0);
  this->tipo = MOVIMIENTO;
}

Reglas::Jugada::Jugada(const Celda &c)
{
    this->init_from_celda(c);
}

Reglas::Jugada::Jugada(const Celda* c)
{
    this->init_from_celda(*c);
}

void Reglas::Jugada::init_from_celda(const Celda &orig)
{
    this->direccion = NORTE;
    this->posicion = orig.getPosicion();
    this->tipo = MOVIMIENTO;
}

Reglas::Jugada::Jugada(const Barrera &b)
{
    this->direccion = b.getDireccion();
    this->posicion = b.getPosicion();
    this->tipo = BARRERA;
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

bool Reglas::Jugada::operator==(const Reglas::Jugada &otro) const
{
    if(otro.tipo != this->tipo)
        return false;

    std::vector<int>::iterator res;
    if ( std::search(this->posicion.begin(), this->posicion.end(),
                otro.posicion.begin(), otro.posicion.end(),
                std::equal_to<int>()) == this->posicion.end())
        return false;

    if( (otro.tipo == MOVIMIENTO) && (otro.direccion != this->direccion) )
        return false;

    return true;
}

bool Reglas::Jugada::operator!=(const Reglas::Jugada &otro) const
{
    return !(otro == (*this));
}

Reglas::Jugada& Reglas::Jugada::operator=(const Reglas::Jugada& rhs)
{
    this->direccion = rhs.direccion;
    this->setPosicion(rhs.posicion);
    this->tipo = rhs.tipo;

    return *this;
}