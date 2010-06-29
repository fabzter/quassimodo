#include "InterpretePython.hpp"
#include <boost/filesystem.hpp>

Scripting::InterpretePython::InterpretePython()
{
}

Scripting::InterpretePython::InterpretePython(const InterpretePython& orig)
{
}

Scripting::InterpretePython::~InterpretePython()
{
}

void Scripting::InterpretePython::iniciar(Reglas::Tablero &t)
{
    using namespace boost::python;
    this->tablero = &t;
    Py_Initialize();

    //configuramos inicialmente el interprete e python.
    object modulo_main;
    object modulo_main_namespace;
    
    try
    {
        modulo_main = import("__main__");
        modulo_main_namespace = modulo_main.attr("__dict__");

        exec("import sys, os, os.path\n"
             "sys.path.append('../lib')\n"
             "sys.path.append('../../lib')\n"
             "import Reglas\n", modulo_main_namespace, modulo_main_namespace);
    }
    catch(error_already_set& e)
    {
        manejar_excepcion_python_libre(e, modulo_main_namespace, 
                                       modulo_main_namespace);
    }
}

void Scripting::InterpretePython::finalizar()
{
    //finalizamos todos los modulos.
    std::map<std::string, Modulo*>::iterator it;
    for(it = this->modulos.begin(); it != this->modulos.end(); it++)
    {
        it->second->finalizar();
        delete it->second;
    }
     //Py_Finalize();
}

Reglas::Agente *Scripting::InterpretePython::getAgente(std::string ruta)
{
    if(!this->manejaModulo(ruta))
        return NULL;
    
    if(this->modulos.count(ruta) == 0)
        this->agregarModulo(ruta);

    return this->modulos.at(ruta)->getAgente();
}

void Scripting::InterpretePython::agregarModulo(std::string ruta)
{
    Modulo * m = new ModuloPython();
    m->cargar(ruta, *this->tablero);
    this->modulos.insert( std::pair<std::string, Modulo*>(ruta, m) );
}

bool Scripting::InterpretePython::manejaModulo(std::string ruta)
{
    std::vector<std::string> subfixes;
    subfixes.push_back("py");
    subfixes.push_back("pyc");

    boost::filesystem::path ruta_path(ruta);

    for(int i = 0; i < subfixes.size(); i++)
    {
        if(subfixes.at(i).compare(ruta_path.extension()))
        {
            return true;
        }
    }
    return false;
}