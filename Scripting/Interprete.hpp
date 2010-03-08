/**
 */

#ifndef _INTERPRETE_HPP
#define	_INTERPRETE_HPP
#include <Reglas/Agente.hpp>
#include <map>
#include "Modulo.hpp"

namespace Scripting{
    /**
     * Esta clase es la interfaz que todos los interpretes deben seguir. Cualquier
     * interprete que se quiera implementar debe seguirla.
     */
class Interprete {
public:
    /**
     * Realiza la inicialización del interprete, la carga de la vm, etc.
     * @throws ErrorVM si falla la inicialización de la máquina virtual.
     */
    virtual void iniciar() = 0;
    /**
     * Finaliza la vm, libera recursos, libera modulos (llamando a su metodo
     * finalizar), etc.
     * @throws ErrorVM si falla en apagarse la máquina virtual.
     */
    virtual void finalizar() = 0;

    /**
     * Carga un Agente contenido en el archivo con la ruta indicada.
     * @param ruta es una string que indica el path completo del archivo donde
     * se encuentra definido el Agente.
     * @return NULL si el Interprete no puede manejar el tipo de archivo, un
     * apuntador a Agente si lo puede manejar y contiene la definición de un
     * Agente.
     * @throws ArchivoNoEncontrado si el archivo no puede ser accedido por
     * algún motivo.
     * @throws SinAgenteDefinido si no se encuentra una clase derivada de
     * Agente en el archivo.
     */
    virtual Reglas::Agente *getAgente(std::string ruta) = 0;
protected:
    /**
     * Método para homogeneizar el comportamiento de los Interpretes. Se llama
     * dentro de getAgente o en alguna actividad que requiera de un nuevo
     * Modulo, y agrega un módulo al Interprete actual.
     * @param ruta es una string que indica el path completo del archivo del
     * módulo.
     * @return true si el Interprete puede manejar el script, false de lo
     * contrario.
     */
    virtual void agregarModulo(std::string ruta) = 0;

    /**
     * Determina si el módulo en ruta pued ser manejado por el Interprete actual.
     * @param ruta es una string que indica el path completo del archivo del
     * módulo.
     */
    virtual bool manejaModulo(std::string ruta) = 0;

    /**
     * Esta es un mapa de los Modulos que actualmente está manejando el
     * Interprete. La clave es la ruta!
     */
    std::map<std::string, Modulo*> modulos;
};
}
#endif	/* _INTERPRETE_HPP */

