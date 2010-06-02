/**
 */

#ifndef _AYUDANTEDEAGENTE_HPP
#define	_AYUDANTEDEAGENTE_HPP
#include "Tablero.hpp"
#include "Jugada.hpp"
#include "Juez.hpp"
#include <list>
#include <algorithm>
#include <cmath>
#include <functional>
namespace Reglas{

 /*Constantes para el A* NO DOCUMENTAR.*/
enum AStar
{
    F,
    H,
    NUM,
    G,
    POS,
    OPEN,
    VALID,
    PARENT
};
/**
 * Esta clase sirve como un ayudante para el Agente implementado en un Script.
 * Debe ser expuesto, tal como el Tablero lo es, y le da información útil al
 * agente.
 */
class AyudanteDeAgente {
public:
    /**
     * Único constructor de AyudanteDeAgente. Recibe una referencia al Tablero
     * de la Partida. No toma posesión de él.
     * @param t una referencia al Tablero de la Partida en curso.
     */
    AyudanteDeAgente(Tablero &t);

    /**
     * Es igual al constructor que recibe un referencia al Tablero, solamente
     * que este recibe un apuntador.
     * @param t un apuntador al Tablero de la Partida en curso.
     */
    AyudanteDeAgente(Tablero *t);

    /**
     * Se queda con el apuntador al Tablero del AyudanteDeAgente origen, pero
     * crea su propio Juez.
     * @param orig una referencia al AyudanteDeAgente origen.
     */
    AyudanteDeAgente(const AyudanteDeAgente& orig);

    /**
     * Igual que el constructor copia, pero recibe un apuntador en lugar de una
     * referenica al origen.
     * @param orig un apuntador al AyudanteDeAgente de origen.
     */
    AyudanteDeAgente(const AyudanteDeAgente* orig);
    
    virtual ~AyudanteDeAgente();

    /**
     * Nos indica todas las Jugadas de tipo MOVIMIENTO que están disponibles
     * para el Jugador solicitado.
     * @param numJugador el número del Jugador del cuál se quiere saber sus
     * Jugadas.
     * @return una lista de las Jugadas de cada uno de los movimientos que tiene
     * disponible el Jugador.
     */
    std::list<Jugada> getMovimientosPosibles(int numJugador);

    /**
     * Nos indica todas las Jugadas de tipo BARRERA que están disponibles para
     * el Jugador solicitado. Regresa una lista vacía si el Jugador no tiene
     * ya Barreras.
     * @param numJugador el número del Jugador del cuál se quiere saber sus
     * Jugadas.
     * @return una lista de las Jugadas de cada uno de las Barreras que tiene
     * disponibles a colocar el Jugador.
     */
    std::list<Jugada> getBarrerasPosibles(int numJugador);

    /*NO DOCUMENTAR. Realiza la busqueda A* y regresa un par de
     * (Tablero, Jugada) con la siguiente Jugada a la que se debe mover el
     Jugador con id idJugador*/
    std::pair<Reglas::Tablero*, Reglas::Jugada*>* astar(int idJugador, int limit);

    struct AcomodadorHeap
    {
        bool operator()(std::vector<int> *first, std::vector<int> *second)
        {
            return first->at(0) > second->at(0);
        }
    };

    struct AcomodadorMap
    {
        bool operator()(const Tablero &first, const Tablero &second)
        {
            return &first < &second;
        }
    };

private:
    Tablero *tablero;
    Juez *juez;

    /**
     * Nos dice en cuanto aumenta X si nos movemos una posición en la dirección
     * d.
     * @param d la dirección en la que nos movemos.
     * @return 0, 1, o -1.
     */
    int getDeltaX(Direccion d);

    /**
     * Nos dice en cuanto aumenta Y si nos movemos una posición en la dirección
     * d.
     * @param d la dirección en la que nos movemos.
     * @return 0, 1, o -1.
     */
    int getDeltaY(Direccion d);

    /**
     * Agrega la Jugada j a la lista de Jugada js sólo si la Jugada j es válida
     * Para el Jugador seleccionado y en el Tablero con que fue iniciado el
     * AyudanteDeAgente.
     * @param numJugador número del Jugador que realiza la Jugada.
     * @param j la Jugada a ser introducida en la lista.
     * @param js la lista de Jugadas que posiblemente sea modificada.
     */
    void agregarJugadaSelectivamente(int numJugador, Jugada &j,
                                    std::list<Jugada> &js);

    /**
     * Realiza el trabajo de los constructores que reciben un Tablero. Se quedan
     * un apuntador a él, pero no toman posecion del apuntador.
     * @param t el Tablero que se usa para este ayudante.
     */
    void init(Tablero &t);

    /**
     * Realiza el trabajo del constructor copia. Se queda un un apuntador al
     * mismo Tablero que el AyudanteDeAgente origen, pero crea su propio Juez.
     * @param orig una referencia al AyudanteDeAgente de origen.
     */
    void copiar(const AyudanteDeAgente &orig);

    /*Siguen funciones de apoyo al A*, NO DOCUMENTARLAS PARA DOXYGEN.*/

    /* Regresa los tableros que se pueden crear moviendo al Jugador con id 
     * idJugador a todos sus movimientos posibles, junto con la jugada que llevo
     * a dicho Tablero. Deja la propiedad de los apuntadores al llamador.
     */
    std::list< std::pair<Reglas::Tablero*, Reglas::Jugada*> >
    neighbors(int idJugador);

    /*Indica si el Tablero actual es meta del jugador idJugador (es decir, si el
     * Jugador con id idJugador ha llegado a su meta)
     */
    bool goal(int idJugador);

    /*Regresa la distancia cuadrada entre los dos puntos a y b*/
    int manhattan(const std::vector<int> &a, const std::vector<int> &b);

    /*Regresa el costo de mover al Jugador con id idJugador de su posicion en el
     tablero del ayudante a su posicion en el Tablero destino.*/
    int cost(Tablero* destino, int idJugador);

    /*Regresa el costo estimado para llevar al Jugador con id idJugador de su
     posicion en el tablero del ayudante a su meta.*/
    int heuristic(int idJugador);

};
}
#endif	/* _AYUDANTEDEAGENTE_HPP */

