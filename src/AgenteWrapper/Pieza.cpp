#include "opaque_types.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <Reglas/Pieza.hpp>

namespace py = pybind11;
using namespace Reglas;

struct PyPieza : Pieza {
    using Pieza::Pieza;
    const std::vector<int>& getPosicion() const override {
        PYBIND11_OVERRIDE(const std::vector<int>&, Pieza, getPosicion);
    }
    bool operator==(const Pieza& otro) const override {
        PYBIND11_OVERRIDE(bool, Pieza, operator==, otro);
    }
};

void export_pieza(py::module_& m)
{
    py::class_<Pieza, PyPieza>(m, "Pieza", py::dynamic_attr())
        .def(py::init<>())
        .def("getPosicion", &Pieza::getPosicion,
             py::return_value_policy::reference_internal)
        .def("__eq__", &Pieza::operator==);
}
