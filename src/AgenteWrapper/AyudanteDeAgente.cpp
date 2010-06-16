#include <boost/python.hpp>
#include <Reglas/AyudanteDeAgente.hpp>
using namespace boost::python;
using namespace Reglas;
using namespace std;

std::vector<Celda*> __astar__(Tablero *t, int idJugador)
{
    std::vector<void*>* path_void = astar(t, idJugador);
    std::vector<Celda*> path_celda;
    
    for(int i = 0; i < path_void->size(); i++)
    {
        path_celda.push_back( (Celda*)(path_void->at(i)) );
    }
    
    delete path_void;
    
    return path_celda;
}

void export_ayudante_de_agente()
{
    class_<AyudanteDeAgente>("AyudanteDeAgente", init<Tablero *>())
    .def("getMovimientosPosibles", &AyudanteDeAgente::getMovimientosPosibles)
    .def("getBarrerasPosibles", &AyudanteDeAgente::getBarrerasPosibles)
    .def("hayGanador", &AyudanteDeAgente::hayGanador)
    ;
    
    def("astar", &__astar__);
}
