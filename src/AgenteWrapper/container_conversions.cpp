#include "container_conversions.h"
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/implicit.hpp>
#include <boost/python/suite/indexing/indexing_suite.hpp>
#include <boost/python/suite/indexing/container_utils.hpp>

#include <Reglas/Tablero.hpp>
#include <Reglas/Jugada.hpp>
#include <Reglas/Barrera.hpp>
#include <Reglas/Celda.cpp>
#include <list>
#include <map>
using namespace boost::python;
using namespace Reglas;

void export_container_conversions()
{

/*lista de Barrera*/
to_python_converter<
      std::list<Barrera>,
      to_tuple<
        std::list<Barrera> > >();
        
from_python_sequence<
      std::list<Barrera>,
      linked_list_policy>();
      
/*lista de Jugada*/
to_python_converter<
      std::list<Jugada>,
      to_tuple<
        std::list<Jugada> > >();
        
from_python_sequence<
      std::list<Jugada>,
      linked_list_policy>();

/*Vectores*/
to_python_converter<
      std::vector<int>,
      to_tuple<
        std::vector<int> > >();
/*Segunda solucion :S*/
class_<std::vector<int> >("IntVector")
        .def(vector_indexing_suite<std::vector<int> >())
;
from_python_sequence<
      std::vector<int>,
      variable_capacity_policy>();

//vectores de celdas.
class_<std::vector<Celda> >("CeldaVector")
        .def(vector_indexing_suite<std::vector<Celda> >())
;
class_<std::vector<Celda*> >("CeldaPtVector")
        .def(vector_indexing_suite<std::vector<Celda*> >())
;

//Vectores de Jugadas.
class_<std::vector<Jugada> >("JugadaVector")
        .def(vector_indexing_suite<std::vector<Jugada> >())
;

}
