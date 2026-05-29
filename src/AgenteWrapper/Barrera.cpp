#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <pybind11/operators.h>
#include <Reglas/Pieza.hpp>
#include <Reglas/Barrera.hpp>
#include <Reglas/Jugada.hpp>
#include <Reglas/Enums.hpp>

namespace py = pybind11;
using namespace Reglas;

void export_barrera(py::module_& m)
{
    py::class_<Barrera, Pieza>(m, "Barrera", py::dynamic_attr())
        .def(py::init<Jugada*>())
        .def("colocar",
             py::overload_cast<const std::vector<int>&, Direccion>(&Barrera::colocar))
        .def("colocar",
             py::overload_cast<int, int, Direccion>(&Barrera::colocar))
        .def("getDireccion", &Barrera::getDireccion)
        .def("getPuntoMedio", &Barrera::getPuntoMedio,
             py::return_value_policy::reference_internal)
        .def("getPunta", &Barrera::getPunta,
             py::return_value_policy::reference_internal)
        .def(py::self == py::self);
}
