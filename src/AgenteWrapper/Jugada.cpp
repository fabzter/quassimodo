#include <Reglas/Celda.hpp>
#include <Reglas/Enums.hpp>
#include <Reglas/Jugada.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>

namespace py = pybind11;
using namespace Reglas;

void export_jugada(py::module_ &m) {
  py::class_<Jugada>(m, "Jugada")
      .def(py::init<>())
      .def(py::init<Celda *>())
      .def("setPosicion", py::overload_cast<int, int>(&Jugada::setPosicion))
      .def("setPosicion",
           py::overload_cast<const std::vector<int> &>(&Jugada::setPosicion))
      .def("getPosicion", &Jugada::getPosicion,
           py::return_value_policy::reference_internal)
      .def("setTipoDeJugada", &Jugada::setTipoDeJugada)
      .def("getTipoDeJugada", &Jugada::getTipoDeJugada)
      .def("setDireccion", &Jugada::setDireccion)
      .def("getDireccion", &Jugada::getDireccion);
}
