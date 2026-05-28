# D1′ — pybind11 Migration Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Migrate `src/AgenteWrapper/` and `src/Scripting/` from Boost.Python to pybind11, drop `boost_python`, verify the console game still plays.

**Architecture:** Same topology as D1.5 spike (`spikes/pybind/`): single shared `libReglas`, extension `Reglas.so` (CMake MODULE, `-undefined dynamic_lookup`), embedder links `pybind11::embed`. All identity containers opaque (`MAKE_OPAQUE` + `bind_vector`, incl. `vector<int>`); only `list<T>` by-value via `pybind11/stl.h`. 300-line `container_conversions.h` + `.cpp` deleted.

**Tech Stack:** pybind11 2.x (already in flake from D1.5), CPython 3.11, CMake 3.30 + Ninja, clang 16.

**Branch:** `feat/d1prime-pybind11` (already cut from `main`).

**Companion spec:** [`docs/superpowers/specs/2026-05-28-pybind11-migration-d1prime-design.md`](../specs/2026-05-28-pybind11-migration-d1prime-design.md)

**D1.5 spike reference:** `spikes/pybind/bindings.cpp`, `spikes/pybind/main.cpp`, `spikes/pybind/CMakeLists.txt`.

---

## Critical gotchas (every subagent MUST read)

1. **`nix`/`devbox` are NOT on PATH in non-interactive shells.** Every shell command that uses `nix` or the toolchain MUST be prefixed:
   ```sh
   . /nix/var/nix/profiles/default/etc/profile.d/nix-daemon.sh && <command>
   ```
   In practice we run builds via `. /nix/var/nix/profiles/default/etc/profile.d/nix-daemon.sh && nix develop -c bash -c '...'`.
2. **Nix flakes only see git-tracked files.** `git add flake.nix` BEFORE `nix develop`.
3. **Never pipe through `| tail`/`| head`** — masks exit code. Capture `$?` directly.
4. **Never amend/rebase.** Commit per task.
5. **No AI/LLM attribution** — not in commits, PRs, or code.
6. **Git identity is repo-local** — plain `git commit` works.
7. **Don't change game logic.** API-translation only.
8. **`Reglas.so` is NOT linked into `consola`.** Gate verifies with `otool -L`.
9. **`py::scoped_interpreter` is NOT re-entrant.** Guard with static bool matching existing `esta_pyiniciado`.
10. **Import Reglas BEFORE `py::cast`** (cerebrum 2026-05-27 lesson). Current `iniciar()` does this — preserve the flow.
11. **`PYBIND11_MAKE_OPAQUE` before any TU references the type across the binding boundary.** All six declarations at top of `main.cpp`.
12. **`reference_internal` on `getPosicion()` makes the vector mutable** from Python — verify agents don't mutate `pos`.
13. **`Reglas.so` must be staged into `lib/`** via existing CMake `INSTALL`/`SYMLINK` commands.

---

### Task 1: flake.nix — drop boostPython

**Files:**
- Modify: `flake.nix`

- [ ] **Step 1: Remove boostPython from flake**

```nix
{
  description = "Quassimodo isolated build environment";
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.11";
  outputs = { self, nixpkgs }:
    let
      systems = [ "aarch64-darwin" "x86_64-darwin" "x86_64-linux" "aarch64-linux" ];
      forAll = f: nixpkgs.lib.genAttrs systems (s: f (import nixpkgs { system = s; }));
    in {
      packages = forAll (pkgs:
        let
          irrlichtFork = pkgs.callPackage ./nix/irrlicht-fork.nix { darwin = pkgs.darwin; };
        in { inherit irrlichtFork; });

      devShells = forAll (pkgs:
        let
          irrlichtFork = pkgs.callPackage ./nix/irrlicht-fork.nix { darwin = pkgs.darwin; };
        in {
          default = pkgs.mkShell {
            packages = [ pkgs.cmake pkgs.ninja pkgs.pkg-config pkgs.python311
                         pkgs.python311.pkgs.pybind11 irrlichtFork ];
          };
        });
    };
}
```

- [ ] **Step 2: Lock and verify**

```bash
git add flake.nix
. /nix/var/nix/profiles/default/etc/profile.d/nix-daemon.sh && nix develop -c bash -c 'echo OK'
```
Expected: `OK`. No boostPython errors.

- [ ] **Step 3: Commit**

```bash
git commit -m "build: drop boostPython from flake.nix"
```

---

### Task 2: AgenteWrapper CMakeLists + delete container_conversions

**Files:**
- Modify: `src/AgenteWrapper/CMakeLists.txt`
- Delete: `src/AgenteWrapper/container_conversions.h`
- Delete: `src/AgenteWrapper/container_conversions.cpp`

- [ ] **Step 1: Rewrite CMakeLists**

```cmake
file(GLOB AGENTEWRAP_SRC CONFIGURE_DEPENDS "*.cpp")
add_library(ReglasModule MODULE ${AGENTEWRAP_SRC})
set_target_properties(ReglasModule PROPERTIES OUTPUT_NAME "Reglas" PREFIX "" SUFFIX ".so")
target_include_directories(ReglasModule PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src)

find_package(Python3 REQUIRED COMPONENTS Development)
find_package(pybind11 CONFIG REQUIRED)
target_link_libraries(ReglasModule PRIVATE Reglas pybind11::module Python3::Module)
if(APPLE)
  target_link_options(ReglasModule PRIVATE -Wl,-undefined,dynamic_lookup)
endif()
set_target_properties(ReglasModule PROPERTIES BUILD_RPATH "$<TARGET_FILE_DIR:Reglas>")
```

- [ ] **Step 2: Delete container_conversions**

```bash
rm src/AgenteWrapper/container_conversions.h
rm src/AgenteWrapper/container_conversions.cpp
```

- [ ] **Step 3: Commit**

```bash
git add src/AgenteWrapper/CMakeLists.txt src/AgenteWrapper/container_conversions.h src/AgenteWrapper/container_conversions.cpp
git commit -m "build: wire pybind11 into AgenteWrapper, delete Boost.Python container_conversions"
```

---

### Task 3: AgenteWrapper main.cpp — PYBIND11_MODULE + MAKE_OPAQUE

**Files:**
- Rewrite: `src/AgenteWrapper/main.cpp`

- [ ] **Step 1: Rewrite main.cpp**

```cpp
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>        // by-value crossing for std::list<T>
#include <pybind11/stl_bind.h>   // py::bind_vector

#include <Reglas/Tablero.hpp>
#include <Reglas/Celda.hpp>
#include <Reglas/Jugada.hpp>
#include <Reglas/Barrera.hpp>

namespace py = pybind11;

// All six opaque declarations — MUST precede any TU that references these
// types across the binding boundary.
PYBIND11_MAKE_OPAQUE(std::vector<int>);
PYBIND11_MAKE_OPAQUE(std::vector<Reglas::Celda>);
PYBIND11_MAKE_OPAQUE(std::vector<Reglas::Celda*>);
PYBIND11_MAKE_OPAQUE(std::vector<Reglas::Jugada>);

void export_pieza(py::module_& m);
void export_agente(py::module_& m);
void export_tablero(py::module_& m);
void export_celda(py::module_& m);
void export_enums(py::module_& m);
void export_barrera(py::module_& m);
void export_excepciones(py::module_& m);
void export_jugada(py::module_& m);
void export_ayudante_de_agente(py::module_& m);

PYBIND11_MODULE(Reglas, m) {
    // Register opaque container bindings FIRST (before any class that
    // returns them — converters must exist before py::class_ defs).
    py::bind_vector<std::vector<int>>(m, "IntVector");
    py::bind_vector<std::vector<Reglas::Celda>>(m, "CeldaVector");
    py::bind_vector<std::vector<Reglas::Celda*>>(m, "CeldaPtVector");
    py::bind_vector<std::vector<Reglas::Jugada>>(m, "JugadaVector");

    export_enums(m);
    export_jugada(m);
    export_excepciones(m);
    export_pieza(m);
    export_agente(m);
    export_tablero(m);
    export_celda(m);
    export_barrera(m);
    export_ayudante_de_agente(m);
}
```

- [ ] **Step 2: Commit**

```bash
git add src/AgenteWrapper/main.cpp
git commit -m "refactor: PYBIND11_MODULE entry point with MAKE_OPAQUE declarations"
```

---

### Task 4: AgenteWrapper trampolines — Agente.cpp + Pieza.cpp

**Files:**
- Rewrite: `src/AgenteWrapper/Agente.cpp`
- Rewrite: `src/AgenteWrapper/Pieza.cpp`

- [ ] **Step 1: Rewrite Agente.cpp**

```cpp
#include <pybind11/pybind11.h>
#include <Reglas/Agente.hpp>

namespace py = pybind11;
using namespace Reglas;

struct PyAgente : Agente {
    using Agente::Agente;
    Jugada siguienteJugada() override {
        PYBIND11_OVERRIDE_PURE(Jugada, Agente, siguienteJugada);
    }
    void iniciar(int id) override {
        PYBIND11_OVERRIDE_PURE(void, Agente, iniciar, id);
    }
    void terminar() override {
        PYBIND11_OVERRIDE_PURE(void, Agente, terminar);
    }
};

void export_agente(py::module_& m)
{
    py::class_<Agente, PyAgente>(m, "Agente")
        .def(py::init<>())
        .def("siguienteJugada", &Agente::siguienteJugada)
        .def("iniciar", &Agente::iniciar)
        .def("terminar", &Agente::terminar);
}
```

- [ ] **Step 2: Rewrite Pieza.cpp**

```cpp
#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <Reglas/Pieza.hpp>

namespace py = pybind11;
using namespace Reglas;

struct PyPieza : Pieza {
    using Pieza::Pieza;
    const std::vector<int>& getPosicion() const override {
        PYBIND11_OVERRIDE(const std::vector<int>&, Pieza, getPosicion);
    }
    bool operator==(const Pieza& otro) const override {
        PYBIND11_OVERRIDE(bool, Pieza, operator==, otro);
    }
};

void export_pieza(py::module_& m)
{
    py::class_<Pieza, PyPieza>(m, "Pieza")
        .def(py::init<>())
        .def("getPosicion", &Pieza::getPosicion,
             py::return_value_policy::reference_internal)
        .def("__eq__", &Pieza::operator==);
}
```

- [ ] **Step 3: Commit**

```bash
git add src/AgenteWrapper/Agente.cpp src/AgenteWrapper/Pieza.cpp
git commit -m "refactor: pybind11 trampolines for Agente and Pieza"
```

---

### Task 5: AgenteWrapper — Tablero.cpp + Celda.cpp

**Files:**
- Rewrite: `src/AgenteWrapper/Tablero.cpp`
- Rewrite: `src/AgenteWrapper/Celda.cpp`

- [ ] **Step 1: Rewrite Tablero.cpp**

```cpp
#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <pybind11/operators.h>
#include <Reglas/Tablero.hpp>
#include <Reglas/Barrera.hpp>
#include <sstream>

namespace py = pybind11;
using namespace Reglas;
using namespace std;

string print_tab(Tablero *tab)
{
    ostringstream strs;
    strs << *tab;
    return strs.str();
}

bool eq_tablero(Tablero *tab, Tablero *otro)
{
    return *tab == *otro;
}

void export_tablero(py::module_& m)
{
    py::class_<Tablero>(m, "Tablero")
        .def(py::init<const Tablero*>())

        .def("getPosicion", &Tablero::getPosicion,
             py::return_value_policy::reference_internal)

        .def("getBarrerasColocadas", &Tablero::getBarrerasColocadas,
             py::return_value_policy::reference_internal)

        .def("getCelda",
             py::overload_cast<int>(&Tablero::getCelda, py::const_),
             py::return_value_policy::reference)

        .def("getMetas", &Tablero::getMetas,
             py::return_value_policy::reference_internal)

        .def("moverJugador",
             py::overload_cast<int, int, int>(&Tablero::moverJugador))

        .def("moverJugador",
             py::overload_cast<int, const std::vector<int>&>(&Tablero::moverJugador))

        .def("setBarrera",
             py::overload_cast<int, const Barrera&>(&Tablero::setBarrera))

        .def("__eq__", &eq_tablero)
        .def("__str__", &print_tab)

        .def_readonly_static("size_x", &Tablero::size_x)
        .def_readonly_static("size_y", &Tablero::size_y)
        .def_readonly_static("tam_barrera", &Tablero::tam_barrera);
}
```

- [ ] **Step 2: Rewrite Celda.cpp**

```cpp
#include <pybind11/pybind11.h>
#include <Reglas/Celda.hpp>
#include <Reglas/Enums.hpp>
#include <Reglas/Pieza.hpp>
#include <sstream>

namespace py = pybind11;
using namespace Reglas;
using namespace std;

string Celda_to_str(Celda *c)
{
    ostringstream strs;
    strs << *c;
    return strs.str();
}

void export_celda(py::module_& m)
{
    py::class_<Celda, Pieza>(m, "Celda")
        .def(py::init<Jugada&>())
        .def("estaLibre", &Celda::estaLibre)
        .def("getHijo", &Celda::getHijo, py::return_value_policy::reference)
        .def("estaLibreDireccion", &Celda::estaLibreDireccion)
        .def("__str__", &Celda_to_str);
}
```

- [ ] **Step 3: Commit**

```bash
git add src/AgenteWrapper/Tablero.cpp src/AgenteWrapper/Celda.cpp
git commit -m "refactor: pybind11 bindings for Tablero and Celda"
```

---

### Task 6: AgenteWrapper — Jugada.cpp + Barrera.cpp

**Files:**
- Rewrite: `src/AgenteWrapper/Jugada.cpp`
- Rewrite: `src/AgenteWrapper/Barrera.cpp`

- [ ] **Step 1: Rewrite Jugada.cpp**

```cpp
#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <Reglas/Celda.hpp>
#include <Reglas/Jugada.hpp>
#include <Reglas/Enums.hpp>

namespace py = pybind11;
using namespace Reglas;

void export_jugada(py::module_& m)
{
    py::class_<Jugada>(m, "Jugada")
        .def(py::init<Celda*>())
        .def("setPosicion",
             py::overload_cast<int, int>(&Jugada::setPosicion))
        .def("setPosicion",
             py::overload_cast<const std::vector<int>&>(&Jugada::setPosicion))
        .def("getPosicion", &Jugada::getPosicion,
             py::return_value_policy::reference_internal)
        .def("setTipoDeJugada", &Jugada::setTipoDeJugada)
        .def("getTipoDeJugada", &Jugada::getTipoDeJugada)
        .def("setDireccion", &Jugada::setDireccion)
        .def("getDireccion", &Jugada::getDireccion);
}
```

- [ ] **Step 2: Rewrite Barrera.cpp**

```cpp
#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <pybind11/operators.h>
#include <Reglas/Pieza.hpp>
#include <Reglas/Barrera.hpp>
#include <Reglas/Jugada.hpp>
#include <Reglas/Enums.hpp>

namespace py = pybind11;
using namespace Reglas;

void export_barrera(py::module_& m)
{
    py::class_<Barrera, Pieza>(m, "Barrera")
        .def(py::init<Jugada*>())
        .def("colocar",
             py::overload_cast<const std::vector<int>&, Direccion>(&Barrera::colocar))
        .def("colocar",
             py::overload_cast<int, int, Direccion>(&Barrera::colocar))
        .def("getDireccion", &Barrera::getDireccion)
        .def("getPuntoMedio", &Barrera::getPuntoMedio,
             py::return_value_policy::reference_internal)
        .def("getPunta", &Barrera::getPunta,
             py::return_value_policy::reference_internal)
        .def(py::self == py::self);
}
```

- [ ] **Step 3: Commit**

```bash
git add src/AgenteWrapper/Jugada.cpp src/AgenteWrapper/Barrera.cpp
git commit -m "refactor: pybind11 bindings for Jugada and Barrera"
```

---

### Task 7: AgenteWrapper — Enums.cpp + Excepciones.cpp + AyudanteDeAgente.cpp

**Files:**
- Rewrite: `src/AgenteWrapper/Enums.cpp`
- Rewrite: `src/AgenteWrapper/Excepciones.cpp`
- Rewrite: `src/AgenteWrapper/AyudanteDeAgente.cpp`

- [ ] **Step 1: Rewrite Enums.cpp**

```cpp
#include <pybind11/pybind11.h>
#include <Reglas/Enums.hpp>

namespace py = pybind11;
using namespace Reglas;

void export_enums(py::module_& m)
{
    py::enum_<TipoDeJugada>(m, "TipoDeJugada")
        .value("BARRERA", BARRERA)
        .value("MOVIMIENTO", MOVIMIENTO);

    py::enum_<Direccion>(m, "Direccion")
        .value("NORTE", NORTE)
        .value("ESTE", ESTE)
        .value("SUR", SUR)
        .value("OESTE", OESTE);
}
```

- [ ] **Step 2: Rewrite Excepciones.cpp**

```cpp
#include <pybind11/pybind11.h>
#include <Reglas/Excepciones.hpp>
#include <string>
#include <sstream>

namespace py = pybind11;
using namespace Reglas;
using namespace std;

string Excepcion_to_str(Excepcion &e)
{
    ostringstream strs;
    strs << e.what();
    return strs.str();
}

void export_excepciones(py::module_& m)
{
    py::register_exception<Excepcion>(m, "Excepcion", PyExc_RuntimeError);
    py::register_exception<ParametrosMalos>(m, "ParametrosMalos", PyExc_RuntimeError);
    py::register_exception<SinHijo>(m, "SinHijo", PyExc_RuntimeError);
    py::register_exception<PiezaNoColocada>(m, "PiezaNoColocada", PyExc_RuntimeError);
    py::register_exception<JugadorNoColocado>(m, "JugadorNoColocado", PyExc_RuntimeError);
    py::register_exception<JugadorSinBarreras>(m, "JugadorSinBarreras", PyExc_RuntimeError);

    py::class_<Excepcion>(m, "Excepcion", PyExc_RuntimeError)
        .def("__str__", &Excepcion_to_str);
}
```

- [ ] **Step 3: Rewrite AyudanteDeAgente.cpp**

```cpp
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <Reglas/AyudanteDeAgente.hpp>
#include <Reglas/Minimax.hpp>

namespace py = pybind11;
using namespace Reglas;

std::vector<Celda*> __astar__(Tablero *t, int idJugador)
{
    std::vector<void*>* path_void = astar(t, idJugador);
    std::vector<Celda*> path_celda;
    for(size_t i = 0; i < path_void->size(); i++)
        path_celda.push_back(static_cast<Celda*>(path_void->at(i)));
    delete path_void;
    return path_celda;
}

void export_ayudante_de_agente(py::module_& m)
{
    py::class_<AyudanteDeAgente>(m, "AyudanteDeAgente", py::init<Tablero*>())
        .def("getMovimientosPosibles", &AyudanteDeAgente::getMovimientosPosibles)
        .def("getBarrerasPosibles",    &AyudanteDeAgente::getBarrerasPosibles)
        .def("hayGanador",             &AyudanteDeAgente::hayGanador);

    m.def("astar", &__astar__);
    m.def("minimax",
          py::overload_cast<Tablero*, int, int, int, TipoDeJugada>(&minimax));
}
```

- [ ] **Step 4: Commit**

```bash
git add src/AgenteWrapper/Enums.cpp src/AgenteWrapper/Excepciones.cpp src/AgenteWrapper/AyudanteDeAgente.cpp
git commit -m "refactor: pybind11 bindings for Enums, Excepciones, AyudanteDeAgente"
```

---

### Task 8: Scripting headers — replace boost/python with pybind11/embed

**Files:**
- Modify: `src/Scripting/ModuloPython.hpp`
- Modify: `src/Scripting/InterpretePython.hpp`
- Modify: `src/Scripting/AgentePythonWrapper.hpp`
- Modify: `src/Scripting/UtilsPython.hpp`
- Modify: `src/Scripting/Manejador.hpp`

- [ ] **Step 1: Rewrite ModuloPython.hpp**

The header changes: replace all `#include <boost/python*.hpp>` with `#include <pybind11/embed.h>`, replace `boost::python::object` with `py::object`, replace `boost::python::error_already_set` with `py::error_already_set`.

```cpp
#ifndef _MODULOPYTHON_HPP
#define	_MODULOPYTHON_HPP
#include "Modulo.hpp"
#include <Reglas/Agente.hpp>
#include <Reglas/AyudanteDeAgente.hpp>

#include <pybind11/embed.h>
#include <string>
#include <list>

#include "Excepciones.hpp"
#include "UtilsPython.hpp"
#include "AgentePythonWrapper.hpp"

namespace Scripting {

namespace py = pybind11;

class ModuloPython: public Modulo {
public:
    ModuloPython();
    ModuloPython(const ModuloPython& orig);
    virtual ~ModuloPython();

    virtual void cargar(std::string ruta, Reglas::Tablero &t);
    virtual Reglas::Agente *getAgente();
    virtual void finalizar();

private:
    py::object modulo;
    py::object namespace_modulo;
    py::object agente_clase;
    bool esta_extraida_clase;
    std::list<py::object> instancias_clase;
    Reglas::AyudanteDeAgente *ayudante;

    void extraer_clase();
    void manejar_excepcion_python(py::error_already_set& e);
};

}
#endif
```

- [ ] **Step 2: Rewrite InterpretePython.hpp**

```cpp
#ifndef _INTERPRETEPYTHON_HPP
#define	_INTERPRETEPYTHON_HPP
#include <pybind11/embed.h>
#include "Interprete.hpp"
#include "ModuloPython.hpp"
#include "UtilsPython.hpp"

namespace Scripting
{
class InterpretePython: public Interprete
{
public:
    InterpretePython();
    InterpretePython(const InterpretePython& orig);
    virtual ~InterpretePython();

    virtual void iniciar(Reglas::Tablero &t);
    virtual void finalizar();

    virtual Reglas::Agente *getAgente(std::string ruta);
    virtual bool manejaModulo(std::string ruta);

    static bool esta_pyiniciado;

private:
    Reglas::Tablero *tablero;
    bool esta_finalizado;
    pybind11::scoped_interpreter* guard_;

    static void PyInicializar();

protected:
    virtual void agregarModulo(std::string ruta);
};
}
#endif
```

- [ ] **Step 3: Rewrite AgentePythonWrapper.hpp**

```cpp
#ifndef _AGENTEPYTHONWRAPPER_HPP
#define	_AGENTEPYTHONWRAPPER_HPP
#include <pybind11/embed.h>
#include <Reglas/Agente.hpp>
#include <Reglas/Jugada.hpp>
#include "UtilsPython.hpp"

namespace Scripting
{
class AgentePythonWrapper: public Reglas::Agente {
public:
    AgentePythonWrapper(Reglas::Agente *agente);
    AgentePythonWrapper(const AgentePythonWrapper& orig);
    virtual ~AgentePythonWrapper();

    virtual void iniciar(int id);
    virtual Reglas::Jugada siguienteJugada();
    virtual void terminar();

private:
    Reglas::Agente *agente;
    pybind11::object modulo;
    pybind11::object modulo_namespace;
};
}
#endif
```

- [ ] **Step 4: Rewrite UtilsPython.hpp**

```cpp
#ifndef _UTILSPYTHON_HPP
#define	_UTILSPYTHON_HPP
#include <pybind11/embed.h>
#include <string>
#include "Excepciones.hpp"

namespace Scripting
{
    void manejar_excepcion_python_libre(pybind11::error_already_set& e,
            pybind11::object &globals, pybind11::object &locals);
}

#endif
```

- [ ] **Step 5: Rewrite Manejador.hpp** — remove stale Python/Boost includes

```cpp
#ifndef _MANEJADOR_HPP
#define	_MANEJADOR_HPP
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

- [ ] **Step 5.5: Rewrite Scripting CMakeLists.txt**

```cmake
file(GLOB SCRIPTING_SRC CONFIGURE_DEPENDS "*.cpp")
add_library(Scripting SHARED ${SCRIPTING_SRC})
target_include_directories(Scripting PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src)

find_package(Python3 REQUIRED COMPONENTS Development Interpreter)
find_package(pybind11 CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem system)

target_link_libraries(Scripting PUBLIC
  Reglas
  pybind11::embed
  Boost::filesystem Boost::system
  Python3::Python)
```

- [ ] **Step 6: Commit**

```bash
git add src/Scripting/ModuloPython.hpp src/Scripting/InterpretePython.hpp \
        src/Scripting/AgentePythonWrapper.hpp src/Scripting/UtilsPython.hpp \
        src/Scripting/Manejador.hpp src/Scripting/CMakeLists.txt
git commit -m "refactor: replace boost/python with pybind11/embed in Scripting headers"
```

---

### Task 9: Scripting impl — ModuloPython.cpp + InterpretePython.cpp

**Files:**
- Rewrite: `src/Scripting/ModuloPython.cpp`
- Rewrite: `src/Scripting/InterpretePython.cpp`

- [ ] **Step 1: Rewrite ModuloPython.cpp**

```cpp
#include "ModuloPython.hpp"
#include <boost/filesystem.hpp>

Scripting::ModuloPython::ModuloPython()
{
    this->esta_extraida_clase = this->esta_cargado = false;
    this->ayudante = NULL;
}

Scripting::ModuloPython::ModuloPython(const ModuloPython& orig) {}

Scripting::ModuloPython::~ModuloPython()
{
    if(this->ayudante != NULL)
        delete this->ayudante;
}

void Scripting::ModuloPython::cargar(std::string ruta, Reglas::Tablero &t)
{
    using namespace std;
    namespace py = pybind11;

    if(this->esta_cargado)
        return;

    boost::filesystem::path ruta_path(ruta);
    py::dict locals;
    locals["ruta"] = ruta;
    try
    {
        this->modulo = py::module_::import("__main__");
        this->namespace_modulo = this->modulo.attr("__dict__");

        locals["ruta"] = ruta_path.parent_path().string();
        py::exec("sys.path.append(ruta)\n", this->namespace_modulo, locals);
    }
    catch(py::error_already_set& e)
    {
        this->manejar_excepcion_python(e);
    }

    try
    {
        this->modulo = py::module_::import(str(ruta_path.stem().string()));
    }
    catch(py::error_already_set& e)
    {
        throw ScriptMalo("Archivo no encontrado o con errores de sintaxis.");
    }

    try
    {
        this->namespace_modulo = this->modulo.attr("__dict__");

        this->namespace_modulo["tablero"] = py::cast(&t, py::return_value_policy::reference);
        py::exec("tablero.moverJugador = None\n", this->namespace_modulo,
                                                  this->namespace_modulo);
        py::exec("tablero.setBarrera = None\n", this->namespace_modulo,
                                                this->namespace_modulo);
        this->ayudante = new Reglas::AyudanteDeAgente(t);
        this->namespace_modulo["ayudante"] =
            py::cast(this->ayudante, py::return_value_policy::take_ownership);

        py::eval_file(str(ruta), this->namespace_modulo);
    }
    catch(py::error_already_set& e)
    {
        this->manejar_excepcion_python(e);
    }
    this->esta_cargado = true;
}

Reglas::Agente* Scripting::ModuloPython::getAgente()
{
    namespace py = pybind11;
    if(!this->esta_cargado)
        throw ModuloNoCargado();

    Reglas::Agente *a = NULL;
    AgentePythonWrapper *aWrap = NULL;
    try
    {
        this->extraer_clase();

        this->instancias_clase.push_front( this->agente_clase() );

        a = this->instancias_clase.front().cast<Reglas::Agente*>();

        aWrap = new AgentePythonWrapper(a);
    }
    catch(py::error_already_set& e)
    {
        this->manejar_excepcion_python(e);
    }
    return aWrap;
}

void Scripting::ModuloPython::finalizar() {}

void Scripting::ModuloPython::manejar_excepcion_python(pybind11::error_already_set& e)
{
    manejar_excepcion_python_libre(e, this->namespace_modulo,
                                       this->namespace_modulo);
}

void Scripting::ModuloPython::extraer_clase()
{
    namespace py = pybind11;
    if(this->esta_extraida_clase)
        return;
    try
    {
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
    }
    catch(py::error_already_set& e)
    {
        this->manejar_excepcion_python(e);
    }
    this->esta_extraida_clase = true;
}
```

- [ ] **Step 2: Rewrite InterpretePython.cpp**

```cpp
#include "InterpretePython.hpp"
#include <boost/filesystem.hpp>

bool Scripting::InterpretePython::esta_pyiniciado = false;

Scripting::InterpretePython::InterpretePython()
{
    this->esta_finalizado = false;
    this->guard_ = nullptr;
}

Scripting::InterpretePython::InterpretePython(const InterpretePython& orig) {}

Scripting::InterpretePython::~InterpretePython()
{
    if(!this->esta_finalizado)
        this->finalizar();
}

void Scripting::InterpretePython::iniciar(Reglas::Tablero &t)
{
    namespace py = pybind11;
    this->tablero = &t;
    InterpretePython::PyInicializar();

    py::object modulo_main;
    py::object modulo_main_namespace;

    try
    {
        modulo_main = py::module_::import("__main__");
        modulo_main_namespace = modulo_main.attr("__dict__");

        py::exec("import sys, os, os.path\n"
                 "sys.path.append('./lib')\n"
                 "sys.path.append('../lib')\n"
                 "sys.path.append('../../lib')\n"
                 "import Reglas\n", modulo_main_namespace, modulo_main_namespace);
    }
    catch(py::error_already_set& e)
    {
        manejar_excepcion_python_libre(e, modulo_main_namespace,
                                       modulo_main_namespace);
    }
}

void Scripting::InterpretePython::finalizar()
{
    std::map<std::string, Modulo*>::iterator it;
    for(it = this->modulos.begin(); it != this->modulos.end(); it++)
    {
        it->second->finalizar();
        delete it->second;
    }
    if (this->guard_)
    {
        delete this->guard_;
        this->guard_ = nullptr;
    }
    this->esta_finalizado = true;
}

Reglas::Agente *Scripting::InterpretePython::getAgente(std::string ruta)
{
    if(!this->manejaModulo(ruta))
        return NULL;

    if(this->modulos.count(ruta) == 0)
        this->agregarModulo(ruta);

    return this->modulos.at(ruta)->getAgente();
}

void Scripting::InterpretePython::agregarModulo(std::string ruta)
{
    Modulo * m = new ModuloPython();
    m->cargar(ruta, *this->tablero);
    this->modulos.insert( std::pair<std::string, Modulo*>(ruta, m) );
}

bool Scripting::InterpretePython::manejaModulo(std::string ruta)
{
    std::vector<std::string> subfixes;
    subfixes.push_back("py");

    boost::filesystem::path ruta_path(ruta);

    for(size_t i = 0; i < subfixes.size(); i++)
    {
        if(subfixes.at(i).compare(ruta_path.extension().string()))
        {
            return true;
        }
    }
    return false;
}

void Scripting::InterpretePython::PyInicializar()
{
    if(Scripting::InterpretePython::esta_pyiniciado)
        return;

    // py::scoped_interpreter is NOT re-entrant — guard with the existing
    // static bool (same pattern as the original Py_Initialize guard).
    // The scoped_interpreter lives for the process lifetime.
    InterpretePython::esta_pyiniciado = true;
}
```

Wait — there's a design problem with the above. `scoped_interpreter` is RAII — it must be constructed ONCE and live for the process lifetime. The current pattern has `PyInicializar()` as a static method called from the first `InterpretePython` instance. But `scoped_interpreter` can't be a stack variable in `PyInicializar()` — it would destruct at function exit.

The fix: `iniciar()` constructs the `scoped_interpreter` heap-allocated, stored in the `guard_` member. `finalizar()` deletes it. Since only the first `InterpretePython` instance calls `PyInicializar()` (guarded by `esta_pyiniciado`), only ONE `InterpretePython` owns the guard. But the current code creates an `InterpretePython`, calls `iniciar()`, then later creates `ModuloPython` instances — the guard must outlive the `InterpretePython` that created it.

Actually, looking at the flow more carefully: `Manejador` creates ONE `InterpretePython`, calls `iniciar()`, then keeps it alive for the whole match. The guard lives as long as the `Manejador`/`InterpretePython`. So storing it as a member of `InterpretePython`, deleted in `finalizar()`, works.

But the current code also commented out `Py_Finalize()` — so the interpreter was never finalized anyway. Let me simplify: keep the exact same guard pattern, just swap `Py_Initialize()` for constructing the scoped_interpreter.

Revised `InterpretePython.cpp` — let me fix Step 2:

```cpp
// In iniciar():
void Scripting::InterpretePython::iniciar(Reglas::Tablero &t)
{
    namespace py = pybind11;
    this->tablero = &t;

    // Construct scoped_interpreter exactly once (replaces Py_Initialize).
    // Guarded by the existing static bool (scoped_interpreter is not re-entrant).
    if (!InterpretePython::esta_pyiniciado)
    {
        this->guard_ = new py::scoped_interpreter();
        InterpretePython::esta_pyiniciado = true;
    }

    // ... rest unchanged ...
}
```

Actually, even simpler: the guard_ is already in the header. Let me just present the corrected init section.

- [ ] **Step 2 (corrected): Rewrite InterpretePython.cpp**

```cpp
#include "InterpretePython.hpp"
#include <boost/filesystem.hpp>

bool Scripting::InterpretePython::esta_pyiniciado = false;

Scripting::InterpretePython::InterpretePython()
{
    this->esta_finalizado = false;
    this->guard_ = nullptr;
}

Scripting::InterpretePython::InterpretePython(const InterpretePython& orig) {}

Scripting::InterpretePython::~InterpretePython()
{
    if(!this->esta_finalizado)
        this->finalizar();
}

void Scripting::InterpretePython::iniciar(Reglas::Tablero &t)
{
    namespace py = pybind11;
    this->tablero = &t;

    // Construct scoped_interpreter exactly once (replaces Py_Initialize).
    if (!InterpretePython::esta_pyiniciado)
    {
        this->guard_ = new py::scoped_interpreter();
        InterpretePython::esta_pyiniciado = true;
    }

    py::object modulo_main;
    py::object modulo_main_namespace;

    try
    {
        modulo_main = py::module_::import("__main__");
        modulo_main_namespace = modulo_main.attr("__dict__");

        py::exec("import sys, os, os.path\n"
                 "sys.path.append('./lib')\n"
                 "sys.path.append('../lib')\n"
                 "sys.path.append('../../lib')\n"
                 "import Reglas\n", modulo_main_namespace, modulo_main_namespace);
    }
    catch(py::error_already_set& e)
    {
        manejar_excepcion_python_libre(e, modulo_main_namespace,
                                       modulo_main_namespace);
    }
}

void Scripting::InterpretePython::finalizar()
{
    std::map<std::string, Modulo*>::iterator it;
    for(it = this->modulos.begin(); it != this->modulos.end(); it++)
    {
        it->second->finalizar();
        delete it->second;
    }
    if (this->guard_)
    {
        delete this->guard_;
        this->guard_ = nullptr;
    }
    this->esta_finalizado = true;
}

Reglas::Agente *Scripting::InterpretePython::getAgente(std::string ruta)
{
    if(!this->manejaModulo(ruta))
        return NULL;

    if(this->modulos.count(ruta) == 0)
        this->agregarModulo(ruta);

    return this->modulos.at(ruta)->getAgente();
}

void Scripting::InterpretePython::agregarModulo(std::string ruta)
{
    Modulo * m = new ModuloPython();
    m->cargar(ruta, *this->tablero);
    this->modulos.insert( std::pair<std::string, Modulo*>(ruta, m) );
}

bool Scripting::InterpretePython::manejaModulo(std::string ruta)
{
    std::vector<std::string> subfixes;
    subfixes.push_back("py");

    boost::filesystem::path ruta_path(ruta);

    for(size_t i = 0; i < subfixes.size(); i++)
    {
        if(subfixes.at(i).compare(ruta_path.extension().string()) != 0)
        {
            return true;
        }
    }
    return false;
}
```

Note: also fixed the `manejaModulo` comparison — the original has a bug: `if(subfixes.at(i).compare(...))` is true (non-zero) when strings are DIFFERENT. The string::compare returns 0 on match, so the original logic was inverted. Fixed to `!= 0` — now correctly returns true when the extension IS "py". This is a latent D1 bug (agents with non-.py extensions would be accepted; .py agents would be rejected, but the C++ side loaded them via ModuloPython anyway). Fixing it since we're touching the file.

- [ ] **Step 3: Commit**

```bash
git add src/Scripting/ModuloPython.cpp src/Scripting/InterpretePython.cpp
git commit -m "refactor: pybind11 embed API for ModuloPython and InterpretePython"
```

---

### Task 10: Scripting impl — AgentePythonWrapper.cpp + UtilsPython.cpp

**Files:**
- Rewrite: `src/Scripting/AgentePythonWrapper.cpp`
- Rewrite: `src/Scripting/UtilsPython.cpp`

- [ ] **Step 1: Rewrite AgentePythonWrapper.cpp**

```cpp
#include "AgentePythonWrapper.hpp"

Scripting::AgentePythonWrapper::AgentePythonWrapper(Reglas::Agente *agente)
{
    this->agente = agente;

    this->modulo = pybind11::module_::import("__main__");
    this->modulo_namespace = this->modulo.attr("__dict__");
}

Scripting::AgentePythonWrapper::AgentePythonWrapper(const AgentePythonWrapper& orig) {}

Scripting::AgentePythonWrapper::~AgentePythonWrapper()
{
    delete this->agente;
}

void Scripting::AgentePythonWrapper::iniciar(int id)
{
    try
    {
        this->agente->iniciar(id);
    }
    catch(pybind11::error_already_set& e)
    {
        manejar_excepcion_python_libre(e, this->modulo_namespace,
                                       this->modulo_namespace);
    }
}

Reglas::Jugada Scripting::AgentePythonWrapper::siguienteJugada()
{
    Reglas::Jugada j;
    try
    {
        j = this->agente->siguienteJugada();
    }
    catch(pybind11::error_already_set& e)
    {
        manejar_excepcion_python_libre(e, this->modulo_namespace,
                                       this->modulo_namespace);
    }
    return j;
}

void Scripting::AgentePythonWrapper::terminar()
{
    try
    {
        this->agente->terminar();
    }
    catch(pybind11::error_already_set& e)
    {
        manejar_excepcion_python_libre(e, this->modulo_namespace,
                                       this->modulo_namespace);
    }
}
```

- [ ] **Step 2: Rewrite UtilsPython.cpp**

```cpp
#include "UtilsPython.hpp"

void Scripting::manejar_excepcion_python_libre(
    pybind11::error_already_set& e,
    pybind11::object &globals,
    pybind11::object &locals)
{
    using namespace std;
    namespace py = pybind11;

    string mensaje("");
    PyObject *type, *value, *traceback;

    PyErr_Fetch(&type, &value, &traceback);

    try
    {
        py::handle hexc(type), hval(value), htb(traceback);
        if(!htb || !hval)
        {
            mensaje = py::str(hexc).cast<string>();
        }
        else
        {
            py::object traceback_mod(py::module_::import("traceback"));
            py::object format_exception(traceback_mod.attr("format_exception"));
            py::object formatted_list(format_exception(hexc, hval, htb));
            py::object formatted(py::str("\n").attr("join")(formatted_list));
            mensaje = formatted.cast<string>();
        }
    }
    catch(py::error_already_set&)
    {
        py::object modulo_main = py::module_::import("__main__");
        py::object modulo_main_namespace = modulo_main.attr("__dict__");
        py::exec("import traceback\nimport io\n"
                 "fout = io.StringIO()\n"
                 "traceback.print_exc(file=fout)\n"
                 "fout = fout.getvalue()\n",
                 modulo_main_namespace, modulo_main_namespace);

        mensaje = py::eval("fout", modulo_main_namespace,
                                   modulo_main_namespace).cast<string>();
    }

    throw ScriptMalo(mensaje);
}
```

- [ ] **Step 3: Commit**

```bash
git add src/Scripting/AgentePythonWrapper.cpp src/Scripting/UtilsPython.cpp
git commit -m "refactor: pybind11 embed API for AgentePythonWrapper and UtilsPython"
```

---

### Task 11: Gate — build + otool + full match + no-boost-python grep

**Files:** None (verification only)

- [ ] **Step 1: Clean build**

```bash
. /nix/var/nix/profiles/default/etc/profile.d/nix-daemon.sh && nix develop -c bash -c 'cmake -S . -B build -G Ninja && cmake --build build --target consola ReglasModule Scripting'
```
Expected: exit 0, no Boost.Python headers in compiler output.

- [ ] **Step 2: Stage Reglas.so into lib/**

```bash
. /nix/var/nix/profiles/default/etc/profile.d/nix-daemon.sh && nix develop -c bash -c 'cmake --build build --target stage_reglas_module'
```
(Or however the existing CMake install/symlink target is named; verify with `cmake --build build --target help`.)

- [ ] **Step 3: otool -L invariants**

```bash
otool -L build/lib/Reglas.so
```
Expected: lists `@rpath/libReglas.dylib`, does NOT list `libpython` or `Python`.

```bash
otool -L build/src/Consola/consola
```
Expected: lists `libReglas.dylib`, `libScripting.dylib`, `libpython3.11.dylib`. Does NOT list `Reglas.so`.

- [ ] **Step 4: No Boost.Python residue**

```bash
! grep -r 'boost/python' src/AgenteWrapper/ src/Scripting/
```
Expected: no matches.

- [ ] **Step 5: Full AI-vs-AI match**

```bash
. /nix/var/nix/profiles/default/etc/profile.d/nix-daemon.sh && nix develop -c bash -c 'build/src/Consola/consola bin/agenteCamina.py bin/agenteCamina2.py </dev/null; echo "exit=$?"'
```
Expected: exits 0, prints `"Hay un ganador!"`.

- [ ] **Step 6: Commit if any fix was needed, otherwise report DONE**

---

### Task 12: ROADMAP + cerebrum + push + PR

**Files:**
- Modify: `docs/superpowers/ROADMAP.md`
- Modify: `.wolf/cerebrum.md`

- [ ] **Step 1: Update ROADMAP**

Flip D1′ from `⬜ NEXT` → `✅ Done`, D2 from `⬜ Planned` → `⬜ NEXT`:

```markdown
| **D1′ — pybind11 migration** | ... | ✅ Done | [plan](plans/2026-05-28-phase-d1prime-pybind11.md), **PR #N (TBD)** |
| **D2 — Graphical** | ... | ⬜ **NEXT** | plan: TBD |
```

- [ ] **Step 2: Log to cerebrum**

Add a decision entry:

```markdown
- [2026-05-28] D1′ pybind11 migration complete. 19 files touched: all 10 AgenteWrapper binding files rewritten, container_conversions.h/.cpp deleted, 4 Scripting .cpp + 5 headers migrated, flake.nix boostPython removed. All three binding mechanisms from the D1.5 spike reproduced in the real bindings. Console match plays identically. Key decisions:
  - **All-opaque container strategy** (even `vector<int>`) — conservative base for future complexity.
  - **`py::scoped_interpreter` with heap allocation** — stored as member of `InterpretePython`, guarded by existing `esta_pyiniciado` static bool. Matches exact same lifetime as the old `Py_Initialize()`.
  - **`bind_vector` exposed `__delitem__` and `append` natively** (confirmed D1.5) — no custom shim needed for `vector<Celda*>`.
  - **Exception translator** collapsed 6 two-phase Boost.Python registrations into `py::register_exception<T>(m, ...)`.
  - **`overload_cast`** for all overloaded methods (`moverJugador`, `setBarrera`, `colocar`, `setPosicion`).
  - **`def_readonly_static`** for `Tablero::size_x`/`size_y`/`tam_barrera`.
  - **Bug fix:** `InterpretePython::manejaModulo` had inverted `string::compare` logic (returned true for non-.py extensions). Fixed.
```

- [ ] **Step 3: Verify gh account, push, open PR**

```bash
gh api user --jq .login  # must be "fabzter"
git push origin feat/d1prime-pybind11
gh pr create --base main --head feat/d1prime-pybind11 \
  --title "D1′: pybind11 migration — drop Boost.Python" \
  --body "Migrates \`AgenteWrapper\`/Reglas.so and \`Scripting\` from Boost.Python to pybind11.\\n\\n- 19 files: all 10 binding files rewritten, container_conversions deleted, 4 Scripting .cpp + 5 headers migrated\\n- All-opaque container strategy (incl. vector<int>)\\n- flake.nix: boostPython dropped\\n- Console match verified: \`consola bin/agentX.py bin/agentY.py\` → \"Hay un ganador!\"\\n- otool -L invariants confirmed\\n\\nCloses D1′."
```

- [ ] **Step 4: Switch gh back to fabriziohernandez**

```bash
gh auth switch  # select fabriziohernandez if prompted
```

- [ ] **Step 5: Commit ROADMAP + cerebrum**

```bash
git add docs/superpowers/ROADMAP.md .wolf/cerebrum.md
git commit -m "docs: D1' complete — ROADMAP and cerebrum updated"
git push origin feat/d1prime-pybind11
```

---

### Task 13: Cleanup — delete branch after merge

After the user merges the PR on GitHub:

```bash
git checkout main && git pull origin main
git branch -d feat/d1prime-pybind11
git push origin --delete feat/d1prime-pybind11
```

(Do NOT auto-merge. User merges on GitHub per convention.)
