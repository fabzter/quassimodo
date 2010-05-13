#include <boost/python.hpp>

void export_pieza();
void export_agente();
void export_container_conversions();
void export_tablero();
void export_celda();
void export_enums();
void export_barrera();
void export_excepciones();
void export_jugada();
void export_ayudante_de_agente();

using namespace boost::python;

BOOST_PYTHON_MODULE(Reglas)
{
    export_container_conversions();
    export_jugada();
    export_excepciones();
    export_agente();
    export_tablero();
    export_pieza();
    export_celda();
    export_enums();
    export_barrera();
    export_ayudante_de_agente();
}

