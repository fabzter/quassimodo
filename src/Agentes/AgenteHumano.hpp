
#ifndef AGENTEHUMANO_HPP
#define	AGENTEHUMANO_HPP
#include <Reglas/Tablero.hpp>
#include <Reglas/Agente.hpp>
#include <Reglas/Jugada.hpp>
#include <Reglas/AyudanteDeAgente.hpp>

#include "FabricaAgente.hpp"

namespace Agentes
{

class AgenteHumano: public Reglas::Agente
{
public:
    AgenteHumano();
    AgenteHumano(const AgenteHumano& orig);
    virtual ~AgenteHumano();

    void iniciar(int id);

    Reglas::Jugada siguienteJugada(const Reglas::Tablero tablero,
                                            Reglas::AyudanteDeAgente &ayudante);
    void terminar();
    void SetJugada( Reglas::Jugada &j);
private:
    int id;
    Reglas::Jugada jugada;
};

class FabricaHumano: public FabricaAgente
{
public:
    Reglas::Agente* operator()();
};

}

#endif	/* AGENTEHUMANO_HPP */

