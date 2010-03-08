/* 
 */

#include "ModuloPython.hpp"

Scripting::ModuloPython::ModuloPython()
{
    this->esta_cargado = false;
}

Scripting::ModuloPython::ModuloPython(const ModuloPython& orig)
{
}

Scripting::ModuloPython::~ModuloPython()
{
}

void Scripting::ModuloPython::cargar(std::string ruta)
{
    //obtenemos un nombre unico para el modulo.
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    strftime (buffer,80,"%I:%M%p.",timeinfo);

    this->nombre = ruta + buffer;

    //creamos el modulo
    this->modulo = boost::python::import(this->nombre.c_str());
    //obtenemos el diccionario del namespace del modulo
    this->namespace_modulo = this->modulo.attr("__dict__");

    //ejecutamos el archivo
    boost::python::object ignored =
            boost::python::exec_file(boost::python::str(ruta),
                                     this->namespace_modulo);
    
    this->esta_cargado = true;
}

Reglas::Agente* Scripting::ModuloPython::getAgente()
{
    if(!this->esta_cargado)
        throw ModuloNoCargado();
    boost::python::object get = this->namespace_modulo["get"];
    Reglas::Agente *a =
            boost::python::extract< Reglas::Agente* >( get.attr("__call__")() );

    return a;
}
void Scripting::ModuloPython::finalizar()
{
    
}