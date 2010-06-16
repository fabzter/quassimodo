/**
 */

#include "Barrera.hpp"
#include "Jugada.hpp"

Reglas::Barrera::Barrera() : Pieza()
{
    this->direccion_actual = NORTE;
    this->punto_medio.resize(2);
    this->punto_final.resize(2);
    for(int i = 0; i < this->posicion.size(); i++)
    {
        this->punto_medio.at(i) = ( this->punto_final.at(i) = -1 );
    }
}

Reglas::Barrera::Barrera(Jugada &j) : Pieza(j)
{
    this->init_jugada(j);
}

Reglas::Barrera::Barrera(Jugada* j): Pieza(*j)
{
    this->init_jugada(*j);
}

void Reglas::Barrera::init_jugada(Jugada& j)
{
    this->direccion_actual = j.getDireccion();
    this->punto_medio.resize(2);
    this->punto_final.resize(2);

    int deltaX = 0, deltaY = 0;
    if(j.getDireccion() == NORTE)
    {
        deltaY = 1;
    }
    if(j.getDireccion() == ESTE)
    {
        deltaX = 1;
    }
    this->punto_medio.at(0) = j.getPosicion().at(0) + deltaX;
    this->punto_medio.at(1) = j.getPosicion().at(1) + deltaY;

    this->punto_final.at(0) = j.getPosicion().at(0) + (2*deltaX);
    this->punto_final.at(1) = j.getPosicion().at(1) + (2*deltaY);

    this->esta_colocado = true;
}

Reglas::Barrera::Barrera(const Barrera& orig) : Pieza(orig)
{
    this->direccion_actual = orig.direccion_actual;
    this->punto_medio.resize(2);
    this->punto_final.resize(2);

    this->punto_medio = orig.punto_medio;
    this->punto_final = orig.punto_final;
}

Reglas::Barrera::~Barrera()
{
}

void Reglas::Barrera::colocar(const std::vector<int>& pos, Direccion d)
{
    if(!(d == NORTE || d == ESTE))
        throw ParametrosMalos(); //TODO: agregar info del error.
    
    Pieza::colocar(pos);
    this->direccion_actual = d;

    this->punto_final = this->posicion;
    this->punto_medio = this->posicion;

    //Calculamos los puntos que toca...
    if(d == ESTE)
    {
        this->punto_medio.at(0)++;
        this->punto_final.at(0)+=2;
    }
    else
    {
        this->punto_medio.at(1)++;
        this->punto_final.at(1)+=2;
    }
}

void Reglas::Barrera::colocar(int x, int y, Direccion d)
{
    std::vector<int> v;
    v.push_back(x);
    v.push_back(y);
    this->colocar(v, d);
}

Reglas::Direccion Reglas::Barrera::getDireccion() const
{
    if(!this->esta_colocado)
        throw PiezaNoColocada(); //TODO: agregar info del error.
    
    return this->direccion_actual;
}

const std::vector<int> &Reglas::Barrera::getPuntoMedio() const
{
    if(!this->esta_colocado)
        throw PiezaNoColocada(); //TODO: agregar info del error.

    return this->punto_medio;
}

const std::vector<int> &Reglas::Barrera::getPunta() const
{
    if(!this->esta_colocado)
        throw PiezaNoColocada(); //TODO: agregar info del error.

    return this->punto_final;
}

bool Reglas::Barrera::operator==(const Reglas::Barrera &otro) const
{
    return Pieza::operator ==(otro) &&
            otro.getDireccion() == this->getDireccion();
}

bool Reglas::Barrera::operator!=(const Reglas::Barrera &otro) const
{
    return !(*this == otro);
}