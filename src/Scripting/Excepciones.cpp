#include "Excepciones.hpp"

Scripting::Excepcion::Excepcion()
{
}
Scripting::Excepcion::Excepcion(std::string what)
{
    this->msg = what;
}
const char* Scripting::Excepcion:: what() const throw()
{
        return msg.c_str();
}
Scripting::Excepcion::~Excepcion() throw()
{
}

Scripting::ModuloNoCargado::ModuloNoCargado() :Excepcion()
{
}
Scripting::ModuloNoCargado::ModuloNoCargado(std::string what)
: Excepcion(what)
{
}

Scripting::ScriptMalo::ScriptMalo() :Excepcion()
{
}
Scripting::ScriptMalo::ScriptMalo(std::string what)
: Excepcion(what)
{
}