#ifndef _AGENTE_HPP
#define	_AGENTE_HPP
#include "Tablero.hpp"
#include "Jugada.hpp"
#include "AyudanteDeAgente.hpp"

namespace Reglas
{
class Tablero;
class AyudanteDeAgente;

class Agente
{
public:
    /**
     *Este método es un aviso al Agente de que los dos Jugadores ya han sido
     * colocados dentro del tablero. Le da oportunidad de construir estructuras,
     * almacenar informacion, etc.
     * @param t una referencia constante al Tablero.
     * @param id es un entero que le indica al Agente su id de Jugador.
     */
    virtual void iniciar(int id) = 0;

    /**
     *Este método se usa para pedirle al Jugador la siguiente Jugada que piense
     * realizar. La Jugada podría no ser válida, asi que hay que revisarla.
     * @param tablero es una copia del Tablero actual del juego.
     * @param ayudante es una referencia al ayudante correspondiente al Tablero
     * actual.
     * @return un objeto Jugada con la Jugada que piensa realizar el Jugador.
     */
    virtual Jugada siguienteJugada(const Tablero tablero,
                                        const AyudanteDeAgente &ayudante) = 0 ;

    /**
     * Este método le avisa al Jugador que la Partida ha terminado. Ya sea por
     * que hay un ganador, o por que alguien rompió las reglas.
     */
    virtual void terminar() = 0;
};
}
#endif	/* _AGENTE_HPP */

