#include "Excepciones.hpp"

Reglas::Excepcion::Excepcion()
{
}
Reglas::Excepcion::Excepcion(std::string what)
{
    this->msg = what;
}
const char* Reglas::Excepcion:: what() const throw()
{
        return msg.c_str();
}
Reglas::Excepcion::~Excepcion() throw()
{
}

Reglas::PiezaFueraDelTablero::PiezaFueraDelTablero() :Excepcion()
{
}
Reglas::PiezaFueraDelTablero::PiezaFueraDelTablero(std::string what)
: Excepcion(what)
{
}

Reglas::CeldaFueraDelTablero::CeldaFueraDelTablero() :PiezaFueraDelTablero()
{
}
Reglas::CeldaFueraDelTablero::CeldaFueraDelTablero(std::string what)
: PiezaFueraDelTablero(what)
{
}

Reglas::PartidaTerminada::PartidaTerminada() :Excepcion()
{
}
Reglas::PartidaTerminada::PartidaTerminada(std::string what) : Excepcion(what)
{
}

Reglas::PartidaNoIniciada::PartidaNoIniciada() :Excepcion()
{
}
Reglas::PartidaNoIniciada::PartidaNoIniciada(std::string what) : Excepcion(what)
{
}

Reglas::ReglasRotas::ReglasRotas() :Excepcion()
{
}
Reglas::ReglasRotas::ReglasRotas(std::string what) : Excepcion(what)
{
}


Reglas::ParametrosMalos::ParametrosMalos() :Excepcion()
{
}
Reglas::ParametrosMalos::ParametrosMalos(std::string what) : Excepcion(what)
{
}

Reglas::PiezaYaColocada::PiezaYaColocada() : Excepcion()
{
    
}
Reglas::PiezaYaColocada::PiezaYaColocada(std::string what) : Excepcion(what)
{

}

Reglas::PiezaNoColocada::PiezaNoColocada() : Excepcion()
{

}
Reglas::PiezaNoColocada::PiezaNoColocada(std::string what) : Excepcion(what)
{

}

Reglas::JugadorNoColocado::JugadorNoColocado() : PiezaNoColocada()
{

}
Reglas::JugadorNoColocado::JugadorNoColocado(std::string what) :
PiezaNoColocada(what)
{

}

Reglas::SinHijo::SinHijo() : Excepcion()
{

}
Reglas::SinHijo::SinHijo(std::string what) :
Excepcion(what)
{

}