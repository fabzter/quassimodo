#include <boost/python.hpp>
#include <Reglas/AyudanteDeAgente.hpp>
using namespace boost::python;
using namespace Reglas;
using namespace std;

void export_ayudante_de_agente()
{
    class_<AyudanteDeAgente>("AyudanteDeAgente", init<Tablero &>())
    .def("getMovimientosPosibles", &AyudanteDeAgente::getMovimientosPosibles)
    .def("getBarrerasPosibles", &AyudanteDeAgente::getBarrerasPosibles)
    ;
}
