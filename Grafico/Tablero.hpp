
#ifndef _TABLERO_HPP
#define	_TABLERO_HPP
#include "Pieza.hpp"

namespace Grafico{
/**
 * Contiene el tablero, tanto de la parte grafica como el de las reglas.
 */
class Tablero: public Pieza {
public:
    Tablero();
    Tablero(const Tablero& orig);
    virtual ~Tablero();
private:

};
}

#endif	/* _TABLERO_HPP */

