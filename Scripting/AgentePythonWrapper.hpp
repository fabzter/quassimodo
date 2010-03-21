/**
 */

#ifndef _AGENTEPYTHONWRAPPER_HPP
#define	_AGENTEPYTHONWRAPPER_HPP
#include <boost/python.hpp>
#include <Reglas/Agente.hpp>
#include <Reglas/Jugada.hpp>
#include "UtilsPython.hpp"

/**
 * Esta clase es un envoltorio para Agente, su propósito es envolver (con
 * agregación)  un Agente creado con un Módulo de python, para traducir sus
 * excepciones de error_set a ScriptMalo. Toma la propiedad del agente con el
 * que se construye.
 */
namespace Scripting
{
class AgentePythonWrapper: public Reglas::Agente {
public:
    AgentePythonWrapper(Reglas::Agente *agente);
    AgentePythonWrapper(const AgentePythonWrapper& orig);
    virtual ~AgentePythonWrapper();

    /**
     * Ejecuta iniciar del Agente que maneja, y en caso de recibir una excepción
     * de boost::python, la traduce a ScriptMalo, manteniendo su mensaje de
     * error.
     * @param id el id del Jugador que se está iniciando.
     * @throws ScriptMalo.
     */
    virtual void iniciar(int id);

    /**
     * Ejecuta siguiene del Agente que maneja, y en caso de recibir una excepción
     * de boost::python, la traduce a ScriptMalo, manteniendo su mensaje de
     * error.
     * @return la Jugada que realizó el Agente.
     * @throws ScriptMalo.
     */
    virtual Reglas::Jugada siguienteJugada();

    /**
     * Ejecuta terminar del Agente que maneja, y en caso de recibir una excepción
     * de boost::python, la traduce a ScriptMalo, manteniendo su mensaje de
     * error.
     * @throws ScriptMalo.
     */
    virtual void terminar();
private:
    /**
     * Este es el Agente que maneja el envoltorio. Contiene a un Agente creado
     * con un módulo de Python.
     */
    Reglas::Agente *agente;

    /**
     * Este miembro contiene el módulo de python (__main__) donde se ejecutará
     * la traducción de la excepción.
     */
    boost::python::object modulo;

    /**
     * Este miembro contiene el diccionario del módulo de python (__main__)
     * donde se ejecutará la traducción de la excepción.
     */
    boost::python::object modulo_namespace;

};
}
#endif	/* _AGENTEPYTHONWRAPPER_HPP */

