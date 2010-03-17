/* 
 * File:   Player.cpp
 * Author: fabrizio
 * 
 * Created on 14 de febrero de 2010, 08:13 PM
 */

#include "Player.hpp"

Player::Player()
{
}

Player::Player(const Player& orig)
{
}

Player::~Player()
{
}

void Player::iniciar(int id)
{
    
}

Reglas::Jugada Player::siguienteJugada()
{
    Reglas::Jugada j;
    j.setTipoDeJugada(Reglas::BARRERA);
    j.setDireccion(Reglas::NORTE);
    j.setPosicion(1, 0);
    return j;

}

void Player::terminar()
{
    
}