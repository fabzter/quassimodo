
#ifndef PARTIDACONSOLA_HPP
#define	PARTIDACONSOLA_HPP
#include"Partida.hpp"
#include<Reglas/Tablero.hpp>
class PartidaConsola :public Partida{
public:
    PartidaConsola();
    PartidaConsola(const PartidaConsola& orig);
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

