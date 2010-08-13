#ifndef FABRICAAGENTE_HPP
#define	FABRICAAGENTE_HPP
#include <Reglas/Agente.hpp>
#include <functional>

namespace Agentes
{

/**
 * Interfaz para crear una clase Fábrica de Agentes. Cada que se implemente un
 * Agente, se debe también implementar una clase fábrica que sirva para crear
 * Agentes del mismo tipo.
 */
class FabricaAgente
{
public:
    /**
     * El destructor default sólo se agrega para poder eliminar la basura de
     * manera correcta.
     */
    virtual ~FabricaAgente(){};
    /**
     * Debe regresar un apuntador a un nuevo Agente creado, y delegar la propiedad
     * a quien llamó el método.
     * @return Un apuntador a un nuevo Agente. Delega la responsabilidad de borrar
     * el objeto al llamador.
     */
    virtual Reglas::Agente* operator()() = 0;
private:

};

}
#endif	/* FABRICAAGENTE_HPP */

