/*
 */

#include "UtilsPython.hpp"

void Scripting::manejar_excepcion_python_libre(
                                            boost::python::error_already_set& e,
                                            boost::python::object &globals,
                                            boost::python::object &locals)
{
    using namespace std;
    using namespace boost::python;

    string mensaje("");
    PyObject *type, *value, *traceback;

    PyErr_Fetch(&type, &value, &traceback);

    try
    {
        handle<> hexc(type),hval(value),htb(traceback);
        if(!htb || !hval)
        {
            mensaje = extract<string>(str(hexc));
        }
        else
        {
            object traceback(import("traceback"));
            object format_exception(traceback.attr("format_exception"));
            object formatted_list(format_exception(hexc,hval,htb));
            object formatted(str("\n").join(formatted_list));
            mensaje = extract<string>(formatted);
        }
    }
    catch(boost::python::error_already_set)
    {
        object modulo_main = import("__main__");
        object modulo_main_namespace = modulo_main.attr("__dict__");
        exec("import traceback\nimport cStringIO\n"
             "fout = cStringIO.StringIO()\n"
             "traceback.print_exc(file=fout)\n"
             "fout = fout.getvalue()\n",
             modulo_main_namespace, modulo_main_namespace);

        mensaje = extract<string>( eval("fout", modulo_main_namespace,
                                                       modulo_main_namespace) );
    }
    
    throw ScriptMalo(mensaje);
}