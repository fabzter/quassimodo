/**
 * 
 */

#ifndef _JUEZ_HPP
#define	_JUEZ_HPP
#include"Tablero.hpp"
#include "Celda.hpp"
#include "Jugada.hpp"
#include "Barrera.hpp"

#include "Excepciones.hpp"

namespace Reglas
{
class Tablero;

/**
 * Esta clase es la que se encarga de revisar las reglas del juego. Pide una a una
 * a os Jugadores sus Jugadas, las revisa, y de ser correctas, actualiza el Tablero
 * con toda la información pertinente (barreras, posiciones de Jugadores, celdas
 * con bloqueo.)
 */
class Juez {
public:
    /**
     *El constructor más simple de la clase Juez, le pasa el tablero de donde
     * el Juez podrá obtener toda la información.
     * @param t un apuntador al Tablero de la Partida.
     */
    Juez(Tablero& t);
    Juez(const Juez& orig);
    virtual ~Juez();

    /**
     *Pide la siguiente Jugada al Jugador correspondiente. Y revisa la validez.
     * @return La Jugada si ésta cumple las reglas.
     * @throws ReglasRotas si la Jugada no comple con las reglas.
     * @throws ScriptMalo Si el script tiene errores de sintaxis.
     * @param t es una copia del Tablero en su estado actual.
     */
    Jugada siguienteJugada(int idJugador);

    /**
     * Nos avisa si hay un ganador ya en el Tablero.
     * @return el id del Jugador que ha ganado.
     * @return -1 si no hay ganador.
     */
    int hayGanador();
private:
    Tablero* tablero;

    /**
     *Revisa que la Jugada cumpla con <strong>todas</strong> las reglas.
     * @param j una referencia a la Jugada que se quiere revisar.
     * @param idJugador el id del Jugador que realiza la Jugada.
     * @throws ReglasRotas si la Jugada no comple con las reglas.
     * @throws ScriptMalo Si el script tiene errores de sintaxis.
     */
    void revisar_reglas(Jugada& j, int idJugador);

    /**
     *Se debe o poner una barrera o moverse.
     * @param j una referencia a la Jugada que se quiere revisar.
     * @param idJugador el id del Jugador que realiza la Jugada.
     * @throws ReglasRotas si la Jugada no comple con las reglas.
     * @throws ScriptMalo Si el script tiene errores de sintaxis.
     */
    void regla_1(Jugada& j, int idJugador);

    /**
     *No se puede salir del Tablero (incluye barreras)
     * @param j una referencia a la Jugada que se quiere revisar.
     * @param idJugador el id del Jugador que realiza la Jugada.
     * @throws ReglasRotas si la Jugada no comple con las reglas.
     * @throws ScriptMalo Si el script tiene errores de sintaxis.
     */
    void regla_2(Jugada& j, int idJugador);

    /**
     *Los Jugadores solo pueden moverse una Celda a la vez hacia uno de los
     * cuatro puntos cardinales. (Si se topa con otro peón, lo puede saltar.)
     * @param j una referencia a la Jugada que se quiere revisar.
     * @param idJugador el id del Jugador que realiza la Jugada.
     * @throws ReglasRotas si la Jugada no comple con las reglas.
     * @throws ScriptMalo Si el script tiene errores de sintaxis.
     */
    void regla_3(Jugada& j, int idJugador);

    /**
     * Método auxiliar de regla_3.
     *Nos indica si la Celda en la posicion pos, es hija de la celdaActual
     * @param pos una referencia a un vector de entros (x, y) que nos indica la
     * posicion a la que se quiere llegar.
     * @param celdaActual un apuntador a la Celda desde la que se parte
     * @return true si la Celda en pos es hija de celdaActual.
     */
    bool es_hijo(const std::vector<int>& pos, const Celda &celdaActual);

    /**
     * Método auxiliar de regla_3, dado un vector direccion
     * de la Jugada, regresa la Dirección de la Jugada.
     * @param vect_dir un vector de dos enteros que representa el vector dirección
     * @return -1 si la dirección no es un valor del enum Direccion, o un entero
     * con su valor de Direccion correspondiente.
     */
    int getDireccionMovimiento(const std::vector<int> &vect_dir);
    /**
     * Método auxiliar de regla_3, dado el vector de la posición actual y otro
     * de la posición de la Jugada, regresa la Dirección de la Jugada.
     * @param dir_jugada Un vector de dos enteros que representa la direccion a la que
     * se quiere llegar
     * @param dir_actual Yn vector de dos enteros que representa la posicion desde donde
     * quiere moverse
     * @return -1 si la dirección no es un valor del enum Direccion, o un entero
     * con su valor de Direccion correspondiente.
     */
    int getDireccionMovimiento(const std::vector<int> &dir_jugada,
                                        const std::vector<int> &dir_actual);
    
    /**Método auxiliar de la regla 3 dado el vector de la posición actual y otro
     * de la posición de la Jugada regresa el vector de la dirección de donde se quiere mover
     * @param dir_jugada Un vector de dos enteros que representa la direccion a la que
     * se quiere llegar
     * @param dir_actual Yn vector de dos enteros que representa la posicion desde donde
     * quiere moverse
     * @return Un vector de dos enteros que representa la direccion del movimiento
     */
    std::vector<int> getVectDireccion(const std::vector<int> &dir_jugada,
                                        const std::vector<int> &dir_actual);

    /**
     *No se puede mover hacia una dirección si existe una barrera.
     * @param j una referencia a la Jugada que se quiere revisar.
     * @param idJugador el id del Jugador que realiza la Jugada.
     * @throws ReglasRotas si la Jugada no comple con las reglas.
     * @throws ScriptMalo Si el script tiene errores de sintaxis.
     */
    void regla_4(Jugada& j, int idJugador);

    /**
     *Si el Jugador no tiene más barreras disponibles, SOLO podrá moverse.
     *
     * @param j una referencia a la Jugada que se quiere revisar.
     * @param idJugador el id del Jugador que realiza la Jugada.
     * @throws ReglasRotas si la Jugada no comple con las reglas.
     * @throws ScriptMalo Si el script tiene errores de sintaxis.
     */
    void regla_5(Jugada& j, int idJugador);

    /**
     *No se puede encerrar a un Jugador. (Cada Jugador debe tener al menos un
     * camino para llegar a la meta)
     *
     * @param j una referencia a la Jugada que se quiere revisar.
     * @param idJugador el id del Jugador que realiza la Jugada.
     * @throws ReglasRotas si la Jugada no comple con las reglas.
     * @throws ScriptMalo Si el script tiene errores de sintaxis.
     */
    void regla_6(Jugada& j, int idJugador);
    
    /**
     *No se pueden poner Barreras sobrelapadas.
     *
     * @param j una referencia a la Jugada que se quiere revisar.
     * @param idJugador el id del Jugador que realiza la Jugada.
     * @throws ReglasRotas si la Jugada no comple con las reglas.
     * @throws ScriptMalo Si el script tiene errores de sintaxis.
     */
    void regla_7(Jugada& j, int idJugador);
};
}
#endif	/* _JUEZ_HPP */

