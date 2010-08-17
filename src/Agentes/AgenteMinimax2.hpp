#ifndef AGENTEMINIMAX2_HPP
#define	AGENTEMINIMAX2_HPP
#include <Reglas/Agente.hpp>
#include "FabricaAgente.hpp"

namespace Agentes
{

class AgenteMinimax2: public Reglas::Agente
{
public:
    AgenteMinimax2();
    AgenteMinimax2(const AgenteMinimax2& orig);
    virtual ~AgenteMinimax2();

    void iniciar(int id);

    Reglas::Jugada siguienteJugada(const Reglas::Tablero tablero,
                                            Reglas::AyudanteDeAgente &ayudante);
    void terminar();
private:
    int id;
    int id_enemigo;
};

class FabricaMinimax2: public FabricaAgente
{
public:
    Reglas::Agente* operator()();
};

}
#endif	/* AGENTEMINIMAX2_HPP */

