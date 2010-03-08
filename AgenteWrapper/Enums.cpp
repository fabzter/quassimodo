#include <boost/python.hpp>
#include <Reglas/Enums.hpp>
using namespace boost::python;
using namespace Reglas;

void export_enums()
{
    enum_<TipoDeJugada>("TipoDeJugada")
    .value("BARRERA", BARRERA)
    .value("MOVIMIENTO", MOVIMIENTO)
    ;
    
    enum_<Direccion>("Direccion")
    .value("NORTE", NORTE)
    .value("ESTE", ESTE)
    .value("SUR", SUR)
    .value("OESTE", OESTE)
    ;

}
