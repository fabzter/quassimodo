/* 
 */

#include "ModuloPython.hpp"

Scripting::ModuloPython::ModuloPython()
{
    this->esta_extraida_clase = this->esta_cargado = false;
}

Scripting::ModuloPython::ModuloPython(const ModuloPython& orig)
{
}

Scripting::ModuloPython::~ModuloPython()
{
}

void Scripting::ModuloPython::cargar(std::string ruta, Reglas::Tablero &t)
{
    if(this->esta_cargado)
        return;
    
    //creamos el modulo
    this->modulo = boost::python::import("__main__");

    //obtenemos el diccionario del namespace del modulo
    this->namespace_modulo = this->modulo.attr("__dict__");

//    >>> path.splitext(path.basename("/usr/hola.py"))
//      ('hola', '.py')
    //ejecutamos el archivo
    try
    {
        boost::python::object ignored =
            boost::python::exec_file(boost::python::str(ruta.c_str()),
                                     this->namespace_modulo,
                                     this->namespace_modulo);
    }
    catch(boost::python::error_already_set& e)
    {

        this->manejar_excepcion_python(e);
    }
    
    this->esta_cargado = true;
}

Reglas::Agente* Scripting::ModuloPython::getAgente()
{
    if(!this->esta_cargado)
        throw ModuloNoCargado();

    Reglas::Agente *a = NULL;
    try
    {
        this->extraer_clase();

        this->instancias_clase.push_front( this->agente_clase() );
        
        a = boost::python::extract<Reglas::Agente*>
                (this->instancias_clase.front());
    }
    catch(boost::python::error_already_set& e)
    {
        this->manejar_excepcion_python(e);
    }
    
    return a;
}
void Scripting::ModuloPython::finalizar()
{
    
}

void Scripting::ModuloPython::manejar_excepcion_python
                                          (boost::python::error_already_set& e)
{
    Scripting::manejar_excepcion_python_libre(e, this->namespace_modulo,
                                       this->namespace_modulo);
}

void Scripting::ModuloPython::extraer_clase()
{
    using namespace boost::python;
    if(this->esta_extraida_clase)
        return;
    try
    {
        object clase =
        exec("l = dir()\n"
             "if l.count('Reglas') == 0: raise Error()\n"
             "clase = None\n"
             "for i in l:\n"
             "    res = eval(i)\n"
             "    if type(res) == type(Reglas.Agente) and res not in clases_creadas:\n"
             "        clase = res\n"
             "        clases_creadas.append(res);break\n"
             "if clase is None: raise Error()\n"
                , this->namespace_modulo, this->namespace_modulo);
        this->agente_clase = this->namespace_modulo["clase"];
    }
    catch(error_already_set& e)
    {
        this->manejar_excepcion_python(e);
    }
    this->esta_extraida_clase = true;
}