/**
 */

#ifndef _AYUDANTEDEAGENTE_HPP
#define	_AYUDANTEDEAGENTE_HPP
#include "Tablero.hpp"
#include "Jugada.hpp"
#include "Juez.hpp"
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
    AyudanteDeAgente(const AyudanteDeAgente& orig);
    virtual ~AyudanteDeAgente();

    /**
     * Nos indica todas las Jugadas de tipo Movimiento que están disponibles
     * para el Jugador solicitado.
     * @param numJugador el número del Jugador del cuál se quiere saber sus
     * Jugadas.
     * @return una lista de las Jugadas de cada uno de los movimientos que tiene
     * disponible el jugador.
     */
    std::list<Jugada> getMovimientosPosibles(int numJugador);
private:
    Tablero *tablero;
    Juez *juez;
    static const int jugadas_posibles = 12;

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
};
}
#endif	/* _AYUDANTEDEAGENTE_HPP */

