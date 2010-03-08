/**
 */

#ifndef _INTERPRETEPYTHON_HPP
#define	_INTERPRETEPYTHON_HPP
#include<boost/python.hpp>
#include "Interprete.hpp"
#include "ModuloPython.hpp"

namespace Scripting{
class InterpretePython: public Interprete {
public:
    InterpretePython();
    InterpretePython(const InterpretePython& orig);
    virtual ~InterpretePython();

    virtual void iniciar();
    virtual void finalizar();

    virtual Reglas::Agente *getAgente(std::string ruta);
    virtual bool manejaModulo(std::string ruta);
private:

protected:
    virtual void agregarModulo(std::string ruta);
};
}
#endif	/* _INTERPRETEPYTHON_HPP */

