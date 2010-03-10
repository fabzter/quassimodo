/**
 */

#ifndef _BARRERA_HPP
#define	_BARRERA_HPP
#include <vector>
#include "Excepciones.hpp"
#include "Enums.hpp"
#include "Pieza.hpp"

namespace Reglas{

/**
 * //FIXME: documentacion!!!
 * Clase derivada de Pieza. Representa las barreras
 */
class Barrera: public Pieza {
public:
    /**
     * Constructor default. Le da una direcccion Norte, lo pone no colocado, y
     * todos sus puntos en (-1, -1).
     * @return
     */
    Barrera();
    Barrera(const Barrera& orig);
    virtual ~Barrera();

    /**
     * Este es el método para colocar una barrera, se usa en lugar del de la
     * clase Pieza pues necesitamos darle también una Direccion a la Barrera.
     * @param pos es un vector de dos enteros (x, y) que  representa la posicion
     * inicial de la Barrera.
     * @param d un valor de enum Direccion que indica la direccion en que será
     * colocada la Barrera.
     * @throws ParametrosMalos.
     */
    void colocar(const std::vector<int> &pos, Direccion d);

    /**
     *Este es el método para colocar una barrera, se usa en lugar del de la
     * clase Pieza pues necesitamos darle también una Direccion a la Barrera.
     * @param x es un entero que  representa la posicion inicial en x de la
     * Barrera.
     * @param y es un entero que  representa la posicion inicial en y de la
     * Barrera.
     * @param d un valor de enum Direccion que indica la direccion en que será
     * colocada la Barrera.
     * @throws ParametrosMalos.
     */
    void colocar(int x, int y, Direccion d);

    /**
     * Nos indica la direccion actual de la Barrera.
     * @return un valor de el enum Direccion, ya sea NORTE o ESTE.
     * @throws PiezaNoColocada.
     */
    Direccion getDireccion() const;
    
    /**
     * Nos indica donde está colocado el punto medio de la barrera.
     * @return un vector de enteros (x, y).
     * @throws PiezaNoColocada.
     */
    const std::vector<int> &getPuntoMedio() const;

    /**
     * Nos indica donde está colocado el punto final de la barrera.
     * @return un vector de enteros (x, y).
     * @throws PiezaNoColocada.
     */
    const std::vector<int> &getPunta() const;

    /**
     *Este operador == sobrecargado nos da la facilidad de comparar si dos objetos
     * Barrera representan a la misma Barrera (es decir, están en la misma 
     * posición. y su punto medio es el mismo)
     * @return true si los objetos Barrera comparados representan a la misma,
     * false de lo contrario
     */
    bool operator==(const Barrera &otro) const;

    /**
     *Similar al operador ==, pero negado.
     * @return true si los Objetos Barrera no representan a la misma Barrera,
     * true de lo contrario.
     */
    bool operator!=(const Barrera &otro) const;

private:
    /**
     * La direccion en la que esta dirigida la barrera. Para este caso particular
     * solo puede ser ESTE o NORTE.
     */
    Direccion direccion_actual;

    /**
     * Un vector de dos enteros (x, y) que nos indica donde esta colocada la
     * parte media de la Barrera.
     */
    std::vector<int> punto_medio;
    /**
     * Un vector de dos enteros (x, y) que nos indica donde esta colocada la
     * punta de la Barrera.
     */
    std::vector<int> punto_final;

};
}
#endif	/* _BARRERA_HPP */

