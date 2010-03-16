#ifndef _PARTIDA_HPP
#define	_PARTIDA_HPP
#include <list>
#include <Scripting/Excepciones.hpp>
#include "Tablero.hpp"
#include "Juez.hpp"
#include "Excepciones.hpp"
#include "Jugador.hpp"

namespace Reglas{
/**
 * Esta clase es una "fachada" para quien quiere interactuar con el módulo de 
 * Reglas.
 * Representa una partida corriendo del juego. Mantiene la informción de a qué
 * Jugador le toca realizar una Jugada, y extrae la Jugada del Jugador que le
 * toca tirar. También actualiza el Tablero si la Jugada es válida, de no serlo
 * así, suelta una excepción (ReglasRotas).
 */
class Partida {
public:
    /**
     *Es el único constructor de Partida. Construye una Partida con el Tablero
     * indicado (debe estar inicializado perfectamente.)
     * (Deben pasarse objetos ya inicializados o se generarán
     * excepciones). Al ejecutarse correctamente, la Partida sigue como no iniciada
     * hasta que se llame el método iniciarPartida.
     * @param t es un apuntador al Tablero sobre el que se realiza la Partida.
     */
    Partida(Tablero *t);
    Partida(const Partida& orig);

    /**
     * Limpia sólo sus recursos (como el Juez), pero no a los Jugadores ni al
     * Tablero, por ejemplo.
     */
    virtual ~Partida();

    /**
     * Una vez que se ha contruido la partida, este método les avisa a los
     * Jugadores que su partida está por empezar.
     */
    void iniciarPartida();

    /**
     * Éste método es el punto principal de interacción con la clase Partida.
     * Le pida una Partida del Jugador que al que le toca realizar una, y de ser
     * ésta válida, actualiza el Tablero para reflejar los cambios hechos por la
     * misma. Esto último incluye actualizar la lista de Barreras colocadas,
     * bloquear las Direcciones de las Celdas dependiendo de las Barreras,poner
     * a los Jugadores en su lugar, poner como ocupadas las Celdas sobre las que
     * están los Jugadores.
     * @return true si se realizó la jugada
     * @return false si la Jugada ha terminado (por ejemplo si ganó un Jugador)
     * @throws ReglasRotas si la Jugada no comple con las reglas.
     * @throws ScriptMalo Si el script tiene errores de sintaxis o algún otro
     * tipo de error relacionado con el lenguage de scripting.
     * @throws PartidaTerminada si se intenta llamar cuando la partida ha
     * terminado por cualquier motivo.
     */
    bool siguienteJugada();

    /**
     * Nos indica el valor de la bandera esta_en_curso.
     * @return true si la Partida está andando, false si se ha detenido por
     * algún motivo (como que hay un ganador o se han roto las reglas).
     */
    bool estaEnCurso();
    
private:

    /**
     * Llamada por siguienteJugada, si la Jugada es válida. Realiza la parte de
     * actualización especificada en siguienteJugada.
     * @sa siguienteJugada.
     * @param j es la Jugada con la que se a actualizar el Tablero.
     * @param idJugador es el id del Jugador que realiza la Jugada.
     */
    void actualizarTablero(Jugada &j, int idJugador);
    /**
     * Este es el Juez que se usará en ésta partida. Atado al Tablero con que se
     * construye la Partida, éste Juez se construye en el contructor de Partida.
     */
    Juez* juez;

    /**
     * Tablero sobre el que se realizará la partida.
     */
    Tablero* tablero;

    /**
     * El identificador del Jugador al que se le pedirá la siguiente Jugada.
     */
     int jugador_en_turno;

     /**
      * Tiene el id del Jugador que ha ganado la Partida.
      */
     int jugador_ganador;

     /**
      * Bandera que se prende en true cuando hay un ganador.
      */
     bool hay_ganador;

     /**
      * Bandera que se encuentra en true mientras la Partida no ha terminado.
      */
     bool en_curso;
};
}
#endif	/* _PARTIDA_HPP */

