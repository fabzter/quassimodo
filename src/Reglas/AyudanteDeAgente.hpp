/**
 */

#ifndef _AYUDANTEDEAGENTE_HPP
#define	_AYUDANTEDEAGENTE_HPP
#include "Tablero.hpp"
#include "Jugada.hpp"
#include "Juez.hpp"
#include "Astar.hpp"
#include <list>

namespace Reglas{
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

    /**
     * Nos regresa al Juez que revisa el Tablero con el que esta trabajando el
     * AyudanteDeAgente.
     * @return Una referencia al Juez que ocupa el AyudanteDeAgente.
     */
    Juez& getJuez();

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
};
}
#endif	/* _AYUDANTEDEAGENTE_HPP */

