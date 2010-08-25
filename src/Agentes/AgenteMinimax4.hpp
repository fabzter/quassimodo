#ifndef AGENTEMINIMAX4_HPP
#define	AGENTEMINIMAX4_HPP
#include <Reglas/Agente.hpp>
#include "FabricaAgente.hpp"

namespace Agentes
{

class AgenteMinimax4: public Reglas::Agente
{
public:
    AgenteMinimax4();
    AgenteMinimax4(const AgenteMinimax4& orig);
    virtual ~AgenteMinimax4();

    void iniciar(int id);

    Reglas::Jugada siguienteJugada(const Reglas::Tablero tablero,
                                            Reglas::AyudanteDeAgente &ayudante);
    void terminar();
private:
    int id;
    int id_enemigo;
};

class FabricaMinimax4: public FabricaAgente
{
public:
    Reglas::Agente* operator()();
};

}
#endif	/* AGENTEMINIMAX4_HPP */

