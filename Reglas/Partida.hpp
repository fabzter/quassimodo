

#ifndef _PARTIDA_HPP
#define	_PARTIDA_HPP
#include <list>
#include "Excepciones.hpp"
#include "Jugador.hpp"

namespace Reglas{
/**
 * Esta clase es una "fachada" para quien quiere interactuar con el módulo de Reglas
 * Inicializa todas las clases de dicho módulo
 */
class Partida {
public:
    /**
     *Es el único constructor de partida.
     * @param jugadores es un lista a referencias de los Jugadores que estarán
     * en la Partida.
     */
    Partida(const std::list< Jugador& > &jugadores);
    Partida(const Partida& orig);
    virtual ~Partida();
    
private:

};
}
#endif	/* _PARTIDA_HPP */

