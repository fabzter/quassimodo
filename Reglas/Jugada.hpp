/**
 */

#ifndef _JUGADA_HPP
#define	_JUGADA_HPP
#include<vector>

#include "Enums.hpp"
#include "Excepciones.hpp"

/**
 * La clase Jugada contiene la información sobre el movimiento que se desea realizar.
 * Cuando el Jugador quiera realizar un movimiento, rellena un objeto Jugada con
 * la información pertinente y lo regresa.
 */
namespace Reglas
{
class Jugada {
public:
    /**
     *Constructor por default. Deja la posicion en (0,0), TipoDeJugada en MOVIMIENTO
     * y Direccion en NORTE.
     */
    Jugada();
    Jugada(const Jugada& orig);
    virtual ~Jugada();

    /**
     *Especifica la posicion de la Jugada.
     * @param x un entero que representa la posición horizontal.
     * @param y un entero que representa la posición vertical.
     */
    void setPosicion(int x, int y);

    /**
     *Especifica la posicion de la Jugada.
     * @param pos un vector de enteros (x,y) que represnta la posición deseada.
     * @throws ParametrosMalos si pos no tiene dos elementos.
     */
    void setPosicion(const std::vector<int>& pos);

    /**
     *Regresa la posicion de la Jugada.
     * @return Una referencia al vector interno de la Jugada.
     */
    const std::vector<int>& getPosicion() const;

    /**
     *Especifica el tipo de Jugada.
     * @param t un valor del enum TipoDeJugada.
     */
    void setTipoDeJugada(TipoDeJugada t);

    /**
     *Regresa el tipo de Jugada.
     * @return un valor del enum TipoDeJugada.
     */
    TipoDeJugada getTipoDeJugada() const;

    /**
     *Especifica la direccion del movimiento o de la barrera a colocar
     * @param d un valor del enum Direccion
     */
    void setDireccion(Direccion d);
    /**
    *Regresa la direccion actual de la jugada
    * @return un valor del enum Direccion
    */
    Direccion getDireccion() const;

private:
    /**
     * Un vector de enteros que representa la posición a la que la que se quiere
     * mover o colocar la barrera. Debe tener un tamaño dos. Por default = (0,0)
     */
    std::vector<int> posicion;

    /**
     * Indica el tipo de Jugada, por default = TipoDeJugada.MOVIMIENTO
     */
    TipoDeJugada tipo;

    /**
     * Indica la dirección del movimiento o de la barrera. Por default =
     * Direccion.NORTE. Para barreras, solo se acepta NORTE y ESTE.
     */
    Direccion direccion;

};
}
#endif	/* _JUGADA_HPP */

