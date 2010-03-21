/**
 */

#include "InterpretePython.hpp"

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

        exec("import sys; sys.path.append('../lib')\n"
                                "import Reglas\n"
                                "if 'clases_creadas' not in dir():\n"
                                "    clases_creadas = []\n",
                                modulo_main_namespace, modulo_main_namespace);

            //exponemos el Tablero
            modulo_main_namespace["tablero"] = object(boost::python::ptr(&t));
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
    std::string subfix = ".py";
    return (ruta.size() - ruta.rfind(subfix)) == 3;
}