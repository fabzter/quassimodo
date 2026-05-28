#include "UtilsPython.hpp"

void Scripting::manejar_excepcion_python_libre(
    pybind11::error_already_set& e,
    pybind11::object &globals,
    pybind11::object &locals)
{
    using namespace std;
    namespace py = pybind11;

    string mensaje("");
    PyObject *type, *value, *traceback;
    PyErr_Fetch(&type, &value, &traceback);

    try
    {
        py::handle hexc(type), hval(value), htb(traceback);
        if(!htb || !hval)
        {
            mensaje = py::str(hexc).cast<string>();
        }
        else
        {
            py::object traceback_mod(py::module_::import("traceback"));
            py::object format_exception(traceback_mod.attr("format_exception"));
            py::object formatted_list(format_exception(hexc, hval, htb));
            py::object formatted(py::str("\n").attr("join")(formatted_list));
            mensaje = formatted.cast<string>();
        }
    }
    catch(py::error_already_set&)
    {
        py::object modulo_main = py::module_::import("__main__");
        py::object modulo_main_namespace = modulo_main.attr("__dict__");
        py::exec("import traceback\nimport io\n"
                 "fout = io.StringIO()\n"
                 "traceback.print_exc(file=fout)\n"
                 "fout = fout.getvalue()\n",
                 modulo_main_namespace, modulo_main_namespace);
        mensaje = py::eval("fout", modulo_main_namespace,
                                   modulo_main_namespace).cast<string>();
    }

    throw ScriptMalo(mensaje);
}
