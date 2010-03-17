#include <boost/python.hpp>
#include <Reglas/Tablero.hpp>
#include <Reglas/Agente.hpp>
#include <sstream>
using namespace boost::python;
using namespace Reglas;
using namespace std;

//esto se hace pues getCelda esta sobrecargada!
typedef const Celda& (Tablero::*getCelda_with_id)(int) const;

/*Esto es la pare 1 de un hack re feo para poder ver los atributos estaticos de
Tablero...*/
static const int Tablero_size_x = Tablero::size_x;
static const int Tablero_size_y = Tablero::size_y;
static const int Tablero_tam_barrera = Tablero::tam_barrera;

string Tablero_to_str(Tablero &tab)
{
    ostringstream out;
    
    Reglas::Tablero *tablero = (Reglas::Tablero*)&tab;
    
    for(int i = tablero->size_y - 1; i >= 0  ; i--){
        int k;
        int esta_libre;

        for(k = 0; k < tablero->size_x; k++){ //imprimimos la barrera Norte.
            esta_libre=
                    tablero->getCelda(k, i).estaLibreDireccion(Reglas::NORTE)?
                        1 : 0;
            out << ' ';
            esta_libre? out << ' ' : out << '_';
            out <<"  " ;
        }
        out << '\n';

        //aqui primero imprimimos la barrera oeste, luego la celda,  luego la barrera este.
        for(k = 0; k < tablero->size_x; k++)
        {
            esta_libre =
                    tablero->getCelda(k, i).estaLibreDireccion(Reglas::OESTE)?
                        1: 0;
            esta_libre? out << ' ' : out << '|';

            esta_libre =
                    tablero->getCelda(k, i).estaLibre()? 1: 0;
            out << esta_libre;

            esta_libre =
                    tablero->getCelda(k, i).estaLibreDireccion(Reglas::ESTE)?
                        1: 0;
            esta_libre? out << "  " : out << "| ";
        }
        out << '\n';

        //ahora imprimimos la disponibilidad de la barrera sur.
        for(k = 0; k < tablero->size_x; k++){
            esta_libre=
                    tablero->getCelda(k, i).estaLibreDireccion(Reglas::SUR)?
                        1 : 0;
            out << ' ';
            esta_libre? out << ' ' : out << '_';
            out <<"  " ;
        }
        out << '\n';
    }
    
    return out.str();
}

void export_tablero()
{
    object tablero = //<--parte 2
    class_<Tablero>("Tablero")
        .def("getPosicion", &Tablero::getPosicion, 
        return_value_policy<reference_existing_object>() )
        
        .def("getBarrerasColocadas", &Tablero::getBarrerasColocadas, 
        return_value_policy<copy_const_reference>() )
        
        .def("getCelda", getCelda_with_id(&Tablero::getCelda), 
        return_value_policy<reference_existing_object>() )
        
        .def("getMetas", &Tablero::getMetas, 
        return_value_policy<reference_existing_object>())

    ;
    //parte 3...v
    tablero.attr("size_x") = Tablero_size_x;
    tablero.attr("size_y") = Tablero_size_y;
    tablero.attr("tam_barrera") = Tablero_tam_barrera;
    
    tablero.attr("__str__") = &Tablero_to_str;
}

