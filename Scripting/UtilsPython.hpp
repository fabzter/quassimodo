/*
 */

#ifndef _UTILSPYTHON_HPP
#define	_UTILSPYTHON_HPP
#include <boost/python.hpp>
#include <boost/python/enum.hpp>
#include <boost/python/def.hpp>
#include <boost/python/module.hpp>
#include <boost/python/object.hpp>
#include <boost/python/import.hpp>

#include <string>
#include "Excepciones.hpp"

namespace Scripting
{
    using namespace std;
    using namespace boost::python;

    /**
     * Maneja las excepciones que nos llegan de python.
     * @param e la excepcion de python a manejar.
     * @param globals es el diccionario global sobre el cual se corre el código
     * de la excepción.
     * @param local es el diccionario local sobre el cual se corre el código
     * de la excepción.
     * @throws ScriptMalo SIEMPRE.
     */
    void manejar_excepcion_python_libre(error_already_set& e,
            object &globals, object &locals);
}


#endif	/* _UTILSPYTHON_HPP */

