#include <boost/python.hpp>
#include <Reglas/Celda.hpp>
#include <Reglas/Enums.hpp>
#include <sstream>

using namespace boost::python;
using namespace Reglas;
using namespace std;

string Celda_to_str(Celda &c)
{
    ostringstream strs;
    
    strs << c;
    
    return strs.str();
}

void export_celda()
{
    object celda =
    class_< Celda, bases<Pieza> >("Celda")
        .def(init<Jugada &>())
        
        .def("estaLibre", &Celda::estaLibre )
        
        .def("getHijo", &Celda::getHijo, 
        return_value_policy<reference_existing_object>() )
        
        .def("estaLibreDireccion", &Celda::estaLibreDireccion)
    ;
    
    celda.attr("__str__") = &Celda_to_str;
}
