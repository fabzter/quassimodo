#ifndef _MODULOPYTHON_HPP
#define _MODULOPYTHON_HPP
#include "Modulo.hpp"
#include <Reglas/Agente.hpp>
#include <Reglas/AyudanteDeAgente.hpp>

#include <pybind11/embed.h>
#include <string>
#include <list>

#include "Excepciones.hpp"
#include "UtilsPython.hpp"
#include "AgentePythonWrapper.hpp"

namespace Scripting {

namespace py = pybind11;

class ModuloPython: public Modulo {
public:
    ModuloPython();
    ModuloPython(const ModuloPython& orig);
    virtual ~ModuloPython();

    virtual void cargar(std::string ruta, Reglas::Tablero &t);
    virtual Reglas::Agente *getAgente();
    virtual void finalizar();

private:
    py::object modulo;
    py::object namespace_modulo;
    py::object agente_clase;
    bool esta_extraida_clase;
    std::list<py::object> instancias_clase;
    Reglas::AyudanteDeAgente *ayudante;

    void extraer_clase();
    void manejar_excepcion_python(py::error_already_set& e);
};

}
#endif
