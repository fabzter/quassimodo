#include <Reglas/Excepciones.hpp>
#include <pybind11/pybind11.h>
#include <sstream>
#include <string>

namespace py = pybind11;
using namespace Reglas;
using namespace std;

string Excepcion_to_str(Excepcion &e) {
  ostringstream strs;
  strs << e.what();
  return strs.str();
}

void export_excepciones(py::module_ &m) {
  py::register_exception<Excepcion>(m, "Excepcion", PyExc_RuntimeError);
  py::register_exception<ParametrosMalos>(m, "ParametrosMalos",
                                          PyExc_RuntimeError);
  py::register_exception<SinHijo>(m, "SinHijo", PyExc_RuntimeError);
  py::register_exception<PiezaNoColocada>(m, "PiezaNoColocada",
                                          PyExc_RuntimeError);
  py::register_exception<JugadorNoColocado>(m, "JugadorNoColocado",
                                            PyExc_RuntimeError);
  py::register_exception<JugadorSinBarreras>(m, "JugadorSinBarreras",
                                             PyExc_RuntimeError);
}
