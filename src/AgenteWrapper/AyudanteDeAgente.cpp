#include <Reglas/AyudanteDeAgente.hpp>
#include <Reglas/Minimax.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;
using namespace Reglas;

std::vector<Celda *> __astar__(Tablero *t, int idJugador) {
  std::vector<void *> *path_void = astar(t, idJugador);
  std::vector<Celda *> path_celda;
  for (size_t i = 0; i < path_void->size(); i++)
    path_celda.push_back(static_cast<Celda *>(path_void->at(i)));
  delete path_void;
  return path_celda;
}

void export_ayudante_de_agente(py::module_ &m) {
  py::class_<AyudanteDeAgente>(m, "AyudanteDeAgente")
      .def(py::init<Tablero *>())
      .def("getMovimientosPosibles", &AyudanteDeAgente::getMovimientosPosibles)
      .def("getBarrerasPosibles", &AyudanteDeAgente::getBarrerasPosibles)
      .def("hayGanador", &AyudanteDeAgente::hayGanador);

  m.def("astar", &__astar__);
  m.def("minimax",
        py::overload_cast<Tablero *, int, int, int, TipoDeJugada>(&minimax));
}
