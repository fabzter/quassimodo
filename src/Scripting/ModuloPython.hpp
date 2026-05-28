/*
 */

#ifndef _MODULOPYTHON_HPP
#define _MODULOPYTHON_HPP
#include "Modulo.hpp"
#include <Reglas/Agente.hpp>
#include <Reglas/AyudanteDeAgente.hpp>

#include <list>
#include <pybind11/embed.h>
#include <string>

#include "AgentePythonWrapper.hpp"
#include "Excepciones.hpp"
#include "UtilsPython.hpp"

namespace Scripting {

namespace py = pybind11;

class ModuloPython : public Modulo {
public:
  ModuloPython();
  ModuloPython(const ModuloPython &orig);
  virtual ~ModuloPython();

  virtual void cargar(std::string ruta, Reglas::Tablero &t);
  virtual Reglas::Agente *getAgente();
  virtual void finalizar();

private:
  /**
   * El módulo de pybind11.
   */
  py::object modulo;

  /**
   * El namespace del modulo.
   */
  py::object namespace_modulo;

  /**
   * Este objeto mantiene LA CLASE derivada de Agente después de que se cargó
   * el Modulo.
   */
  py::object agente_clase;

  /**
   * Nos indica si la clase ya fue extraida para evitar intentar extraerla de
   * nuevo.
   */
  bool esta_extraida_clase;

  /**
   * Aqui se guardan apuntadores a las instancias creadas.
   */
  std::list<py::object> instancias_clase;

  Reglas::AyudanteDeAgente *ayudante;

  /**
   * Extrae la clase Derivada de Agente del Modulo.
   * @throws ModuloNoCargado si se intenta llamar antes de haber cargado el
   * Modulo.
   * @throws ScriptMalo si el Modulo no contiene una clase derivada de Agente.
   */
  void extraer_clase();

  /**
   * Manejo interno de las excepciones del Modulo.
   */
  void manejar_excepcion_python(py::error_already_set &e);
};

} // namespace Scripting
#endif /* _MODULOPYTHON_HPP */
