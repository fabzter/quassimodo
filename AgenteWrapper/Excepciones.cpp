#include <boost/python.hpp>
#include <Reglas/Excepciones.cpp>
#include <iostream>

#include <string>


using namespace boost::python;
using namespace Reglas;
using namespace std;

PyObject *ExcepcionType = NULL;
PyObject *ParametrosMalosType = NULL;
void translate_excepcion(Excepcion const &e)
{
    assert(ExcepcionType != NULL);
    boost::python::object pythonExceptionInstance(e);
    PyErr_SetObject(ExcepcionType, pythonExceptionInstance.ptr());
}

void translate_parametros_malos(ParametrosMalos const &e)
{
    assert(ParametrosMalosType != NULL);
    boost::python::object pythonExceptionInstance(e);
    PyErr_SetObject(ParametrosMalosType, pythonExceptionInstance.ptr());
}

string Excepcion_to_str(Excepcion &e)
{
    string m = e.what();
    return m;
}


void export_excepciones()
{
    class_<Excepcion> ExcepcionClass("Excepcion")
    ;
    ExcepcionType = ExcepcionClass.ptr();
    register_exception_translator<Excepcion>(&translate_excepcion);
    ExcepcionClass.def("__str__", Excepcion_to_str);
    
    class_< ParametrosMalos, bases<Excepcion> > 
    ParametrosMalosClass("ParametrosMalos")
    ;
    ParametrosMalosType = ParametrosMalosClass.ptr();
    register_exception_translator<ParametrosMalos>(&translate_parametros_malos);
    
}
