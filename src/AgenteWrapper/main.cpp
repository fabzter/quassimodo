#include "opaque_types.h"        // MAKE_OPAQUE block — must be first; see header
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>        // by-value crossing for std::list<T>
#include <pybind11/stl_bind.h>   // py::bind_vector

#include <Reglas/Tablero.hpp>
#include <Reglas/Celda.hpp>
#include <Reglas/Jugada.hpp>
#include <Reglas/Barrera.hpp>

namespace py = pybind11;

void export_pieza(py::module_& m);
void export_agente(py::module_& m);
void export_tablero(py::module_& m);
void export_celda(py::module_& m);
void export_enums(py::module_& m);
void export_barrera(py::module_& m);
void export_excepciones(py::module_& m);
void export_jugada(py::module_& m);
void export_ayudante_de_agente(py::module_& m);

PYBIND11_MODULE(Reglas, m) {
    // Register opaque container bindings FIRST (before class defs that
    // return them — converters must exist before py::class_ uses them).
    py::bind_vector<std::vector<int>>(m, "IntVector");
    py::bind_vector<std::vector<Reglas::Celda>>(m, "CeldaVector");
    py::bind_vector<std::vector<Reglas::Celda*>>(m, "CeldaPtVector");
    py::bind_vector<std::vector<Reglas::Jugada>>(m, "JugadaVector");

    export_enums(m);
    export_jugada(m);
    export_excepciones(m);
    export_pieza(m);
    export_agente(m);
    export_tablero(m);
    export_celda(m);
    export_barrera(m);
    export_ayudante_de_agente(m);
}
