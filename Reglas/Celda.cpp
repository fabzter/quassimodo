/**
 */

#include "Celda.hpp"

Reglas::Celda::Celda() : Pieza()
{
    this->init_hijos();
    this->bloquear(false);
}

Reglas::Celda::Celda(const Celda& orig) : Pieza(orig)
{
    this->init_hijos();
    this->esta_libre = orig.esta_libre;
    this->esta_colocado = orig.esta_colocado;
    this->hijos = orig.hijos;
}

Reglas::Celda::~Celda()
{
}

bool Reglas::Celda::estaLibre() const
{
    return this->esta_libre;
}

void Reglas::Celda::bloquear(bool bloqueo)
{
    this->esta_libre = !bloqueo;
}

void Reglas::Celda::setHijo(Reglas::Direccion d, const Reglas::Celda &c)
{
    this->hijos[d] = (Celda*)&c;
}

Reglas::Celda& Reglas::Celda::getHijo(Direccion d) const
{
    return *this->hijos[d];
}

/**
 *FIXME: aqui comparamos direcciones de apuntadores, esto no debería ser así...
 * o no esoy seguro :(
 */
bool Reglas::Celda::tieneHijo(const Reglas::Celda& hijo) const
{
    for(int i = 0; i < this->hijos.size(); i++)
    {
        if( this->hijos.at(i) !=NULL && (this->hijos.at(i)) == &hijo )
        {
            return true;
        }
    }
    return false;
}

void Reglas::Celda::bloquearDireccion(Reglas::Direccion d)
{
    if(this->hijos[d] != NULL)
    {
        //bloqueamos la direccion opuesta de la celda hijo en direcion d
        int dir = (int)d;
        if(d == NORTE || d == ESTE)
        {
            dir+=2;
        }
        else
        {
            dir-=2;
        }
        this->hijos[d]->hijos[(Direccion)dir] = NULL;

        //bloqueamos la direccion de la celda dada
        this->hijos[d] = NULL;
    }
}

void Reglas::Celda::desbloquearDireccion(Direccion d, Celda &celdaHijo)
{
    if(&this->getHijo(d) == NULL)
    {
        this->setHijo(d, celdaHijo);

        //ahora desbloqueamos el lado contrario del hijo
        int dir = (int)d;
        if(d == NORTE || d == ESTE)
            dir+=2;
        else
            dir-=2;
        this->hijos.at(d)->setHijo((Direccion)dir, *this);
    }
}

bool Reglas::Celda::estaLibreDireccion(Reglas::Direccion d) const
{
    return this->hijos[d] != NULL;
}


void Reglas::Celda::init_hijos()
{
    //reservamos el espacio para los 4 hijos de cada nodo.
    this->hijos.reserve(this->num_hijos);
    this->hijos.resize(this->num_hijos);

    //ponemos a cada hijo como NULL.
    for(unsigned int i = 0; i < this->hijos.size(); i++){
        this->hijos.at(i) = NULL;
    }
}

std::ostream& operator<< (std::ostream &out, const Reglas::Celda &celda)
{
    Reglas::Celda* c = (Reglas::Celda*)&celda;
    std::vector<int> pos( c->getPosicion() );

    out << '(' << pos.at(0) << ", " << pos.at(1) << ") " << c->estaLibre() <<
            "\n\t" << c->estaLibreDireccion(Reglas::NORTE) << '\n' <<
            c->estaLibreDireccion(Reglas::OESTE) << "\t\t" << 
            c->estaLibreDireccion(Reglas::ESTE) << '\n' <<
            '\t' << c->estaLibreDireccion(Reglas::SUR);

    return out;
}