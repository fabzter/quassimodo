/* 
 * File:   Player.hpp
 * Author: fabrizio
 *
 * Created on 14 de febrero de 2010, 08:13 PM
 */

#ifndef _PLAYER_HPP
#define	_PLAYER_HPP
#include <Reglas/Jugador.hpp>
#include <Reglas/Jugada.hpp>
#include <Reglas/Tablero.hpp>

class Player: public Reglas::Agente {
public:
    Player();
    Player(const Player& orig);
    virtual ~Player();

    virtual void iniciar(int id);
    virtual Reglas::Jugada siguienteJugada();
    virtual void terminar();
private:

};

#endif	/* _PLAYER_HPP */

