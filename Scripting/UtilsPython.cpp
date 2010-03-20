/*
 */

#include "UtilsPython.hpp"

void Scripting::manejar_excepcion_python_libre(error_already_set& e,
                                         object &globals, object &locals)
{
    PyObject *type, *value, *traceback;
    // Save the error state because PyErr_Print() is going toclear
    // it. That's not what we want.
    PyErr_Fetch(&type, &value, &traceback);
    // But whoops, PyErr_Fetch() just cleared the exceptionflag! If
    // we now call PyErr_Print(), it thinks there's nothingwrong, and
    // doesn't print anything! Immediately restore the exceptionso
    // PyErr_Print() will see it.
    PyErr_Restore(type, value, traceback);
    // Okay, print the traceback to stderr...
    //PyErr_Print();

    //convertimos el error a string.
    object msg(handle<>(borrowed(value)));
    string mensaje;
    mensaje = extract<string>(msg);

    // then restore (again!) the original exception state.
    PyErr_Restore(type, value, traceback);

    throw ScriptMalo(mensaje);
}