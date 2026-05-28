#include <pybind11/pybind11.h>
#include <Reglas/Celda.hpp>
#include <Reglas/Enums.hpp>
#include <Reglas/Pieza.hpp>
#include <sstream>

namespace py = pybind11;
using namespace Reglas;
using namespace std;

string Celda_to_str(Celda *c)
{
    ostringstream strs;
    strs << *c;
    return strs.str();
}

void export_celda(py::module_& m)
{
    py::class_<Celda, Pieza>(m, "Celda")
        .def(py::init<Jugada&>())
        .def("estaLibre", &Celda::estaLibre)
        .def("getHijo", &Celda::getHijo, py::return_value_policy::reference)
        .def("estaLibreDireccion", &Celda::estaLibreDireccion)
        .def("__str__", &Celda_to_str);
}
