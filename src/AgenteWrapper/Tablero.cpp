#include <boost/python.hpp>
#include <Reglas/Tablero.hpp>
#include <Reglas/Agente.hpp>
#include <Reglas/Barrera.hpp>
#include <sstream>
using namespace boost::python;
using namespace Reglas;
using namespace std;

//esto se hace pues getCelda esta sobrecargada!
typedef const Celda& (Tablero::*getCelda_with_id)(int) const;
typedef void (Tablero::*moverJugador_with_int)(int, int, int);
typedef void (Tablero::*setBarrera_with_int)(int, const Barrera&);

/*Esto es la pare 1 de un hack re feo para poder ver los atributos estaticos de
Tablero...*/
static const int Tablero_size_x = Tablero::size_x;
static const int Tablero_size_y = Tablero::size_y;
static const int Tablero_tam_barrera = Tablero::tam_barrera;

string print_tab(Tablero *tab)
{
    ostringstream strs;
    
    strs << *tab;
    
    return strs.str();
}

bool eq_tablero(Tablero * tab, Tablero *otro)
{
    return *tab == *otro;
}

void export_tablero()
{
    object tablero = //<--parte 2
    class_<Tablero>("Tablero")
        .def(init<const Tablero*>())
        
        .def("getPosicion", &Tablero::getPosicion, 
        return_value_policy<reference_existing_object>() )
        
        .def("getBarrerasColocadas", &Tablero::getBarrerasColocadas, 
        return_value_policy<copy_const_reference>() )
        
        .def("getCelda", getCelda_with_id(&Tablero::getCelda), 
        return_value_policy<reference_existing_object>() )
        
        .def("getMetas", &Tablero::getMetas, 
        return_value_policy<reference_existing_object>() )
        
        .def("moverJugador", moverJugador_with_int(&Tablero::moverJugador))
        
        .def("setBarrera", setBarrera_with_int(&Tablero::setBarrera))
        
        .def("__eq__", &eq_tablero)
        
        .def("__str__", &print_tab)

    ;
    //parte 3...v
    tablero.attr("size_x") = Tablero_size_x;
    tablero.attr("size_y") = Tablero_size_y;
    tablero.attr("tam_barrera") = Tablero_tam_barrera;
}

