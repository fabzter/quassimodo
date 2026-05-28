#ifndef _AGENTEPYTHONWRAPPER_HPP
#define _AGENTEPYTHONWRAPPER_HPP
#include <pybind11/embed.h>
#include <Reglas/Agente.hpp>
#include <Reglas/Jugada.hpp>
#include "UtilsPython.hpp"

namespace Scripting
{
class AgentePythonWrapper: public Reglas::Agente {
public:
    AgentePythonWrapper(Reglas::Agente *agente);
    AgentePythonWrapper(const AgentePythonWrapper& orig);
    virtual ~AgentePythonWrapper();

    virtual void iniciar(int id);
    virtual Reglas::Jugada siguienteJugada();
    virtual void terminar();

private:
    Reglas::Agente *agente;
    pybind11::object modulo;
    pybind11::object modulo_namespace;
};
}
#endif
