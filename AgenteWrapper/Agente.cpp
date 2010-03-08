#include <boost/python.hpp>
#include <Reglas/Tablero.hpp>
#include <Reglas/Agente.hpp>
using namespace boost::python;
using namespace Reglas;

struct AgenteWrapper: Agente, wrapper<Agente>
{
    public:
    Jugada siguienteJugada()
    {
        return this->get_override("siguienteJugada")();
    }
    void iniciar(const Tablero& t, int id)
    {
        this->get_override("iniciar")(t, id);
    }
    void terminar()
    {
        this->get_override("terminar")();
    }
};

void export_agente()
{
    class_<AgenteWrapper, boost::noncopyable>("Agente")
        .def("siguienteJugada", pure_virtual(&Agente::siguienteJugada) )
        .def("iniciar", pure_virtual(&Agente::iniciar) )
        .def("terminar", pure_virtual(&Agente::terminar) )
    ;
}

