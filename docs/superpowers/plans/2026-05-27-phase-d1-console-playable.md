# Phase D1 — Console-Playable Port Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Port the non-graphical modules of Quassimodo (Python 2.6→3.11, Boost 1.40→1.81) so that an AI-vs-AI Quoridor match runs to completion in the **console**, building with CMake inside the isolated Nix env on macOS arm64.

**Architecture:** Bottom-up by dependency order — `Reglas` (rules, shared lib) → `Opciones` → `Scripting` (embeds CPython) + `AgenteWrapper` (the `Reglas.so` Boost.Python extension agents import) → the `.py` agents → `Aplicacion` built **console-only** (excludes `Grafico`/`PartidaGrafica` behind a `QM_CONSOLE_ONLY` CMake option). Each module gets a CMakeLists mirroring the proven spike linkage rules (single shared `libReglas` for RTTI, single shared `boost_python`, the extension as a `MODULE` with `-undefined dynamic_lookup`). The graphical port (`Grafico` + IrrlichtMt GUI) is explicitly out of scope — it's Phase D2.

**Tech Stack:** CMake + Ninja, clang 16, Python 3.11, Boost 1.81 (python/filesystem/system/thread/program_options), all pinned via the existing flake/devbox.

**Scope:** Phase D1 of `docs/superpowers/specs/2026-05-26-build-isolation-design.md` (the "source port" + "console match" portion of Phases D/E). Phase D2 (Grafico/IrrlichtMt GUI → graphical playable) is a separate plan.

**Discovery note:** This is a port. Each module task lists the KNOWN breakages (from the port-surface exploration) with concrete fixes, then a compile gate. Additional compile errors WILL appear; resolve them with `superpowers:systematic-debugging` (don't guess), keeping changes minimal and documented. Do NOT change game logic — only what's needed to compile/link/run under the modern toolchain.

**Universal env reminders (every task):**
- `nix`/`devbox` not on PATH non-interactively → prefix Bash with `. /nix/var/nix/profiles/default/etc/profile.d/nix-daemon.sh && ...`. Build via `nix develop -c bash -c '...'`. "Git tree is dirty" warnings are harmless.
- NO AI/LLM authorship in commits. Plain `git commit`. Identity pinned `fabzter <faboster@gmail.com>`.
- Branch: `feat/phase-d-port`.

---

## File Structure

```
/CMakeLists.txt                 # MODIFY: add_subdirectory(src) behind option QM_BUILD_GAME (default ON)
/src/CMakeLists.txt             # CREATE: orchestrates module subdirs; defines QM_CONSOLE_ONLY (default ON for D1)
/src/Reglas/CMakeLists.txt      # CREATE: libReglas (SHARED)
/src/Opciones/CMakeLists.txt    # CREATE: libOpciones (SHARED) -> Reglas
/src/Scripting/CMakeLists.txt   # CREATE: libScripting (SHARED) -> Reglas, Python, Boost.python
/src/AgenteWrapper/CMakeLists.txt # CREATE: Reglas extension module (Reglas.so, MODULE) -> Reglas, Boost.python
/src/Aplicacion/CMakeLists.txt  # CREATE: aplicacion executable (console-only sources when QM_CONSOLE_ONLY)
```
Source `.cpp/.hpp` files under each module are MODIFIED in place for py2→3 / Boost API fixes. `Grafico` gets NO CMakeLists in D1 (deferred to D2).

Responsibilities: each module CMakeLists builds exactly one artifact with explicit `target_link_libraries` (no implicit symbol resolution — macOS forbids it). `src/CMakeLists.txt` owns the `QM_CONSOLE_ONLY` switch that keeps `Grafico` and `PartidaGrafica` out of the D1 build.

---

## Task 1: Build-system wiring + libReglas

**Files:**
- Modify: `CMakeLists.txt` (top level)
- Create: `src/CMakeLists.txt`
- Create: `src/Reglas/CMakeLists.txt`
- Modify (as needed): `src/Reglas/*.cpp`, `src/Reglas/*.hpp` (only if compile errors appear)

- [ ] **Step 1: Add `src` to the top-level `CMakeLists.txt`**

Append to `/CMakeLists.txt` (after the existing spikes block):
```cmake
option(QM_BUILD_GAME "Build the real game modules" ON)
if(QM_BUILD_GAME)
  add_subdirectory(src)
endif()
```

- [ ] **Step 2: Create `src/CMakeLists.txt`**

```cmake
# D1 builds the non-graphical game. Grafico + the graphical Partida path are
# excluded while QM_CONSOLE_ONLY is ON (flipped OFF in Phase D2).
option(QM_CONSOLE_ONLY "Build console-only (exclude Grafico/PartidaGrafica)" ON)

add_subdirectory(Reglas)
add_subdirectory(Opciones)
add_subdirectory(Scripting)
add_subdirectory(AgenteWrapper)
add_subdirectory(Aplicacion)
```

- [ ] **Step 3: Create `src/Reglas/CMakeLists.txt`**

`Reglas` is the rules engine — the single shared lib that owns the domain-type RTTI (every other module and the Python extension link it). Glob its sources (it has ~13 .cpp).
```cmake
file(GLOB REGLAS_SRC CONFIGURE_DEPENDS "*.cpp")
add_library(Reglas SHARED ${REGLAS_SRC})
target_include_directories(Reglas PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src)
# Reglas uses Boost headers (and boost::filesystem in some helpers). Link defensively;
# if a component is unused the linker drops it.
find_package(Boost REQUIRED COMPONENTS filesystem system)
target_link_libraries(Reglas PUBLIC Boost::filesystem Boost::system)
```

- [ ] **Step 4: Configure + build libReglas; fix breakage**

```bash
. /nix/var/nix/profiles/default/etc/profile.d/nix-daemon.sh
nix develop -c bash -c 'cmake -S . -B build -G Ninja && cmake --build build --target Reglas 2>&1 | tail -40'
```
Expected first pass: may surface Boost API breakage. KNOWN candidates to fix with minimal edits (only if the compiler flags them):
- `boost::filesystem` path methods now return `path` not `std::string`: wrap with `.string()` (e.g. `p.extension().string()`, `p.stem().string()`) or compare via `.string()`.
- Any `#include` of removed Boost headers / deprecated `wpath` (Reglas is unlikely to have these — Grafico does).
Use `superpowers:systematic-debugging` for anything non-obvious. Do not alter rules logic.
Gate: `build/.../libReglas.dylib` (or `src/Reglas/libReglas.dylib`) builds with no errors.

- [ ] **Step 5: Commit**

```bash
git add CMakeLists.txt src/CMakeLists.txt src/Reglas/CMakeLists.txt src/Reglas
git commit -m "port: build libReglas with CMake (Reglas rules engine compiles)"
```

---

## Task 2: libOpciones

**Files:**
- Create: `src/Opciones/CMakeLists.txt`
- Modify (as needed): `src/Opciones/*.cpp`, `src/Opciones/*.hpp`

- [ ] **Step 1: Inspect what Opciones actually links**

```bash
ls src/Opciones; grep -rnE 'boost::program_options|boost/program_options|#include' src/Opciones/*.cpp src/Opciones/*.hpp | head
```
This determines whether to link `Boost::program_options` (the original makefile linked `boost_program_options-mt`, but the parsing may be custom — verify and link only what's used).

- [ ] **Step 2: Create `src/Opciones/CMakeLists.txt`**

```cmake
file(GLOB OPCIONES_SRC CONFIGURE_DEPENDS "*.cpp")
add_library(Opciones SHARED ${OPCIONES_SRC})
target_include_directories(Opciones PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(Opciones PUBLIC Reglas)
# If Step 1 showed boost::program_options usage, also link it:
find_package(Boost REQUIRED COMPONENTS program_options)
target_link_libraries(Opciones PUBLIC Boost::program_options)
```
(If Step 1 showed NO program_options usage, omit the `find_package(... program_options)` + its `target_link_libraries` line — don't link unused deps.)

- [ ] **Step 3: Build + fix**

```bash
. /nix/var/nix/profiles/default/etc/profile.d/nix-daemon.sh
nix develop -c bash -c 'cmake -S . -B build -G Ninja && cmake --build build --target Opciones 2>&1 | tail -40'
```
Gate: `libOpciones` builds. Fix any breakage (likely none beyond includes) with systematic-debugging.

- [ ] **Step 4: Commit**

```bash
git add src/Opciones/CMakeLists.txt src/Opciones
git commit -m "port: build libOpciones with CMake"
```

---

## Task 3: libScripting (Python 2.6 -> 3.11 embedding)

**Files:**
- Create: `src/Scripting/CMakeLists.txt`
- Modify: `src/Scripting/Manejador.hpp` (Python header path)
- Modify: `src/Scripting/UtilsPython.cpp` (cStringIO -> io)
- Modify: `src/Scripting/ModuloPython.cpp`, `src/Scripting/InterpretePython.cpp` (boost::filesystem path returns)
- Modify (as needed): other `src/Scripting/*` on compile errors

- [ ] **Step 1: Fix the hardcoded Python 2.6 header in `src/Scripting/Manejador.hpp`**

Find the line `#include <python2.6/Python.h>` and replace with:
```cpp
#include <Python.h>
```
(CMake's `Python3::Python` target supplies the correct include dir.)

- [ ] **Step 2: Fix `cStringIO` in `src/Scripting/UtilsPython.cpp`**

The exception-reporting helper runs an embedded Python snippet using `cStringIO` (removed in Python 3). Find the `exec("import traceback\nimport cStringIO\n...")` block (around line 39) and change the Python source string so it uses `io` instead of `cStringIO`. Concretely, replace `import cStringIO` with `import io` and `cStringIO.StringIO()` with `io.StringIO()` inside the embedded snippet. Read the surrounding lines first to keep the rest of the snippet intact.

- [ ] **Step 3: Fix `boost::filesystem` path-method returns in `ModuloPython.cpp` and `InterpretePython.cpp`**

In modern Boost, `path::extension()`, `path::stem()`, `path::parent_path()` return `boost::filesystem::path`, not `std::string`. Fix the specific call sites:
- `InterpretePython.cpp`: `ruta_path.extension().compare(...)` → `ruta_path.extension().string().compare(...)` (call `.string()` before `.compare`). Read the exact line and adjust; if the logic compares an extension to `"py"`, ensure the comparison uses the string form (note `extension()` includes the leading dot, e.g. `.py` — preserve existing behavior; if the original compared without the dot, keep that semantics).
- `ModuloPython.cpp`: `ruta_path.parent_path().string()` already calls `.string()` — likely fine. `str(ruta_path.stem())` and `import(str(ruta_path.stem()))` — wrap as `str(ruta_path.stem().string())` so a `std::string` is passed to `boost::python::str`.
Read each site before editing; make the minimal change that preserves behavior.

- [ ] **Step 4: Create `src/Scripting/CMakeLists.txt`**

```cmake
file(GLOB SCRIPTING_SRC CONFIGURE_DEPENDS "*.cpp")
add_library(Scripting SHARED ${SCRIPTING_SRC})
target_include_directories(Scripting PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src)

find_package(Python3 REQUIRED COMPONENTS Development Interpreter)
find_package(Boost REQUIRED COMPONENTS python filesystem system)

# Explicit deps (macOS two-level namespace forbids implicit resolution):
target_link_libraries(Scripting PUBLIC
  Reglas
  Boost::python Boost::filesystem Boost::system
  Python3::Python)
```

- [ ] **Step 5: Build + fix**

```bash
. /nix/var/nix/profiles/default/etc/profile.d/nix-daemon.sh
nix develop -c bash -c 'cmake -S . -B build -G Ninja && cmake --build build --target Scripting 2>&1 | tail -50'
```
Likely additional fixes (resolve with systematic-debugging, minimal edits):
- Any other py2-only C-API or embedded-Python-string usage surfaced by compilation.
- Boost.Python signature nuances under py3.
Gate: `libScripting` builds.

- [ ] **Step 6: Commit**

```bash
git add src/Scripting/CMakeLists.txt src/Scripting
git commit -m "port: libScripting embeds Python 3.11 (header path, io, boost::filesystem fixes)"
```

---

## Task 4: Reglas.so (AgenteWrapper Boost.Python extension)

**Files:**
- Create: `src/AgenteWrapper/CMakeLists.txt`
- Modify (as needed): `src/AgenteWrapper/*.cpp`

This builds the `Reglas` Python module that agents `import`. It mirrors the proven `pyembed` spike topology: a `MODULE` linking the shared `Reglas` lib + shared `boost_python`, NOT static.

- [ ] **Step 1: Create `src/AgenteWrapper/CMakeLists.txt`**

```cmake
file(GLOB AGENTEWRAP_SRC CONFIGURE_DEPENDS "*.cpp")
# The Python extension MUST be named Reglas.so (agents do `import Reglas`).
add_library(ReglasModule MODULE ${AGENTEWRAP_SRC})
set_target_properties(ReglasModule PROPERTIES OUTPUT_NAME "Reglas" PREFIX "" SUFFIX ".so")
target_include_directories(ReglasModule PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src)

find_package(Python3 REQUIRED COMPONENTS Development)
find_package(Boost REQUIRED COMPONENTS python)
target_link_libraries(ReglasModule PRIVATE Reglas Boost::python Python3::Module)
if(APPLE)
  target_link_options(ReglasModule PRIVATE -Wl,-undefined,dynamic_lookup)
endif()
```

- [ ] **Step 2: Build + fix**

```bash
. /nix/var/nix/profiles/default/etc/profile.d/nix-daemon.sh
nix develop -c bash -c 'cmake -S . -B build -G Ninja && cmake --build build --target ReglasModule 2>&1 | tail -50'
```
Likely fixes (systematic-debugging, minimal): Boost.Python `class_`/`def`/`pure_virtual`/`vector_indexing_suite`/converter code that changed between Boost 1.40 and 1.81 (mostly stable; watch for `bases<>`, registration order, and any `extract`/`object` API nuances). Do NOT change which classes/methods are exposed.
Gate: `Reglas.so` builds.

- [ ] **Step 3: Verify the module imports under Python 3 (find it via the build dir)**

```bash
. /nix/var/nix/profiles/default/etc/profile.d/nix-daemon.sh
SO_DIR=$(dirname $(find build -name 'Reglas.so' | head -1))
nix develop -c bash -c "cd $SO_DIR && python3 -c 'import Reglas; print(\"Reglas import OK:\", [n for n in dir(Reglas) if not n.startswith(\"__\")][:10])'"
```
Note: this import may need the `Reglas` C++ lib + boost_python on the dynamic-load path. If it fails with a dyld/symbol error, set the module's rpath to find `libReglas.dylib` (`set_target_properties(ReglasModule PROPERTIES BUILD_RPATH "$<TARGET_FILE_DIR:Reglas>")`) and rebuild. If `import Reglas` requires the embedding app context (it's a wrapper over the same registry), it's acceptable for full validation to occur in Task 6's match; if so, report that import-in-isolation needs the embedder and proceed.
Gate (soft): either `Reglas import OK` prints, or you've documented that import requires the embedder (validated in Task 6).

- [ ] **Step 4: Commit**

```bash
git add src/AgenteWrapper/CMakeLists.txt src/AgenteWrapper
git commit -m "port: build Reglas.so Boost.Python extension (Python 3)"
```

---

## Task 5: Port the Python agent scripts to Python 3

**Files:**
- Modify: `bin/*.py` (the agent scripts with py2-isms)

- [ ] **Step 1: Find all py2-isms across the agents**

```bash
grep -rnE 'print [^(]|xrange|maxint|print >>|has_key|iteritems|itervalues|\braw_input\b' bin/*.py
```
This lists every offending line across all agent scripts (the exploration found `print "..."` statements, `from sys import maxint`, `xrange` in at least `agenteCamina.py`, `agenteMiniMax2.py`, `agenteMiniMax4.py`).

- [ ] **Step 2: Fix each agent file**

For every match from Step 1, apply the mechanical Python 3 fix:
- `print "x"` / `print x` → `print("x")` / `print(x)` (wrap in parens; for `print >>f, x` → `print(x, file=f)`).
- `from sys import maxint` → `from sys import maxsize as maxint` (preserves the name `maxint` used later, minimal change), or replace usages with `sys.maxsize`.
- `xrange(` → `range(`.
- `.has_key(k)` → `k in dict`; `.iteritems()` → `.items()`; `.itervalues()` → `.values()`; `raw_input` → `input` (only if present).
Edit each file; keep agent logic identical.

- [ ] **Step 3: Verify each agent at least parses + imports under Python 3**

```bash
. /nix/var/nix/profiles/default/etc/profile.d/nix-daemon.sh
nix develop -c bash -c 'for f in bin/*.py; do python3 -m py_compile "$f" && echo "OK $f" || echo "FAIL $f"; done'
```
Gate: every `bin/*.py` reports `OK` (compiles under py3). (Full runtime import that does `import Reglas` will be exercised in Task 6.)

- [ ] **Step 4: Commit**

```bash
git add bin/
git commit -m "port: Python 3 fixes for agent scripts (print/xrange/maxint)"
```

---

## Task 6: Aplicacion (console-only) + a playable console match

**Files:**
- Create: `src/Aplicacion/CMakeLists.txt`
- Modify (as needed): `src/Aplicacion/*.cpp`, `src/Aplicacion/*.hpp`

`Aplicacion` normally links `Grafico` (graphical path). For D1 we build CONSOLE-ONLY: exclude `PartidaGrafica.cpp` and any Grafico-dependent sources, and don't link `Grafico`.

- [ ] **Step 1: Determine the console vs graphical source split**

```bash
ls src/Aplicacion
grep -rnE 'Grafico|PartidaGrafica|loopGrafico|irr::|irrlicht' src/Aplicacion/*.cpp src/Aplicacion/*.hpp
```
Identify which `.cpp` files reference `Grafico`/Irrlicht (e.g. `PartidaGrafica.cpp`, possibly parts of `ManejadorJuego.cpp`). The console build must exclude/guard those.

- [ ] **Step 2: Create `src/Aplicacion/CMakeLists.txt` (console-only sources)**

```cmake
# Console-only build: exclude graphical sources while QM_CONSOLE_ONLY is ON.
file(GLOB APP_SRC CONFIGURE_DEPENDS "*.cpp")
if(QM_CONSOLE_ONLY)
  list(FILTER APP_SRC EXCLUDE REGEX "PartidaGrafica\\.cpp$")
endif()

add_executable(aplicacion ${APP_SRC})
target_include_directories(aplicacion PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src)

find_package(Boost REQUIRED COMPONENTS filesystem system thread)
target_link_libraries(aplicacion PRIVATE
  Reglas Opciones Scripting
  Boost::filesystem Boost::system Boost::thread)
# Ensure the executable can dlopen Reglas.so and find sibling dylibs at runtime.
set_target_properties(aplicacion PROPERTIES
  BUILD_RPATH "$<TARGET_FILE_DIR:Reglas>;$<TARGET_FILE_DIR:ReglasModule>")
```
If Step 1 found Grafico references OUTSIDE `PartidaGrafica.cpp` (e.g. in `ManejadorJuego.cpp`), guard those code regions with `#ifndef QM_CONSOLE_ONLY` and add `target_compile_definitions(aplicacion PRIVATE $<$<BOOL:${QM_CONSOLE_ONLY}>:QM_CONSOLE_ONLY>)`. Make the minimal change that lets the console path compile without Grafico.

- [ ] **Step 3: Build the executable; fix breakage**

```bash
. /nix/var/nix/profiles/default/etc/profile.d/nix-daemon.sh
nix develop -c bash -c 'cmake -S . -B build -G Ninja && cmake --build build --target aplicacion 2>&1 | tail -60'
```
Resolve compile/link errors with systematic-debugging (py2→3, boost::filesystem, missing console-mode guards). Gate: `aplicacion` executable builds and links.

- [ ] **Step 4: Discover how to launch a console AI-vs-AI match**

```bash
ls conf; sed -n '1,60p' conf/opciones.conf
grep -rnE 'ManejadorOpciones|EDT_NULL|grafico|agente|--|argv|opciones.conf' src/Aplicacion/*.cpp | head -40
```
Determine how the app selects console mode (video driver `EDT_NULL`) and how two agents are specified (config file `conf/opciones.conf` and/or CLI args, with agent `.py` paths from `bin/`). The Reglas.so the agents import must be on `sys.path` — note `InterpretePython.cpp` appends `./lib`, `../lib`, `../../lib` to `sys.path`, so the built `Reglas.so` must be reachable at one of those (copy it next to where the app runs, or run from a dir where `lib/Reglas.so` exists). Set this up.

- [ ] **Step 5: Run a full console match (the gate)**

Construct the actual invocation discovered in Step 4 (console mode + two ported agents, e.g. `bin/agenteCamina.py` vs `bin/agenteMiniMax2.py`), ensuring `Reglas.so` is importable. Run it inside the env:
```bash
. /nix/var/nix/profiles/default/etc/profile.d/nix-daemon.sh
nix develop -c bash -c '<the discovered console-match command>'
```
**Gate:** the match runs to a terminal outcome — a winner is declared (or a rule-violation loss), printed to the console, with no crash. Capture the output.
If the match crashes (e.g. converter/registry error, agent import failure), debug with systematic-debugging — the most likely causes are `Reglas.so` not on `sys.path`, or a Boost.Python registry/RTTI issue (verify `Reglas`, `Scripting`, and `Reglas.so` all link the SAME shared `Reglas` + `boost_python`, per the proven spike rules).

- [ ] **Step 6: Commit**

```bash
git add src/Aplicacion/CMakeLists.txt src/Aplicacion
git commit -m "port: console-only aplicacion runs a full AI-vs-AI match"
```

---

## Task 7: Document + reproducibility

**Files:**
- Modify: `BUILDING.md`

- [ ] **Step 1: Add a "Build & run the console game (Phase D1)" section to `BUILDING.md`**

Document, accurately from what worked in Task 6: how to build the game (`cmake -S . -B build -G Ninja && cmake --build build`), where `aplicacion` and `Reglas.so` land, how to make `Reglas.so` importable (sys.path/lib dir), and the exact command to run an AI-vs-AI console match with two example agents. Keep it concise and copy-pasteable.

- [ ] **Step 2: Verify a clean build + match from scratch**

```bash
cd /Users/fabzter/src/quassimodo
. /nix/var/nix/profiles/default/etc/profile.d/nix-daemon.sh
rm -rf build
nix develop -c bash -c 'cmake -S . -B build -G Ninja && cmake --build build'
# then the documented console-match command:
nix develop -c bash -c '<the documented console-match command>'
```
Gate: clean build succeeds and the console match completes with a declared outcome.

- [ ] **Step 3: Commit**

```bash
git add BUILDING.md
git commit -m "docs: how to build + run the console game (Phase D1)"
```

---

## Definition of Done (Phase D1)

- `libReglas`, `libOpciones`, `libScripting` build as shared libs; `Reglas.so` (Boost.Python extension) builds.
- All `bin/*.py` agents compile under Python 3.
- `aplicacion` builds console-only (no Grafico/IrrlichtMt dependency) and links.
- A full AI-vs-AI Quoridor match runs to completion in the console with a declared outcome and no crash, using the embedded Python 3 interpreter + the `Reglas` extension.
- A clean from-scratch build reproduces it; `BUILDING.md` documents the run command.

When this passes, write the **Phase D2 plan** (port `Grafico` against IrrlichtMt — first verifying which of Irrlicht's GUI API survived in the fork — to reach a graphical playable match).
```
