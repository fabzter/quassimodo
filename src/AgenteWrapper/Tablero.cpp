#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <Reglas/Tablero.hpp>
#include <Reglas/Barrera.hpp>
#include <sstream>

namespace py = pybind11;
using namespace Reglas;
using namespace std;

string print_tab(Tablero *tab)
{
    ostringstream strs;
    strs << *tab;
    return strs.str();
}

bool eq_tablero(Tablero *tab, Tablero *otro)
{
    return *tab == *otro;
}

void export_tablero(py::module_& m)
{
    py::class_<Tablero>(m, "Tablero")
        .def(py::init<const Tablero*>())

        .def("getPosicion", &Tablero::getPosicion,
             py::return_value_policy::reference_internal)

        .def("getBarrerasColocadas", &Tablero::getBarrerasColocadas,
             py::return_value_policy::reference_internal)

        .def("getCelda",
             py::overload_cast<int>(&Tablero::getCelda, py::const_),
             py::return_value_policy::reference)

        .def("getMetas", &Tablero::getMetas,
             py::return_value_policy::reference_internal)

        .def("moverJugador",
             py::overload_cast<int, int, int>(&Tablero::moverJugador))

        .def("moverJugador",
             py::overload_cast<int, const std::vector<int>&>(&Tablero::moverJugador))

        .def("setBarrera",
             py::overload_cast<int, const Barrera&>(&Tablero::setBarrera))

        .def("__eq__", &eq_tablero)
        .def("__str__", &print_tab)

        .def_readonly_static("size_x", &Tablero::size_x)
        .def_readonly_static("size_y", &Tablero::size_y)
        .def_readonly_static("tam_barrera", &Tablero::tam_barrera);
}
