#include "ModuloPython.hpp"
#include <boost/filesystem.hpp>

Scripting::ModuloPython::ModuloPython() {
  this->esta_extraida_clase = this->esta_cargado = false;
  this->ayudante = NULL;
}

Scripting::ModuloPython::ModuloPython(const ModuloPython &orig) {}

Scripting::ModuloPython::~ModuloPython() {
  if (this->ayudante != NULL)
    delete this->ayudante;
}

void Scripting::ModuloPython::cargar(std::string ruta, Reglas::Tablero &t) {
  using namespace std;
  namespace py = pybind11;

  if (this->esta_cargado)
    return;

  boost::filesystem::path ruta_path(ruta);
  py::dict locals;
  locals["ruta"] = ruta;
  try {
    this->modulo = py::module_::import("__main__");
    this->namespace_modulo = this->modulo.attr("__dict__");

    locals["ruta"] = ruta_path.parent_path().string();
    py::exec("sys.path.append(ruta)\n", this->namespace_modulo, locals);
  } catch (py::error_already_set &e) {
    this->manejar_excepcion_python(e);
  }

  try {
    this->modulo = py::module_::import(ruta_path.stem().string().c_str());
  } catch (py::error_already_set &e) {
    throw ScriptMalo("Archivo no encontrado o con errores de sintaxis.");
  }

  try {
    this->namespace_modulo = this->modulo.attr("__dict__");

    this->namespace_modulo["tablero"] =
        py::cast(&t, py::return_value_policy::reference);
    this->ayudante = new Reglas::AyudanteDeAgente(t);
    this->namespace_modulo["ayudante"] =
        py::cast(this->ayudante, py::return_value_policy::take_ownership);

    py::eval_file(ruta.c_str(), this->namespace_modulo);
  } catch (py::error_already_set &e) {
    this->manejar_excepcion_python(e);
  }
  this->esta_cargado = true;
}

Reglas::Agente *Scripting::ModuloPython::getAgente() {
  namespace py = pybind11;
  if (!this->esta_cargado)
    throw ModuloNoCargado();

  Reglas::Agente *a = NULL;
  AgentePythonWrapper *aWrap = NULL;
  try {
    this->extraer_clase();

    this->instancias_clase.push_front(this->agente_clase());

    a = this->instancias_clase.front().cast<Reglas::Agente *>();

    aWrap = new AgentePythonWrapper(a);
  } catch (py::error_already_set &e) {
    this->manejar_excepcion_python(e);
  }
  return aWrap;
}

void Scripting::ModuloPython::finalizar() {}

void Scripting::ModuloPython::manejar_excepcion_python(
    pybind11::error_already_set &e) {
  manejar_excepcion_python_libre(e, this->namespace_modulo,
                                 this->namespace_modulo);
}

void Scripting::ModuloPython::extraer_clase() {
  namespace py = pybind11;
  if (this->esta_extraida_clase)
    return;
  try {
    py::exec("l = dir()\n"
             "if l.count('Reglas') == 0: raise Error()\n"
             "clase = None\n"
             "for i in l:\n"
             "    res = eval(i)\n"
             "    if type(res) == type(Reglas.Agente):\n"
             "        clase = res\n"
             "        break\n"
             "if clase is None: raise Error()\n",
             this->namespace_modulo, this->namespace_modulo);
    this->agente_clase = this->namespace_modulo["clase"];
  } catch (py::error_already_set &e) {
    this->manejar_excepcion_python(e);
  }
  this->esta_extraida_clase = true;
}
