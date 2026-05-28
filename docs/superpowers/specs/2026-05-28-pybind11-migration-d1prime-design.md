# D1′ — pybind11 Migration Design

> Migrates `AgenteWrapper`/`Reglas.so` and `Scripting` from Boost.Python to
> pybind11, drops `boost_python`, and verifies the console game still plays.
> **Predecessor:** D1.5 (proved all three binding mechanisms); its spike lives at
> `spikes/pybind/` and serves as the binding-template reference throughout.

## 1. Goal & non-goals

**Goal:** Replace Boost.Python with pybind11 in the real `AgenteWrapper`/`Reglas.so`
extension and `Scripting` embedder. Drop the `boost_python` dependency. Prove the
console game still plays identically.

**Non-goals:** Changing game logic, changing the Python agent API (agents continue
to `import Reglas` and use the same class/method/attribute names), removing other
Boost libraries (filesystem/system/thread/program_options stay), touching `Grafico`
or `Aplicacion` (D2). No coexisting Boost.Python + pybind11 — it's a clean swap.

## 2. Container strategy

**Decision: every container that carries identity is `MAKE_OPAQUE` + `bind_vector`.**
Only by-value `std::list<T>` crossings (where identity is irrelevant) use automatic
`pybind11/stl.h`. `container_conversions.h` + `.cpp` (300 lines of hand-rolled
Boost.Python converter templates) are **deleted**.

| Container | Binding | Reason |
|---|---|---|
| `std::vector<int>` | `MAKE_OPAQUE` + `bind_vector` | Conservative; positions are mutable views |
| `std::vector<Celda>` | `MAKE_OPAQUE` + `bind_vector` | Identity-preserving view (`Tablero::getCeldas`) |
| `std::vector<Celda*>` | `MAKE_OPAQUE` + `bind_vector` | Identity-preserving (proven D1.5, `spikes/pybind/bindings.cpp:19,31-32`) |
| `std::vector<Jugada>` | `MAKE_OPAQUE` + `bind_vector` | Identity-preserving view |
| `std::list<Barrera>` | `pybind11/stl.h` automatic | By-value; no identity to preserve |
| `std::list<Jugada>` | `pybind11/stl.h` automatic | By-value (`AyudanteDeAgente::getMovimientosPosibles`) |

The D1.5 spike's `bind_vector<std::vector<Cell*>>` implementation
(`spikes/pybind/bindings.cpp:43`) is the reference: `MAKE_OPAQUE` at file scope
(before any TU that references the type), `bind_vector` inside `PYBIND11_MODULE`,
`return_value_policy::reference_internal` on the accessor.

## 3. File-by-file migration

All binding files live in `src/AgenteWrapper/`. The embedder files live in
`src/Scripting/`. Pybind11 API patterns reference the D1.5 spike explicitly.

### 3.1 `src/AgenteWrapper/container_conversions.h` + `.cpp` — DELETED

All converter registration replaced by `#include <pybind11/stl.h>` for
`std::list<T>` and `MAKE_OPAQUE` + `py::bind_vector` for `std::vector<T>`/`T*`.

### 3.2 `src/AgenteWrapper/main.cpp`

```
BOOST_PYTHON_MODULE(Reglas)  →  PYBIND11_MODULE(Reglas, m)
```
Each `export_*()` call receives `m` as argument. Same exports, same order.
`#include <boost/python.hpp>` → `#include <pybind11/pybind11.h>`.

### 3.3 `src/AgenteWrapper/Agente.cpp`

```cpp
// Boost.Python (before)
struct AgenteWrapper : Agente, wrapper<Agente> {
    Jugada siguienteJugada() {
        return this->get_override("siguienteJugada")();
    }
    // ...
};
class_<AgenteWrapper, boost::noncopyable>("Agente")
    .def("siguienteJugada", pure_virtual(&Agente::siguienteJugada))
    // ...

// pybind11 (after) — pattern from spikes/pybind/bindings.cpp:22-27
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
// Module body:
py::class_<Agente, PyAgente>(m, "Agente")
    .def(py::init<>())
    .def("siguienteJugada", &Agente::siguienteJugada)
    .def("iniciar", &Agente::iniciar)
    .def("terminar", &Agente::terminar);
```

No `pure_virtual()` calls — pybind11 detects pure virtuals from the trampoline.
No `boost::noncopyable` — pybind11 classes are noncopyable by default.

### 3.4 `src/AgenteWrapper/Pieza.cpp`

```cpp
// Trampoline for virtual methods (same pattern as Agente)
struct PyPieza : Pieza {
    using Pieza::Pieza;
    const std::vector<int>& getPosicion() const override {
        PYBIND11_OVERRIDE(const std::vector<int>&, Pieza, getPosicion);
    }
    bool operator==(const Pieza& otro) const override {
        PYBIND11_OVERRIDE(bool, Pieza, operator==, otro);
    }
};

// Module body:
py::class_<Pieza, PyPieza>(m, "Pieza")
    .def(py::init<>())
    .def("getPosicion", &Pieza::getPosicion,
         py::return_value_policy::reference_internal)
    .def("__eq__", &Pieza::operator==);
```

`return_value_policy<copy_const_reference>()` → `reference_internal`.
`getPosicion()` returns `const std::vector<int>&` (a member `posicion`).
`reference_internal` ties Python's `IntVector` view to the `Pieza`'s lifetime
and allows mutation visibility — same semantics as D1, actually more correct.

### 3.5 `src/AgenteWrapper/Tablero.cpp`

Overloaded methods use `py::overload_cast` (Boost.Python used typedef + cast):

```cpp
// Overloaded: moverJugador(int, int, int) vs moverJugador(int, const vector<int>&)
.def("moverJugador",
     py::overload_cast<int, int, int>(&Tablero::moverJugador))
.def("moverJugador",
     py::overload_cast<int, const std::vector<int>&>(&Tablero::moverJugador))

// setBarrera(int, const Barrera&)
.def("setBarrera",
     py::overload_cast<int, const Barrera&>(&Tablero::setBarrera))

// getCelda: return_value_policy<reference_existing_object> → py::return_value_policy::reference
.def("getCelda",
     py::overload_cast<int>(&Tablero::getCelda, py::const_),
     py::return_value_policy::reference)
```

Static attributes (`size_x`, `size_y`, `tam_barrera`):
```cpp
py::class_<Tablero>(m, "Tablero")
    // ... methods ...
    .def_readonly_static("size_x", &Tablero::size_x)   // was .attr("size_x") = ...
    .def_readonly_static("size_y", &Tablero::size_y)
    .def_readonly_static("tam_barrera", &Tablero::tam_barrera);
```

`__str__` and `__eq__` helpers stay; bound with `py::def("__str__", &fn)`.

### 3.6 `src/AgenteWrapper/Celda.cpp`

```cpp
py::class_<Celda, Pieza>(m, "Celda")  // bases<Pieza> → second template arg
    .def(py::init<Jugada&>())
    .def("estaLibre", &Celda::estaLibre)
    .def("getHijo", &Celda::getHijo, py::return_value_policy::reference)
    .def("estaLibreDireccion", &Celda::estaLibreDireccion)
    .def("__str__", &Celda_to_str);
```

### 3.7 `src/AgenteWrapper/Jugada.cpp`

```cpp
py::class_<Jugada>(m, "Jugada")
    .def(py::init<Celda*>())
    // Overloaded setPosicion:
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
```

### 3.8 `src/AgenteWrapper/Barrera.cpp`

```cpp
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
    .def("__eq__", &Barrera::operator==);
```

`self == self` → `py::self == py::self` (requires `#include <pybind11/operators.h>`).

### 3.9 `src/AgenteWrapper/Enums.cpp`

```cpp
py::enum_<TipoDeJugada>(m, "TipoDeJugada")
    .value("BARRERA", BARRERA)
    .value("MOVIMIENTO", MOVIMIENTO);

py::enum_<Direccion>(m, "Direccion")
    .value("NORTE", NORTE)
    .value("ESTE", ESTE)
    .value("SUR", SUR)
    .value("OESTE", OESTE);
```

Pattern from `spikes/pybind/bindings.cpp:35-39`.

### 3.10 `src/AgenteWrapper/AyudanteDeAgente.cpp`

```cpp
py::class_<AyudanteDeAgente>(m, "AyudanteDeAgente",
    py::init<Tablero*>())
    .def("getMovimientosPosibles", &AyudanteDeAgente::getMovimientosPosibles)
    .def("getBarrerasPosibles",    &AyudanteDeAgente::getBarrerasPosibles)
    .def("hayGanador",             &AyudanteDeAgente::hayGanador);

m.def("astar",   &__astar__);  // was def("astar", ...)
m.def("minimax",
     py::overload_cast<Tablero*, int, int, int, TipoDeJugada>(&minimax));
```

`getMovimientosPosibles` / `getBarrerasPosibles` return `std::list<T>` — handled
automatically by `pybind11/stl.h` (included in main.cpp, visible module-wide).

### 3.11 `src/AgenteWrapper/Excepciones.cpp`

Boost.Python uses a two-phase pattern: (1) create `PyObject*` type objects,
(2) register translator functions. pybind11 collapses both:

```cpp
// In PYBIND11_MODULE (not separate translator functions):
py::register_exception<Excepcion>(m, "Excepcion", PyExc_RuntimeError);
py::register_exception<ParametrosMalos>(m, "ParametrosMalos", PyExc_RuntimeError);
py::register_exception<SinHijo>(m, "SinHijo", PyExc_RuntimeError);
py::register_exception<PiezaNoColocada>(m, "PiezaNoColocada", PyExc_RuntimeError);
py::register_exception<JugadorNoColocado>(m, "JugadorNoColocado", PyExc_RuntimeError);
py::register_exception<JugadorSinBarreras>(m, "JugadorSinBarreras", PyExc_RuntimeError);

// __str__ binding:
py::class_<Excepcion>(m, "Excepcion", PyExc_RuntimeError)
    .def("__str__", &Excepcion_to_str);
```

The `__str__` helper is kept; exception `__init__` from `what()` is automatic
with the `PyExc_RuntimeError` base. The 6-translator array + `PyObject*` globals
are deleted.

### 3.12 `src/Scripting/ModuloPython.cpp`

```cpp
// Boost.Python                             →  pybind11
#include <boost/python.hpp>                  →  #include <pybind11/embed.h>
boost::python::object                        →  py::object
boost::python::dict                          →  py::dict
boost::python::import("__main__")            →  py::module_::import("__main__")
boost::python::exec(string, ns, ns)          →  py::exec(string, ns)
boost::python::import(stem(ruta))            →  py::module_::import(stem(ruta))
boost::python::ptr(&t)                       →  py::cast(&t, py::return_value_policy::reference)
object(boost::python::ptr(new Ayudante(...)) →  py::cast(new Ayudante(...), py::return_value_policy::take_ownership)
boost::python::extract<Reglas::Agente*>(obj) →  obj.cast<Reglas::Agente*>()
boost::python::error_already_set             →  py::error_already_set
```

The `cargar()` method's flow is unchanged: `exec` the agent file → `import` the
module → inject `tablero`/`ayudante` into the namespace → `extraer_clase()`
pulls the agent subclass out. The D1.5 spike's `spikes/pybind/main.cpp:75-79`
shows the `py::cast` + `py::return_value_policy::reference` injection pattern.

### 3.13 `src/Scripting/InterpretePython.cpp`

```cpp
// Boost.Python                             →  pybind11
#include <boost/python.hpp>                  →  #include <pybind11/embed.h>
Py_Initialize()                              →  py::scoped_interpreter guard{};
                                                (RAII — replaces static bool esta_pyiniciado)

// sys.path setup kept as-is; add build/lib to sys.path for Reglas.so
// (cerebrum D1.5 lesson, 2026-05-27: extension lives in build tree, not source)

// main module injection (same pattern as ModuloPython)
boost::python::object main_mod = import("__main__");
boost::python::object ns = main_mod.attr("__dict__");
ns["tablero"] = object(ptr(&t));             →  ns["tablero"] = py::cast(&t, py::return_value_policy::reference);
```

**`py::scoped_interpreter` replaces the lazy-init pattern:** The current code
uses a static `bool esta_pyiniciado` + manual `Py_Initialize()`/`Py_Finalize()`.
pybind11's `scoped_interpreter` is RAII — construct once in `InterpretePython::iniciar()`,
destruct in `InterpretePython::finalizar()`. Since the embedder already manages
lifetime (single interpreter per process), this is a tightening, not a break.
The D1.5 spike uses exactly this pattern (`spikes/pybind/main.cpp:49`).

### 3.14 `src/Scripting/AgentePythonWrapper.cpp`

```cpp
#include <boost/python.hpp>                  →  #include <pybind11/embed.h>
boost::python::import("__main__")            →  py::module_::import("__main__")
boost::python::dict                          →  py::dict
boost::python::error_already_set             →  py::error_already_set
```

The wrapper delegates through `this->agente->iniciar(id)` etc. — C++ virtual
dispatch into the pybind11 trampoline. No API surface change for the wrapper.

### 3.15 `src/Scripting/UtilsPython.cpp`

```cpp
#include <boost/python.hpp>                  →  #include <pybind11/embed.h>
boost::python::object                        →  py::object
boost::python::error_already_set             →  py::error_already_set
```

Mechanical namespace swap. No logic changes.

## 4. Build changes

### 4.1 `src/AgenteWrapper/CMakeLists.txt`

```cmake
# Before:
find_package(Boost REQUIRED COMPONENTS python)
target_link_libraries(ReglasModule PRIVATE Reglas Boost::python Python3::Module)

# After (pattern from spikes/pybind/CMakeLists.txt:13-19):
find_package(pybind11 CONFIG REQUIRED)
target_link_libraries(ReglasModule PRIVATE
  Reglas
  pybind11::module
  Python3::Module)

if(APPLE)
  target_link_options(ReglasModule PRIVATE -Wl,-undefined,dynamic_lookup)
endif()
```

`OUTPUT_NAME "Reglas"` + `PREFIX ""` + `SUFFIX ".so"` properties stay.

### 4.2 `src/Scripting/CMakeLists.txt`

```cmake
# Before:
find_package(Boost REQUIRED COMPONENTS python filesystem system)
target_link_libraries(Scripting PUBLIC
  Reglas Boost::python Boost::filesystem Boost::system Python3::Python)

# After:
find_package(pybind11 CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem system)
target_link_libraries(Scripting PUBLIC
  Reglas
  pybind11::embed
  Boost::filesystem Boost::system
  Python3::Python)
```

`Boost::python` dropped; `Boost::filesystem`/`Boost::system` kept.

### 4.3 `flake.nix`

Remove `boostPython` from both `packages` and `devShells`. pybind11 is already
present (`pkgs.python311.pkgs.pybind11` from D1.5).

```nix
# After:
packages = forAll (pkgs:
  let
    irrlichtFork = pkgs.callPackage ./nix/irrlicht-fork.nix { darwin = pkgs.darwin; };
  in { inherit irrlichtFork; });   # boostPython removed

devShells = forAll (pkgs:
  let
    irrlichtFork = pkgs.callPackage ./nix/irrlicht-fork.nix { darwin = pkgs.darwin; };
  in {
    default = pkgs.mkShell {
      packages = [ pkgs.cmake pkgs.ninja pkgs.pkg-config pkgs.python311
                   pkgs.python311.pkgs.pybind11 irrlichtFork ];  # boostPython removed
    };
  });
```

### 4.4 Top-level CMakeLists.txt / lib symlink

The `lib/` symlink for `Reglas.so` stays: `consola` adds `./lib` to `sys.path`
and imports `Reglas`. The `INSTALL`/`SYMLINK` commands keep staging `Reglas.so`
into `lib/`. No change to the D1 staging logic.

## 5. Gate (exact assertions)

**Build:**
```bash
nix develop -c cmake --build build --target consola ReglasModule Scripting
```
Must exit 0 with no Boost.Python headers or `-lboost_python` in the link lines.

**Linkage invariants (verified with `otool -L`, pattern from D1.5 §7):**
```bash
otool -L build/lib/Reglas.so
# Must list: @rpath/libReglas.dylib
# Must NOT list: libpython, Python
```
```bash
otool -L build/src/Consola/consola
# Must list: libReglas.dylib, libScripting.dylib, libpython3.11.dylib
# Must NOT list: Reglas.so
```

**Full match:**
```bash
nix develop -c build/src/Consola/consola bin/agentX.py bin/agentY.py </dev/null
```
Must exit 0, print `"Hay un ganador!"`, no crash/exception.

**No Boost.Python residue:**
```bash
! grep -r 'boost/python' src/AgenteWrapper/ src/Scripting/
```
No `#include <boost/python.hpp>` in any migrated file.

## 6. Risks

**R1 — `py::overload_cast` vs. typedef+cast:** pybind11's `py::overload_cast<Args...>(&Class::method)` is a different syntax than Boost.Python's typedef + explicit cast. Every overloaded method site must be checked. Mitigation: the D1.5 spike didn't exercise overloaded methods; a compile failure on an overloaded bind is a soft failure (easy to fix).

**R2 — `py::register_exception` hierarchy:** pybind11's exception registration is simpler than Boost.Python's two-phase pattern, but the 6-exception hierarchy must be tested. Mitigation: the gate's full match exercises exception paths (agents that make illegal moves trigger `ReglasRotas` → Python exception → caught by the embedder).

**R3 — `py::scoped_interpreter` vs. lazy `Py_Initialize`:** The current embedder uses a static `bool esta_pyiniciado` with manual `Py_Initialize()`/`Py_Finalize()`. pybind11's `scoped_interpreter` is RAII — construct at `iniciar()`, destruct at `finalizar()`. If the embedder calls `iniciar()` twice, the second construction will fail (scoped_interpreter is not re-entrant). Mitigation: the current code already guards with `esta_pyiniciado`; the RAII version must ensure `scoped_interpreter` is constructed exactly once, matching the existing guard.

**R4 — `def_readonly_static` for `Tablero::size_x`:** The current Boost.Python bindings inject static attributes via `.attr("size_x") = …` after `class_` construction. pybind11's `def_readonly_static` requires the attributes to be actual `static const` class members — which `Tablero::size_x`/`size_y`/`tam_barrera` appear to be in `Pieza.hpp` (`const static int size_x = 9, size_y = 9`). If these are NOT declared `static` in `Tablero` itself but inherited from `Pieza`, they may need `def_readonly_static` with the `Pieza::` qualifier or a manual `.attr()` approach. Mitigation: verify the declaration in `Tablero.hpp` before binding.

**R5 — `operator==` for `Barrera`:** Boost.Python's `self == self` uses the `__eq__` protocol. pybind11's `py::self == py::self` (from `pybind11/operators.h`) is equivalent, but `Barrera` inherits from `Pieza` and its `operator==` may need explicit disambiguation if overloaded. Mitigation: the D1 code already binds `self == self` successfully; pybind11's operator binding should match.

**R6 — `return_value_policy::reference` for `getCelda`:** Boost.Python's `reference_existing_object` means "the C++ object outlives the Python reference." pybind11's `reference` means the same thing. If the `Tablero` is destroyed while Python holds a `Celda` reference, it's a dangling pointer — exactly the same risk D1 already has. No regression.

**R7 — `bind_vector` on `std::vector<int>`:** The current Boost.Python bindings register `std::vector<int>` with BOTH a `to_tuple` converter AND a `vector_indexing_suite`. pybind11's `bind_vector` provides the indexing-suite semantics (mutable view). If any agent code iterates a position vector as a Python `tuple` (e.g., `tuple(tablero.getPosicion(j))[0]`), it will still work — `bind_vector` exposes `__len__`, `__getitem__`, and `__iter__`. But if agent code relies on `isinstance(pos, tuple)` being `True`, that will break. Mitigation: the existing agents (`bin/*.py`) use `pos[0]`/`pos[1]` style, not `isinstance` checks. Verify by grepping the agent code.

**R8 — No `Reglas.so` import before injection (cerebrum D1.5 lesson):** The embedder must `py::module_::import("Reglas")` BEFORE injecting `Tablero`/`Ayudante` objects into the namespace — otherwise `py::cast` fails with "no converter." The D1.5 spike enforces this (`spikes/pybind/main.cpp:63`). The current `InterpretePython::iniciar()` already does an `import("Reglas")` early; verify it happens before the first `py::cast(&t, ...)`.

## 7. Files summary

| Action | File |
|---|---|
| **Delete** | `src/AgenteWrapper/container_conversions.h` |
| **Delete** | `src/AgenteWrapper/container_conversions.cpp` |
| **Rewrite** | `src/AgenteWrapper/main.cpp` |
| **Rewrite** | `src/AgenteWrapper/Agente.cpp` |
| **Rewrite** | `src/AgenteWrapper/Pieza.cpp` |
| **Rewrite** | `src/AgenteWrapper/Tablero.cpp` |
| **Rewrite** | `src/AgenteWrapper/Celda.cpp` |
| **Rewrite** | `src/AgenteWrapper/Jugada.cpp` |
| **Rewrite** | `src/AgenteWrapper/Barrera.cpp` |
| **Rewrite** | `src/AgenteWrapper/Enums.cpp` |
| **Rewrite** | `src/AgenteWrapper/AyudanteDeAgente.cpp` |
| **Rewrite** | `src/AgenteWrapper/Excepciones.cpp` |
| **Modify** | `src/AgenteWrapper/CMakeLists.txt` |
| **Modify** | `src/Scripting/ModuloPython.cpp` |
| **Modify** | `src/Scripting/InterpretePython.cpp` |
| **Modify** | `src/Scripting/AgentePythonWrapper.cpp` |
| **Modify** | `src/Scripting/UtilsPython.cpp` |
| **Modify** | `src/Scripting/CMakeLists.txt` |
| **Modify** | `flake.nix` |

