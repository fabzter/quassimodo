#ifndef _INTERPRETEPYTHON_HPP
#define _INTERPRETEPYTHON_HPP
#include "Interprete.hpp"
#include "ModuloPython.hpp"
#include "UtilsPython.hpp"
#include <pybind11/embed.h>

namespace Scripting {
class InterpretePython : public Interprete {
public:
  InterpretePython();
  InterpretePython(const InterpretePython &orig);
  virtual ~InterpretePython();

  virtual void iniciar(Reglas::Tablero &t);
  virtual void finalizar();

  virtual Reglas::Agente *getAgente(std::string ruta);
  virtual bool manejaModulo(std::string ruta);

  static bool esta_pyiniciado;

private:
  Reglas::Tablero *tablero;
  bool esta_finalizado;

  /**
   * El intérprete de pybind11. Se construye una sola vez (no es reentrante)
   * y se destruye en finalizar(). Reemplaza el viejo Py_Initialize().
   */
  pybind11::scoped_interpreter *guard_;

protected:
  virtual void agregarModulo(std::string ruta);
};
} // namespace Scripting
#endif /* _INTERPRETEPYTHON_HPP */
