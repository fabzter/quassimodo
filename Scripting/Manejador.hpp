/**
 */

#ifndef _MANEJADOR_HPP
#define	_MANEJADOR_HPP
#include <python2.6/Python.h>
#include <boost/python.hpp>
#include <list>
#include <string>
#include "Interprete.hpp"

/**
 * Este namespace contiene todas las clases, enums, etc, que se relacionan con
 * la carga de modulos de scripts, como lo son los Agentes. Su clase fachada es
 * la clase Manejador.
 */
namespace Scripting{
using namespace boost;

/**
 * Es la clase que sirve como interfaz a todos los servicios de Scripting. Tiene
 * maneja a cada interprete de manera independiente.
 */
class Manejador {
public:
    /**
     * El constructor de la clase Manejador agrega a la lista todos los interpretes
     * que éste tiene, y despues los inicializa.
     */
    Manejador();
    Manejador(const Manejador& orig);
    /**
     * El destructor se encarga de finalizar todos los interpretes y borrarlos
     * de la memoria.
     */
    virtual ~Manejador();

    virtual Reglas::Agente *getAgente(std::string ruta);
private:
    /**
     * Recorre la lista de interpretes y llama al método iniciar de cada uno.
     */
    void iniciarInterpretes();

    /**
     * Una lista de apuntadores a todos los interpretes que son manejados.
     */
    std::list<Interprete*> interpretes;
};
}
#endif	/* _MANEJADOR_HPP */

