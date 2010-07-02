
#ifndef PARTIDACONSOLA_HPP
#define	PARTIDACONSOLA_HPP
#include"Partida.hpp"

class PartidaConsola:public Partida {
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
    bool SetJugadores(std::string rutaAgente1,std::string rutaAgente2);

private:
    /**
     *@as Partida::actualizarTablero
     */
    virtual void actualizarTablero(Reglas::Jugada &j, int idJugador);
    /**
     * Tablero sobre el que se realizará la partida.
     */
    Reglas::Tablero* tablero;

};

#endif	/* PARTIDACONSOLA_HPP */

