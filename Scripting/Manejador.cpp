/**
 */

#include "Manejador.hpp"
#include "InterpretePython.hpp"

Scripting::Manejador::Manejador()
{
    //Agregamos todos los interpretes a la lista
    this->interpretes.push_front(new InterpretePython());

    //los iniciamos!
    this->iniciarInterpretes();
}

Scripting::Manejador::Manejador(const Manejador& orig)
{
}

Scripting::Manejador::~Manejador()
{
    std::list<Interprete*>::iterator it;
    for(it = this->interpretes.begin(); it != this->interpretes.end(); it++)
    {
        (*it)->finalizar();
        delete (*it);
    }
}

void Scripting::Manejador::iniciarInterpretes()
{
    std::list<Interprete*>::iterator it;
    for(it = this->interpretes.begin(); it != this->interpretes.end(); it++)
    {
        (*it)->iniciar();
    }
}

Reglas::Agente* Scripting::Manejador::getAgente(std::string ruta)
{
    Reglas::Agente* agente = NULL;
    std::list<Interprete*>::iterator it;
    for(it = this->interpretes.begin(); it != this->interpretes.end(); it++)
    {
        agente = (*it)->getAgente(ruta);
        if(agente != NULL)
            break;
    }
    return agente;
}