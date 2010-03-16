/**
 */

#ifndef _EXCEPCIONES_HPP
#define	_EXCEPCIONES_HPP
#include <sstream>

namespace Reglas{

class Excepcion: public std::exception
{
public:
    Excepcion(std::string what);
    Excepcion();
    ~Excepcion() throw();
    virtual const char* what() const throw();
protected:
    std::string msg;
};


class PiezaFueraDelTablero: public Excepcion{
public:
    PiezaFueraDelTablero();
    PiezaFueraDelTablero(std::string what);
};

class CeldaFueraDelTablero: public PiezaFueraDelTablero{
public:
    CeldaFueraDelTablero();
    CeldaFueraDelTablero(std::string what);
};

/**
 * Excepcion arrojada cuando los parámetros nos cumplen con los requisitos
 * esperados.
 */
class ParametrosMalos: public Excepcion
{
public:
    ParametrosMalos();
    ParametrosMalos(std::string what);
};

/**
 * Excepción usada por la clase Jugador, cuando se intenta decrementar su numero
 * de barreras cuando éste ya no tiene más.
 */
class JugadorSinBarreras: public Excepcion{

};

/**
 * Esta excepción es lanzada cuando se trata de mover una Pieza que no ha sido
 * aún colocado en el Tablero.
 */
class PiezaNoColocada: public Excepcion{
public:
    PiezaNoColocada();
    PiezaNoColocada(std::string what);
};

/**
 *Esta excepción se arroja cuando se trata de colocar una Pieza que ya ha sido
 * colocada en el Tablero.
 */
class PiezaYaColocada: public Excepcion{
public:
    PiezaYaColocada();
    PiezaYaColocada(std::string what);
};

/**
 * Esta excepción es lanzada cuando se trata de mover a un Jugador que no ha sido
 * aún colocado en el Tablero.
 */
class JugadorNoColocado: public PiezaNoColocada{
public:
    JugadorNoColocado();
    JugadorNoColocado(std::string what);
};

/**
 *Esta excepción se arroja cuando se trata de colocar a un Jugador que ya ha sido
 * colocado en el Tablero.
 */
class JugadorYaColocado: public PiezaYaColocada{
};

class ReglasRotas: public Excepcion{
public:
    ReglasRotas(std::string what);
    ReglasRotas();
};

/**
 * Excepción arrojada cuando se trata de pedir una Jugada a una Partida que no
 * está en curso.
 */
class PartidaTerminada: public Excepcion{
public:
    PartidaTerminada(std::string what);
    PartidaTerminada();
};

/**
 * Excepción lanzada cuando se quiere realizar algún movimiento en una Partida que
 * aún no ha iniciaodo.
 */
class PartidaNoIniciada: public Excepcion{
public:
    PartidaNoIniciada(std::string what);
    PartidaNoIniciada();
};

}

#endif	/* _EXCEPCIONES_HPP */

