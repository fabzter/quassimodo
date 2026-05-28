/*
 */

#ifndef _UTILSPYTHON_HPP
#define _UTILSPYTHON_HPP
#include "Excepciones.hpp"
#include <pybind11/embed.h>
#include <string>

namespace Scripting {
/**
 * Maneja las excepciones que nos llegan de python.
 * @param e la excepcion de python a manejar.
 * @param globals es el diccionario global sobre el cual se corre el código
 * de la excepción.
 * @param locals es el diccionario local sobre el cual se corre el código
 * de la excepción.
 * @throws ScriptMalo SIEMPRE.
 */
void manejar_excepcion_python_libre(pybind11::error_already_set &e,
                                    pybind11::object &globals,
                                    pybind11::object &locals);
} // namespace Scripting

#endif /* _UTILSPYTHON_HPP */
