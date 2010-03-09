/* 
 */

#include "ModuloPython.hpp"

Scripting::ModuloPython::ModuloPython()
{
    this->esta_cargado = false;
}

Scripting::ModuloPython::ModuloPython(const ModuloPython& orig)
{
}

Scripting::ModuloPython::~ModuloPython()
{
}

void Scripting::ModuloPython::cargar(std::string ruta)
{
    //creamos el modulo
    this->modulo = boost::python::import("__main__");

    //obtenemos el diccionario del namespace del modulo
    this->namespace_modulo = this->modulo.attr("__dict__");

    //ejecutamos el archivo
    try
    {
        boost::python::exec("import sys; sys.path.append('../lib')\n", 
                            this->namespace_modulo, this->namespace_modulo);

        boost::python::object ignored =
            boost::python::exec_file(boost::python::str(ruta.c_str()),
                                     this->namespace_modulo,
                                     this->namespace_modulo);
    }
    catch(boost::python::error_already_set& e)
    {
        this->manejar_excepcion_python(e);
        // Propagate the exception.
        throw;

    }
    
    this->esta_cargado = true;
}

Reglas::Agente* Scripting::ModuloPython::getAgente()
{
    if(!this->esta_cargado)
        throw ModuloNoCargado();

    Reglas::Agente *a = NULL;
    try
    {
        this->extraer_clase();
        boost::python::object clase_instancia = this->agente_clase();
        a = boost::python::extract<Reglas::Agente*>(clase_instancia);
        a->terminar();
    }
    catch(boost::python::error_already_set& e)
    {
        this->manejar_excepcion_python(e);
        // Propagate the exception.
        throw;
    }
    
    return a;
}
void Scripting::ModuloPython::finalizar()
{
    
}

int Scripting::ModuloPython::manejar_excepcion_python
                                          (boost::python::error_already_set& e)
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
    PyErr_Print();
    // then restore (again!) the original exception state.
    PyErr_Restore(type, value, traceback);
}

void Scripting::ModuloPython::extraer_clase()
{
    using namespace boost::python;
    //iteramos el diccionario de __main__
    boost::python::ssize_t n = len(this->namespace_modulo);
    object values = this->namespace_modulo.attr("values")();

    try
    {
        object clase = eval("Reglas.Agente.__class__", this->namespace_modulo,
                            this->namespace_modulo);
        for(boost::python::ssize_t i = 0; i < n; i++)
        {
            std::string cls_str = extract<std::string>(values[i].attr("__class__"));
            if(values[i].attr("__class__") == clase)
            {
                this->agente_clase = values[i];
                return;
            }
        }
    }
    catch(boost::python::error_already_set& e)
    {
        this->manejar_excepcion_python(e);
        throw ScriptMalo();
    }

    throw ScriptMalo();
}