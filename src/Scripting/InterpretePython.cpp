#include "InterpretePython.hpp"
#include <boost/filesystem.hpp>

bool Scripting::InterpretePython::esta_pyiniciado = false;

Scripting::InterpretePython::InterpretePython() {
  this->esta_finalizado = false;
  this->guard_ = nullptr;
}

Scripting::InterpretePython::InterpretePython(const InterpretePython &orig) {}

Scripting::InterpretePython::~InterpretePython() {
  if (!this->esta_finalizado)
    this->finalizar();
}

void Scripting::InterpretePython::iniciar(Reglas::Tablero &t) {
  namespace py = pybind11;
  this->tablero = &t;

  // Construct scoped_interpreter exactly once (replaces Py_Initialize).
  // Guarded by the existing static bool — scoped_interpreter is NOT re-entrant.
  if (!InterpretePython::esta_pyiniciado) {
    this->guard_ = new py::scoped_interpreter();
    InterpretePython::esta_pyiniciado = true;
  }

  py::object modulo_main;
  py::object modulo_main_namespace;

  try {
    modulo_main = py::module_::import("__main__");
    modulo_main_namespace = modulo_main.attr("__dict__");

    py::exec("import sys, os, os.path\n"
             "sys.path.append('./lib')\n"
             "sys.path.append('../lib')\n"
             "sys.path.append('../../lib')\n"
             "import Reglas\n",
             modulo_main_namespace, modulo_main_namespace);
  } catch (py::error_already_set &e) {
    manejar_excepcion_python_libre(e, modulo_main_namespace,
                                   modulo_main_namespace);
  }
}

void Scripting::InterpretePython::finalizar() {
  std::map<std::string, Modulo *>::iterator it;
  for (it = this->modulos.begin(); it != this->modulos.end(); it++) {
    it->second->finalizar();
    delete it->second;
  }
  // D2: Don't finalize Python interpreter. Original code had Py_Finalize()
  // commented out — finalizing frees agent objects still referenced by
  // PartidaGrafica::SetJugadores. Match lifetime matches process lifetime.
  this->esta_finalizado = true;
}

Reglas::Agente *Scripting::InterpretePython::getAgente(std::string ruta) {
  if (!this->manejaModulo(ruta))
    return NULL;

  if (this->modulos.count(ruta) == 0)
    this->agregarModulo(ruta);

  return this->modulos.at(ruta)->getAgente();
}

void Scripting::InterpretePython::agregarModulo(std::string ruta) {
  Modulo *m = new ModuloPython();
  m->cargar(ruta, *this->tablero);
  this->modulos.insert(std::pair<std::string, Modulo *>(ruta, m));
}

bool Scripting::InterpretePython::manejaModulo(std::string ruta) {
  std::vector<std::string> subfixes;
  subfixes.push_back("py");

  boost::filesystem::path ruta_path(ruta);

  for (size_t i = 0; i < subfixes.size(); i++) {
    if (subfixes.at(i).compare(ruta_path.extension().string()) != 0) {
      return true;
    }
  }
  return false;
}
