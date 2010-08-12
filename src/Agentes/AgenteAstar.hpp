#ifndef AGENTEASTAR_HPP
#define	AGENTEASTAR_HPP
#include <Reglas/Tablero.hpp>
#include <Reglas/Agente.hpp>
#include <Reglas/Jugada.hpp>
#include <Reglas/AyudanteDeAgente.hpp>

#include "FabricaAgente.hpp"

namespace Agentes
{

class AgenteAstar: public Reglas::Agente
{
public:
    AgenteAstar();
    AgenteAstar(const AgenteAstar& orig);
    virtual ~AgenteAstar();

    void iniciar(int id);

    Reglas::Jugada siguienteJugada(const Reglas::Tablero tablero, 
                                            Reglas::AyudanteDeAgente &ayudante);
    void terminar();
private:
    int id;
};

class FabricaAstar: public FabricaAgente
{
public:
    Reglas::Agente* operator()();
};

}
#endif	/* AGENTEASTAR_HPP */

