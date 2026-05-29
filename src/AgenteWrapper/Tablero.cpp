#include "opaque_types.h"
#include <Reglas/Barrera.hpp>
#include <Reglas/Tablero.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <sstream>

namespace py = pybind11;
using namespace Reglas;
using namespace std;

// File-scope copies of inherited static const members — avoids ODR-use
// (these have no out-of-class definitions; Boost.Python used the same hack).
static const int Tablero_size_x = Tablero::size_x;
static const int Tablero_size_y = Tablero::size_y;
static const int Tablero_tam_barrera = Tablero::tam_barrera;

string print_tab(Tablero *tab) {
  ostringstream strs;
  strs << *tab;
  return strs.str();
}

bool eq_tablero(Tablero *tab, Tablero *otro) { return *tab == *otro; }

void export_tablero(py::module_ &m) {
  auto cls =
      py::class_<Tablero>(m, "Tablero", py::dynamic_attr())
          .def(py::init<const Tablero *>())

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

          .def("moverJugador", py::overload_cast<int, const std::vector<int> &>(
                                   &Tablero::moverJugador))

          .def("setBarrera",
               py::overload_cast<int, const Barrera &>(&Tablero::setBarrera))

          .def("__eq__", &eq_tablero)
          .def("__str__", &print_tab);

  cls.attr("size_x") = Tablero_size_x;
  cls.attr("size_y") = Tablero_size_y;
  cls.attr("tam_barrera") = Tablero_tam_barrera;
}
