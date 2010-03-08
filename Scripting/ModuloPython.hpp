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

    virtual void cargar(std::string ruta);
    virtual Reglas::Agente *getAgente();
    virtual void finalizar();
private:

     /**
     * El módulo de Boost::Python.
     */
    boost::python::object modulo;

    /**
     * El namespace del modulo.
     */
    boost::python::object namespace_modulo;
};
}
#endif	/* _MODULOPYTHON_HPP */

