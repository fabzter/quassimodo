
#include "Exepciones.hpp"

Grafico::Excepcion::Excepcion()
{
}
Grafico::Excepcion::Excepcion(std::string what)
{
    this->msg = what;
}
const char* Grafico::Excepcion:: what() const throw()
{
        return msg.c_str();
}
Grafico::Excepcion::~Excepcion() throw()
{
}

Grafico::SkinNoCargado::SkinNoCargado() :Excepcion()
{
}
Grafico::SkinNoCargado::SkinNoCargado(std::string what)
: Excepcion(what)
{
}


