#ifndef _MANEJADOR_HPP
#define _MANEJADOR_HPP
#include <list>
#include <string>
#include "Interprete.hpp"

namespace Scripting {

class Manejador {
public:
    Manejador(Reglas::Tablero &t);
    Manejador(const Manejador& orig);
    virtual ~Manejador();

    virtual Reglas::Agente *getAgente(std::string ruta);
private:
    void iniciarInterpretes();
    std::list<Interprete*> interpretes;
    Reglas::Tablero *tablero;
};

}
#endif
