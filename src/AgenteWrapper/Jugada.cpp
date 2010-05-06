#include <boost/python.hpp>
#include <Reglas/Jugada.hpp>
#include <Reglas/Enums.hpp>

using namespace boost::python;
using namespace Reglas;
using namespace std;

typedef void (Jugada::*setPosicion_with_int)(int x, int y);
typedef void (Jugada::*setPosicion_with_vect)(const std::vector<int>& pos);

void export_jugada()
{
    class_<Jugada>("Jugada")
        .def("setPosicion", setPosicion_with_int(&Jugada::setPosicion) )
        .def("setPosicion", setPosicion_with_vect(&Jugada::setPosicion) )
        
        .def("getPosicion", &Jugada::getPosicion, 
                            return_value_policy<reference_existing_object>() )
        .def("setTipoDeJugada", &Jugada::setTipoDeJugada)
        .def("getTipoDeJugada", &Jugada::getTipoDeJugada)
        .def("setDireccion", &Jugada::setDireccion)
        .def("getDireccion", &Jugada::getDireccion)
    ;
}
