/**
 */

#ifndef _MODULO_HPP
#define _MODULO_HPP
#include <Reglas/Agente.hpp>
#include <string>

namespace Scripting {
/**
 * Esta clase es la interfaz que todo Módulo dependiente de un Interprete
 * especifico debe cumplir.
 */
class Modulo {
public:
  virtual ~Modulo() = default;

  /**
   * Carga módulo que esta en el archivo indicado por ruta. Actualiza la bandera
   * esta_cargado, y le asigna un nombre al modulo. Este nombre debe ser unico
   * pues será usado como nombre del ambiente en que será ejecutado dicho
   * módulo.
   * @param ruta es una string que indica la localización del archivo.
   * @param t es una referencia al Tablero de la Partida, para poder exponérsela
   * al Modulo.
   * @throws ArchivoNoEncontrado si el archivo no puede ser accedido por
   * algún motivo.
   */
  virtual void cargar(std::string ruta, Reglas::Tablero &t) = 0;

  /**
   * Nos entrega un apuntador a una instancia de una clase derivada de Agente
   * por el script.
   * @return NULL si el script no tiene una clase derivada de Agente, y un
   * apuntador a Agente si si lo tiene.
   */
  virtual Reglas::Agente *getAgente() = 0;

  /**
   * Libera los recursos usados por el Modulo.
   */
  virtual void finalizar() = 0;

protected:
  /**
   * Bandera que indica si ya ha sido cargado el script.
   */
  bool esta_cargado;

  /**
   * Es el nombre del ambiente en que se ejecutará el script.
   */
  std::string nombre;
};
} // namespace Scripting

#endif /* _MODULO_HPP */
