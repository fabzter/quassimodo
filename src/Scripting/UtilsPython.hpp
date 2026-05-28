#ifndef _UTILSPYTHON_HPP
#define _UTILSPYTHON_HPP
#include <pybind11/embed.h>
#include <string>
#include "Excepciones.hpp"

namespace Scripting
{
    void manejar_excepcion_python_libre(pybind11::error_already_set& e,
            pybind11::object &globals, pybind11::object &locals);
}

#endif
