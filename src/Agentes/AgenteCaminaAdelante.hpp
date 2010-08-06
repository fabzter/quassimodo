#ifndef AGENTECAMINAADELANTE_HPP
#define	AGENTECAMINAADELANTE_HPP
#include <Reglas/Agente.hpp>
#include <Reglas/Jugada.hpp>

namespace Agentes
{

class AgenteCaminaAdelante: public Reglas::Agente
{
public:
    AgenteCaminaAdelante();
    AgenteCaminaAdelante(const AgenteCaminaAdelante& orig);
    virtual ~AgenteCaminaAdelante();

    void iniciar(int id);
    Reglas::Jugada siguienteJugada(const Reglas::Tablero tablero,
                                            Reglas::AyudanteDeAgente &ayudante);
    void terminar();

private:
    int id;
    Reglas::Direccion direccion;
    bool esta_iniciado;
};

}
#endif	/* AGENTECAMINAADELANTE_HPP */

