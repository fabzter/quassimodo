/**
 */

#ifndef _INTERPRETEPYTHON_HPP
#define	_INTERPRETEPYTHON_HPP
#include<boost/python.hpp>
#include "Interprete.hpp"
#include "ModuloPython.hpp"
#include "UtilsPython.hpp"

namespace Scripting
{
class InterpretePython: public Interprete
{
public:
    InterpretePython();
    InterpretePython(const InterpretePython& orig);
    virtual ~InterpretePython();

    virtual void iniciar(Reglas::Tablero &t);
    virtual void finalizar();

    virtual Reglas::Agente *getAgente(std::string ruta);
    virtual bool manejaModulo(std::string ruta);

    static bool esta_pyiniciado;

private:
    Reglas::Tablero *tablero;
    bool esta_finalizado;
    
    static void PyInicializar();
    
protected:
    virtual void agregarModulo(std::string ruta);
};
}
#endif	/* _INTERPRETEPYTHON_HPP */

