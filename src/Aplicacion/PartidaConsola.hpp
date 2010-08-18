
#ifndef PARTIDACONSOLA_HPP
#define	PARTIDACONSOLA_HPP
#include"Partida.hpp"
#include<Reglas/Tablero.hpp>
/**
 * Clase encargada de mantener una partida en modo consola.
 * @as Partida
 */
class PartidaConsola :public Partida{
public:
    /**
     * Constructor de la clase. Crea un nuevo Tablero y un nuevo Juez.
     */
    PartidaConsola();
    /**
     * Cosntructor copia.
     * @param orig un objeto de la clase PartidaConsola
     */
    PartidaConsola(const PartidaConsola& orig);
    /**
     * Destructor
     */
    virtual ~PartidaConsola();
    /**
     *@as Partida::iniciarPartida
     */
    virtual void iniciarPartida();

    /**
     *@as Partida::siguienteJugada
     */
    virtual bool siguienteJugada();

    /**
     *@as Partida::SetJugadores
     */
    virtual bool SetJugadores(std::string rutaAgente1,std::string rutaAgente2);

    /**
     * Imprime el tablero en consola.
     */
    void impimeTablero();


private:
    /**
     *@as Partida::actualizarTablero
     */
    virtual void actualizarTablero(Reglas::Jugada &j, int idJugador);
    /**
     * Tablero sobre el que se realizará la partida.
     */
    Reglas::Tablero* t;

};

#endif	/* PARTIDACONSOLA_HPP */

