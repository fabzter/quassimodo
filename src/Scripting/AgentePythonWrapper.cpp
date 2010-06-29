#include "AgentePythonWrapper.hpp"

Scripting::AgentePythonWrapper::AgentePythonWrapper(Reglas::Agente *agente)
{
    this->agente = agente;

    this->modulo = boost::python::import("__main__");
    this->modulo_namespace = this->modulo.attr("__dict__");
}

Scripting::AgentePythonWrapper::AgentePythonWrapper(const AgentePythonWrapper& orig)
{
}

Scripting::AgentePythonWrapper::~AgentePythonWrapper()
{
    delete this->agente;
}

void Scripting::AgentePythonWrapper::iniciar(int id)
{
    try
    {
        this->agente->iniciar(id);
    }
    catch(boost::python::error_already_set& e)
    {
        manejar_excepcion_python_libre(e, this->modulo_namespace,
                                       this->modulo_namespace);
    }
}

Reglas::Jugada Scripting::AgentePythonWrapper::siguienteJugada()
{
    try
    {
        return this->agente->siguienteJugada();
    }
    catch(boost::python::error_already_set& e)
    {
        manejar_excepcion_python_libre(e, this->modulo_namespace,
                                       this->modulo_namespace);
    }
}

void Scripting::AgentePythonWrapper::terminar()
{
    try
    {
        this->agente->terminar();
    }
    catch(boost::python::error_already_set& e)
    {
        manejar_excepcion_python_libre(e, this->modulo_namespace,
                                       this->modulo_namespace);
    }
}