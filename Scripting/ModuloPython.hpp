/* 
 */

#ifndef _MODULOPYTHON_HPP
#define	_MODULOPYTHON_HPP
#include "Modulo.hpp"
#include <Reglas/Agente.hpp>

#include <boost/python.hpp>
#include <boost/python/enum.hpp>
#include <boost/python/def.hpp>
#include <boost/python/module.hpp>
#include <boost/python/object.hpp>
#include <boost/python/import.hpp>
#include <ctime>
#include <string>

#include "Excepciones.hpp"

namespace Scripting{
class ModuloPython: public Modulo{

public:
    ModuloPython();
    ModuloPython(const ModuloPython& orig);
    virtual ~ModuloPython();

    virtual void cargar(std::string ruta, Reglas::Tablero &t);
    virtual Reglas::Agente *getAgente();
    virtual void finalizar();
private:

     /**
     * El módulo de Boost::Python.
     */
    boost::python::object modulo_main;

    /**
     * El namespace del modulo.
     */
    boost::python::object namespace_modulo_main;

    /**
     * Este objeto mantiene LA CLASE derivada de Agente después de que se cargó
     * el Modulo.
     */
    boost::python::object agente_clase;

    /**
     * Aqui se guardan apuntadores a las instancias creadas.
     */
    std::list<boost::python::object> instancias_clase;

    /**
     * Extrae la clase Derivada de Agente del Modulo.
     * @throws ModuloNoCargado si se intenta llamar antes de haber cargado el
     * Modulo.
     * @throws ScriptMalo si el Modulo no contiene una clase derivada de Agente.
     */
    void extraer_clase();

    /**
     * Manejo interno de las excepciones del Modulo.
     */
    int manejar_excepcion_python(boost::python::error_already_set& e);
};
}
#endif	/* _MODULOPYTHON_HPP */

