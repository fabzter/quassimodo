#include "container_conversions.h"
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/implicit.hpp>
#include <boost/python/suite/indexing/indexing_suite.hpp>
#include <boost/python/suite/indexing/container_utils.hpp>

#include <Reglas/Barrera.hpp>

using namespace boost::python;
using namespace Reglas;

void export_container_conversions()
{
/*lista de enteros*/
to_python_converter<
      std::list<int>,
      to_tuple<
        std::list<int> > >();

from_python_sequence<
      std::list<int>,
      linked_list_policy>();

/*lista de Barrera*/
to_python_converter<
      std::list<Barrera>,
      to_tuple<
        std::list<Barrera> > >();
        
from_python_sequence<
      std::list<Barrera>,
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


}
