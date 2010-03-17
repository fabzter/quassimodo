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
    this->tablero = &t;
    Py_Initialize();
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