/**
 */

#ifndef _PIEZA_HPP
#define	_PIEZA_HPP
#include <vector>
#include <sstream>
#include "Excepciones.hpp"

namespace Reglas{
/**
 * Ésta es una clase base de la que deben heredar todas las clases que quieran
 * ser piezas colocables en el Tablero.
 */
class Pieza {
public:
    /**
     * Constructor por default de Pieza. Inicializa a esta_colocado en false y a
     * la posicion en (-1, -1).
     */
    Pieza();
    Pieza(const Pieza& orig);
    virtual ~Pieza();

    /**
     *Este método nos indica si la Pieza ha sido ya colocado en el Tablero.
     * @return true si la Pieza ya fue colocado en el Tablero, false de no ser así.
     */
    virtual bool estaColocado() const;

    /**
     *Coloca la Pieza que NO ha sido colocada en el Tablero en su posicion inicial
     * @param x es un entero que  representa la posicion horizontal en que se
     * quiere colocar la pieza.
     * @param y es un entero que  representa la posicion vertical en que se
     * quiere colocar la pieza.
     * @throws PiezaYaColocada si se trata de colocar una Pieza ya colocada.
     * @htrows PiezaFueraDelTablero si se trata de colocar una Pieza en una
     * posición que no existe en el Tablero.
     */
    virtual void colocar(int x, int y);

    /**
     *Coloca la Pieza que NO ha sido colocada en el Tablero en su posicion inicial
     * @param pos es un vector de dos enteros (x, y) que  representa la posicion
     * actual de la celda.
     * @throws PiezaYaColocada si se trata de colocar una pieza ya colocada.
     */
    virtual void colocar(const std::vector<int> &pos);

    /**
     * Nos indica la posicion actual en la que se localiza la Pieza.
     * @return un vector de dos enteros (x, y) que nos muestra la posición de la
     * pieza sobre el Tablero.
     * @throws PiezaNoColocada si se intenta llamar antes de colocar la Pieza.
     */
    virtual const std::vector<int> &getPosicion() const;

    const static int size_x = 9, size_y = 9;

    /**
     *Este operador == sobrecargado nos da la facilidad de comparar si dos objetos
     * Pieza representan a la misma Pieza (es decir, están en la misma posición.)
     * @return true si los objetos Pieza comparados representan a la misma,
     * false de lo contrario
     */
    virtual bool operator==(const Pieza &otro) const;

    /**
     *Similar al operador ==, pero negado.
     * @return true si los Objetos Celda no representan a la misma celda, true de lo contrario.
     */
    virtual bool operator!=(const Pieza &otro) const;

protected:
    /**
     * Bandera que nos indica si ya ha sido colocado en el Tablero o no. Se debe
     * construir con false.
     */
    bool esta_colocado;

    /**
     *Posicion sobre la que se localiza a la Pieza actualmente. Debe construirse
     * con (-1, -1), y posteriormente será actualizada con colocar.
     */
    std::vector<int> posicion;
private:

};
}
#endif	/* _PIEZA_HPP */

