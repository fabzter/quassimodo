#ifndef AGENTEASTAR_HPP
#define	AGENTEASTAR_HPP
#include <Reglas/Tablero.hpp>
#include <Reglas/Agente.hpp>
#include <Reglas/Jugada.hpp>
#include <Reglas/AyudanteDeAgente.hpp>

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

}
#endif	/* AGENTEASTAR_HPP */

