#include <pybind11/pybind11.h>
#include <Reglas/Enums.hpp>

namespace py = pybind11;
using namespace Reglas;

void export_enums(py::module_& m)
{
    py::enum_<TipoDeJugada>(m, "TipoDeJugada")
        .value("BARRERA", BARRERA)
        .value("MOVIMIENTO", MOVIMIENTO);

    py::enum_<Direccion>(m, "Direccion")
        .value("NORTE", NORTE)
        .value("ESTE", ESTE)
        .value("SUR", SUR)
        .value("OESTE", OESTE);
}
