#include <boost/python.hpp>
#include <Reglas/Pieza.hpp>
#include <Reglas/Barrera.hpp>

using namespace boost::python;
using namespace Reglas;
//esto se hace pues colocar esta sobrecargada!
typedef void (Barrera::*colocar_con_vect_y_id)(const std::vector<int>& pos, Direccion d);
typedef void (Barrera::*colocar_con_int_y_id)(int x, int y, Direccion d);

void export_barrera()
{
    class_< Barrera, bases<Pieza> >("Barrera")
    
        .def("colocar", colocar_con_vect_y_id(&Barrera::colocar) )
    
        .def("colocar", colocar_con_int_y_id(&Barrera::colocar) )
        
        .def("getDireccion", &Barrera::getDireccion )
        
        .def("getPuntoMedio", &Barrera::getPuntoMedio,
        return_value_policy<reference_existing_object>() )
        
        .def("getPunta", &Barrera::getPunta,
        return_value_policy<reference_existing_object>() )
        
        .def(self == self)
    ;
}
