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
    PyGILState_STATE gstate=PyGILState_Ensure();
    try
    {
        this->agente->iniciar(id);
    }
    catch(boost::python::error_already_set& e)
    {
         PyGILState_Release(gstate);
        manejar_excepcion_python_libre(e, this->modulo_namespace,
                                       this->modulo_namespace);
    }
     PyGILState_Release(gstate);
}

Reglas::Jugada Scripting::AgentePythonWrapper::siguienteJugada()
{
    Reglas::Jugada j;
    //PyGILState_STATE gstate=PyGILState_Ensure();
    try
    {
         j = this->agente->siguienteJugada();
    }
    catch(boost::python::error_already_set& e)
    {
     //   PyGILState_Release(gstate);
        manejar_excepcion_python_libre(e, this->modulo_namespace,
                                       this->modulo_namespace);
    }
    //PyGILState_Release(gstate);
    return j;
}

void Scripting::AgentePythonWrapper::terminar()
{
    PyGILState_STATE gstate=PyGILState_Ensure();
    try
    {
        this->agente->terminar();
    }
    catch(boost::python::error_already_set& e)
    {
        PyGILState_Release(gstate);
        manejar_excepcion_python_libre(e, this->modulo_namespace,
                                       this->modulo_namespace);
    }
    PyGILState_Release(gstate);
}