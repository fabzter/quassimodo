/**
 */

#ifndef _JUGADOR_HPP
#define	_JUGADOR_HPP
#include "Agente.hpp"
#include "Jugada.hpp"
#include "Excepciones.hpp"
#include "Pieza.hpp"

namespace Reglas{

/**
 * Esta clase es rara. Tiene métodos publicos y asi, y también métodos
 * virtuales puros. Esto se debe a que, a parte de tener una funcionalidad, también
 * sirve de interfaz para los agentes. Tener cuidado de no exponer los métodos
 * implementados a las interfaces de los agentes!!
 */
class Tablero;
class Agente;
class Jugador: public Pieza {
public:
     /**
     *El constructor más básico de Jugador. Le proporciona un número de jugador
     * y lo pone como no colocado.
     * @param num un entero indicando el numero del jugador (num == 0 para el
     * Jugador 1).
      * @param a es un apuntador a un Agente creado con anterioridad.
     */
    Jugador(int num, Agente *a);
    Jugador(const Jugador& orig);
    virtual ~Jugador();

    /**
     *Cambia la posicion sobre la que se encuentra el Jugador actualmente.
     * @param nuevaPos es una referencia a un vector de dos enteros (x, y) que
     * representa la nueva posicion a la que se quiere mover.
     * @throws JugadorNoColocado si se intenta llamar antes de colocar al Jugador.
     * @throws ParametrosMalos si el vector no tiene dos enteros.
     */
    void mover(const std::vector<int> &nuevaPos);

    /**
     *Nos indica cuantas barreras disponibles le quedan al Jugador.
     * @return el número de barreras disponibles del Jugador.
     */
    int getBarrerasDisponibles() const;

    /**
     *Este método decrementa en 1 las barreras que le quedan al Jugador.
     * @throws JugadorSinBarreras si se trata de decrementar el número de barreras
     * cuando éste ya es == 0.
     */
    void decBarrerasDisponibles();

    /**
     *Este método incrementa en 1 las barreras que le quedan al Jugador.
     */
    void incBarrerasDisponibles();

    /**
     * Nos permite conocer el número identificador del Jugador actual.
     * @return un entero que representa el número del identificador del Jugador.
     */
    int getIdentificador() const;

    /**
     * Este entero representa el numero de Barreras iniciales con las que cuenta
     * el Jugador.
     */
    const static int barreras_iniciales = 10;

    /**
     * Delegado de Agente::iniciar. Manda a llamar al método iniciar del Agente
     * asignado al Jugador.s
     * @param t una referencia constante al Tablero.
     */
    virtual void iniciar(int id) const;

    /**
     * Delegado de Agente::siguienteJugada. Manda a llamar al método
     * siguienteJugada de Agente y regresa su resultado.
     * @return un objeto Jugada con la Jugada que piensa realizar el Agente
     * asignado al Jugador.
     */
    virtual Jugada siguienteJugada() const;

    /**
     * Delegado de Agente::terminar. Manda a llamar a dicho método del
     * Agente asignado al Jugador.
     */
    virtual void terminar() const;
private:
    /**
     * El identificador del jugador. Por ejemplo, el jugador 1 tiene num = 0.
     */
    int num;
    /**
     * Nos indica cauntas barreras le quedan al jugador. Debe construirse con 10.
     */
    int barreras_disponibles;

    /**
     * El Agente que se encarga de realizar los métodos iniciar, siguienteMovimiento
     * y terminar.
     */
    Agente* agente;

};
}
#endif	/* _JUGADOR_HPP */

