/**
 */

#ifndef _CELDA_HPP
#define	_CELDA_HPP
#include <iostream>
#include <vector>
#include "Enums.hpp"
#include "Excepciones.hpp"
#include "Pieza.hpp"

/**
 * En el namespace Reglas, se contienen las clases pertenecientes al manejo de
 * el tablero, los jugadores, los peones, las barreras, las reglas del juego, y
 * las excepciones arrojadas por sus métodos.
 */
namespace Reglas{
/**
 * La clase Celda contiene la información y los métodos para conocer el estado
 * de cada una de las Celdas contenidas en el Tablero.
 */
class Celda : public Pieza{
public:
    /**
     * Este es el constructor default de la clase Celda. Le da una posición de
     * -1 en ambos ejes, pone como bloqueados a sus lados. Y la pone
     * como no colocada, y como libre.
     */
    Celda();
    /**
     * Construye la Celda a partir de los datos de una Jugada.
     * @param j una referencia a la Jugada desde la cual se quiere construir
     * una Celda.
     */
    Celda(Jugada &j);
    Celda(const Celda& orig);
    virtual ~Celda();


    /**
     *Este método nos indica si la celda está ocupada por un Jugador o libre.
     * @return true si la celda se encuentra actualmente libre,
     * y false de no ser así
     */
    bool estaLibre() const;

    /**
     * Este método libera o bloquea la celda.
     * @param bloqueo si es true, la celda se bloquea efectivamente.
     */
    void bloquear(bool bloqueo = true);

    /**
     *Asigna un hijo a la celda. Esto significa que la Direccion d queda libre.
     * @param d es la dirección en la que se va a colocar al hijo.
     * @param c un apuntador a la Celda hijo en Direccion d.
     */
    void setHijo(Direccion d, const Celda &c);

    /**
     * Éste método nos indica al hijo de la celda en alguna dirección.
     * @param d es la direccion de la cual se quiere obtener al hijo.
     * @return un apuntador a la Celda hijo en dirección d.
     * @throws SinHijo si la Celda no tiene un hijo en esa posición.
     */
    Celda& getHijo(Direccion d) const;

    /**
     * Determina si la Celda enviada es hijo de la Celda actual.
     * @param hijo una referencia a una Celda, que se quiere saber si es hijo de
     * la Celda desde la que se llama al método.
     * @return true si "hijo" es hijo de la Celda actual, false de lo contrario.
     */
    bool tieneHijo(const Celda& hijo) const;

    /**
     *Bloquea el paso de la Celda hacia la dirección indicada.
     * @param d un valor del enum Direccion con la dirección que se desea bloquear.
     */
    void bloquearDireccion(Direccion d);

    /**
     * Desbloquea el paso de la Celda, hacia la dirección indicada, pone como
     * hijo en esa direccion a la Celda celdaHijo.
     * @param d una Direccion que se va a debloquear.
     * @param celdaHijo una referencia a la Celda que se pondrá como hijo en esa
     * dirección.
     */
    void desbloquearDireccion(Direccion d, Celda &celdaHijo);

    /**
     *Nos dice si la Direccion especificada esta libre o no.
     * @param d un valor del enum Direccion de la que se quiere saber su estado.
     * @return true si está libre el paso en esa Direccion, false de lo contario.
     */
    bool estaLibreDireccion(Direccion d) const;
    
private:
    /**
     * bandera de estado que nos indica si la Celda está bloqueada con un 
     * Jugador encima.
     */
    bool esta_libre;

    /**
     * Si tomamos a la Celda como un nodo del Grafo, esta constante es el número
     * de posibles hijos del nodo.
     */
    const static int num_hijos = OESTE + 1;

    /**
     *Aqui se guardan los apuntadores a los hijos de la Celda.
     */
    std::vector< Celda* > hijos;

    /**
     *Este atributo solo es usado para ayudar al constructor. le da tamaño al
     * vector de apuntadores a las Celdas hijo, y los inicializa con null. (Es
     * decir, se interpretan como bloqueados, y no conectados a ningún otro nodo)
     */
    void init_hijos();
};
}

/**
     * Este es una sobrecarga del operador "<<" de cout, para poder imprimir de
     * manera bonita e informativa a stdout la clase Celda. Ejemplo
     * @code
     * Reglas::Celda c;
     * c.colocar(0, 0);
     * std::cout << c << endl;
     * @endcode
     */
    std::ostream& operator<< (std::ostream &out, const Reglas::Celda &celda);
#endif	/* _CELDA_HPP */

