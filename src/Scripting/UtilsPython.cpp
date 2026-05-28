#include "UtilsPython.hpp"

void Scripting::manejar_excepcion_python_libre(pybind11::error_already_set &e,
                                               pybind11::object &globals,
                                               pybind11::object &locals) {
  using namespace std;

  // pybind11's what() formats the active Python exception; try it first.
  string mensaje = string(e.what());
  if (!mensaje.empty() && mensaje != "<NULL>") {
    throw ScriptMalo(mensaje);
  }

  // Fallback: manual extraction if what() returned empty
  PyObject *type = nullptr, *value = nullptr, *traceback = nullptr;
  PyErr_Fetch(&type, &value, &traceback);

  if (type || value) {
    namespace py = pybind11;
    try {
      py::handle hexc(type), hval(value), htb(traceback);
      if (htb && hval) {
        py::object tb(py::module_::import("traceback"));
        py::object fmt(tb.attr("format_exception"));
        py::object flist(fmt(hexc, hval, htb));
        py::object joined(py::str("\n").attr("join")(flist));
        mensaje = joined.cast<string>();
      } else if (hexc) {
        mensaje = py::str(hexc).cast<string>();
      }
    } catch (...) {
      PyErr_Clear();
    }
  }

  if (mensaje.empty())
    mensaje = "Python exception (details unavailable)";

  throw ScriptMalo(mensaje);
}
