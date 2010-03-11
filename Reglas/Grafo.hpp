/**
 */

#ifndef _GRAFO_HPP
#define	_GRAFO_HPP
#include "Celda.hpp"
#include "Tablero.hpp"
#include <list>
#include <algorithm>

namespace Reglas{
class Tablero;
/**
 * Grafo es una clase auxiliar de Tablero. Acomoda apuntadores de las Celdas del
 * Tablero de tal modo que puedan usarse cual si fueran nodos de un grafo.
 */
class Grafo {
public:
     /**
     *Este constructor recibe la matriz de Celas cuyos apuntadores serán usados para
     * contruir el grafo. Y lo construye!!! :D
     * @param datos es el vector de vectores (matriz) de Celdas, a partir de las cuales
     * se construirá el grafo.
     */
    Grafo(Tablero &tablero);
    Grafo(const Grafo& orig);
    virtual ~Grafo();

     /**
     *Nos indica si hay al menos un camino desde la posición del Jugador solicitado
     * hasta la meta que le corresponde
     * @param id El identificador del Jugador que busca un camino hacia su meta
     * @return true si existe almenos un camino, y false de lo contrario.
     */
    bool hayCaminoMeta(int idJugador) const;

    /**
     * Nos indica la Celda que es la meta del Jugador con id idJugador.
     * @param idJugador id del Jugador del que se quiere saber la meta.
     * @return Una referencia a la Celda que es el nodo meta del Jugador.
     */
    const Celda& getMeta(int idJugador) const;

private:
    /**
     * Esta es la Celda que sirve como meta del NORTE.
     */
    Celda meta_norte;

    /**
     * Esta es la Celda que sirce como meta del SUR.
     */
    Celda meta_sur;

    /**
     * Un apuntador al Tablero de Celdas. Nos ayuda a localizar la Celda en la
     * que se inicia una búsqueda.
     */
    Tablero *tablero;

    /**
     *Le da la forma de Grafo de las Celdas. Une sus apuntadores a otros nodos
     * con sus respectivos hijos, también conecta unidireccionalmente los nodos
     * meta.
     * @param tablero es un apuntador al Tablero que contiene las Celdas.
     */
    void init_grafo();

    /**
     *Realiza la busqueda recursiva del metodo hayCaminoMeta.
     * @param inicio Una referencia a la Celda desde la que se realiza la busqueda
     * @param meta Una referencia a la Celda que se esta buscando
     * @param visitados Una referencia hacia la lista de nodos que ya hemos visitado.
     * @return true si existe almenos un camino, y false de lo contrario.
     */
    bool 
    busqueda_recursiva(const Celda &inicio, const Celda &meta,
                        std::list<Celda*> &visitados)
    const;



};

}
#endif	/* _GRAFO_HPP */

