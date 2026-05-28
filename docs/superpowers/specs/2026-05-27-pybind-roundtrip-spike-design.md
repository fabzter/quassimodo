# pybind11 Round-Trip Spike (Phase D1.5) — Design Spec

- **Date:** 2026-05-27
- **Status:** Approved (design); pending implementation plan
- **Project:** Quassimodo — Phase D1.5
- **Companion docs:** [`docs/superpowers/ROADMAP.md`](../ROADMAP.md), [`ONBOARDING.md`](../../../ONBOARDING.md) §8, [`docs/superpowers/specs/2026-05-26-build-isolation-design.md`](2026-05-26-build-isolation-design.md)

## 1. Background

Quassimodo's D1 console-playable port (PR #2, merged) runs the full Boost.Python topology — `libReglas` shared + `Reglas.so` separate extension imported at runtime + a `Scripting` embedder — and a console AI-vs-AI match plays end-to-end on macOS arm64. The next phase, **D1′**, will swap Boost.Python for pybind11 in `src/AgenteWrapper/` and `src/Scripting/`. **D1.5 is the de-risking spike for that migration.**

The bar for D1.5 is the **live D1 bindings**, not the earlier toy `spikes/pyembed/` (which only proved an abstract `Widget` with a `string` return and zero STL crossings). D1.5 must prove pybind11 can reproduce the three concrete mechanisms the real bindings use today, in one process under the existing isolated env.

## 2. Goal & Non-Goals

### Goal
Build a minimal pybind11 spike under `spikes/pybind/` that, in one process and one round-trip, exercises **exactly** the three binding mechanisms `AgenteWrapper`/`Scripting` rely on:

1. **Pure-virtual trampoline returning a bound value type** — Python subclasses an abstract C++ class and overrides a method that returns a bound value-type instance; C++ calls the method and receives the value back.
2. **STL containers of bound types, both directions and both binding styles** — (a) `pybind11/stl.h` by-value list↔list of a bound value type, mirroring `std::list<Jugada>` from `AyudanteDeAgente::getMovimientosPosibles`; (b) `py::bind_vector` + `PYBIND11_MAKE_OPAQUE` over a `std::vector<T*>`, a live mutable view with identity preserved across the boundary, mirroring `std::vector<Celda*>` in `container_conversions.cpp`.
3. **Embed + cross-`.so` registry + cast subclass back** — embedder uses `pybind11/embed.h`, imports a *separate* extension `.so` at runtime (the extension is **not** linked into the embedder), injects a live C++ object into the agent namespace, execs an agent script, then casts the Python subclass instance back to a C++ pointer and calls a virtual method on it.

A single gate string **`PYBIND ROUNDTRIP OK`** is printed iff all assertions in §6 pass; the binary exits non-zero otherwise.

### Non-Goals
- Porting any real game module (that work is D1′).
- Coexisting Boost.Python and pybind11 in one process. D1.5 is additive at the build level (the `boost_python` dep stays in `flake.nix` for D1); it is *not* loaded into the spike process.
- Performance, threading, exception propagation, multi-round-trip flows.
- Replacing or modifying `spikes/pyembed/` — D1.5 lives next to it and the pyembed spike continues to build until D1′ replaces the real bindings.

## 3. Target & Constraints

- **Primary target:** native macOS arm64 (Apple Silicon), inside the project's pinned Nix devShell. The `.so` must load via `dyld` two-level namespace rules.
- **Reachable later:** the same source must remain buildable on x86_64 / Linux under the same flake; no macOS-only `#ifdef` paths in the source.
- **Isolation:** all toolchain + deps come from `flake.nix`; pybind11 added to the devShell and pinned via the same nixpkgs revision (24.11). No Homebrew, no system Python.
- **Build system:** the spike is a CMake subdirectory of the existing `spikes/` tree, built by the same top-level `cmake -S . -B build -G Ninja` invocation that builds the rest of the project (under the `QM_BUILD_SPIKES` option, default ON).

## 4. Topology & Linkage

Mirrors the D1 boost.python topology one-to-one; pybind11 replaces boost.python; no second registry lib is needed because pybind11's converter registry is process-global through the embedded interpreter.

```
pybind_spike                                   (analog of Scripting/consola embedder)
 ├─ libpybindspike_domain  ← C++ stand-ins     (analog of libReglas — single RTTI source)
 └─ pybind11::embed        ← Python3::Python   (the embedder pulls in libpython)
PybindSpike.so   = CMake MODULE, OUTPUT_NAME PybindSpike, PREFIX "", SUFFIX ".so"
 ├─ libpybindspike_domain  ← SAME shared lib   (single RTTI for cross-boundary casts)
 └─ pybind11::module       ← -undefined dynamic_lookup on APPLE
```

### Non-negotiable rules
1. **`PybindSpike.so` is built as a CMake `MODULE`** with output `PybindSpike.so` (PREFIX "", SUFFIX ".so") and on macOS uses `-Wl,-undefined,dynamic_lookup` so the Python C-API symbols resolve from the host process. Equivalent to D1's `Reglas.so` rule.
2. **`PybindSpike.so` is NOT linked into `pybind_spike`.** The embedder discovers and imports it at runtime via `py::module_::import("PybindSpike")`. Verified post-build with `otool -L` (see §7).
3. **One shared `libpybindspike_domain`** linked by BOTH the extension and the embedder. Single RTTI source: pointers and references to `Cell`/`Move`/`Player` cross the binding boundary without slicing or type-info mismatch.
4. **Explicit `target_link_libraries`** everywhere. No reliance on Linux load-time implicit resolution (macOS two-level namespace forbids it).
5. **`py::module_::import("PybindSpike")` is called BEFORE any C++ object is injected** into Python. This registers the type converters; without it, `py::cast(&helper, …)` raises "no such conversion" (cerebrum lesson from the pyembed spike, 2026-05-26).

## 5. Domain Types & Bindings

The stand-ins are **minimal but carry the real shapes** of the live `Reglas::` types — neither the toy pyembed `Widget` nor the full `Jugada`/`Celda` graph.

### C++ side (`spikes/pybind/domain.hpp` + `domain.cpp`)
```cpp
namespace pybindspike {

enum class Direction { N, E, S, W };

struct Move {
    int x = 0;
    int y = 0;
    Direction dir = Direction::N;
    Move() = default;
    Move(int x_, int y_, Direction d) : x(x_), y(y_), dir(d) {}
};

struct Cell {
    int x = 0;
    int y = 0;
    bool blocked = false;
};

class Player {
public:
    virtual ~Player() = default;
    virtual Move nextMove() = 0;
};

class Helper {
public:
    Helper();   // seeds storage with 3 Cells; cells_view points at them;
                // moves_ seeded with 2 Moves
    std::vector<Cell*>& cells();   // live mutable view, lifetime tied to *this
    std::list<Move>     moves();   // by-value copy returned

    // Public so the embedder can inspect post-round-trip state without
    // re-crossing into Python. Not part of the binding surface.
    std::vector<Cell>   storage;
    std::vector<Cell*>  cells_view;
    std::list<Move>     moves_;
};

} // namespace pybindspike
```

### Binding TU (`spikes/pybind/bindings.cpp`)
```cpp
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>            // enables std::list<Move> by-value crossing
#include <pybind11/stl_bind.h>       // py::bind_vector
#include "domain.hpp"

namespace py = pybind11;
using namespace pybindspike;

// Critical: must appear BEFORE the bindings TU references std::vector<Cell*>.
PYBIND11_MAKE_OPAQUE(std::vector<Cell*>);

class PyPlayer : public Player {
public:
    using Player::Player;
    Move nextMove() override {
        PYBIND11_OVERRIDE_PURE(Move, Player, nextMove);
    }
};

PYBIND11_MODULE(PybindSpike, m) {
    py::enum_<Direction>(m, "Direction")
        .value("N", Direction::N).value("E", Direction::E)
        .value("S", Direction::S).value("W", Direction::W);

    py::class_<Move>(m, "Move")
        .def(py::init<>())
        .def(py::init<int,int,Direction>())
        .def_readwrite("x",   &Move::x)
        .def_readwrite("y",   &Move::y)
        .def_readwrite("dir", &Move::dir);

    py::class_<Cell>(m, "Cell")
        .def_readwrite("x",       &Cell::x)
        .def_readwrite("y",       &Cell::y)
        .def_readwrite("blocked", &Cell::blocked);

    // Opaque mutable view.
    py::bind_vector<std::vector<Cell*>>(m, "CellPtrVector");

    py::class_<Player, PyPlayer>(m, "Player")
        .def(py::init<>())
        .def("nextMove", &Player::nextMove);

    py::class_<Helper>(m, "Helper")
        .def(py::init<>())
        .def("cells", &Helper::cells, py::return_value_policy::reference_internal)
        .def("moves", &Helper::moves);  // returns std::list<Move> by value via stl.h
}
```

Notes:
- `PYBIND11_MAKE_OPAQUE` must be visible in **every** TU that uses the opaque type. For D1.5 only `bindings.cpp` references `std::vector<Cell*>` across the binding; `domain.cpp` only touches `cells_view` as a plain field. If any other TU later references it, MAKE_OPAQUE has to move into a shared header — flagged for D1′, not for D1.5.
- `def(py::init<>())` on `Player` is the standard trampoline-class registration pattern; Python subclasses inherit through `PyPlayer`.
- If `bind_vector<std::vector<Cell*>>` does not by default expose the mutation API the round-trip needs (`append`, `__delitem__`/`erase`), the binding TU adds explicit `.def("append", …)` / `.def("erase", …)` lambdas — this is one of the surfaces D1.5 exists to discover.

### Python agent (`spikes/pybind/agent.py`)
```python
import PybindSpike as PS

class MyAgent(PS.Player):
    def nextMove(self):
        cs = helper.cells()                     # opaque CellPtrVector view
        assert cs[0].x == 0 and cs[0].y == 0    # initial C++ state visible

        # Mutate the opaque view: erase index 1, then append cs[0] back.
        # This exercises bind_vector mutation API + pointer identity.
        del cs[1]                # or cs.erase(...) if del isn't supported
        cs.append(cs[0])         # appends an existing C++-owned Cell*

        ms = helper.moves()                     # std::list<Move> -> Python list
        assert len(ms) == 2                     # seeded with 2 moves
        assert ms[0].x == 1 and ms[0].y == 2

        return PS.Move(7, 9, PS.Direction.E)

made = MyAgent()
```

### Embedder (`spikes/pybind/main.cpp`)
```cpp
#include <pybind11/embed.h>
#include "domain.hpp"
#include <iostream>

namespace py = pybind11;

int main() {
    py::scoped_interpreter guard{};
    try {
        py::module_::import("sys").attr("path").attr("insert")(
            0, "spikes/pybind");

        // Register types BEFORE injecting C++ objects.
        py::module_ ext = py::module_::import("PybindSpike");

        pybindspike::Helper helper;
        py::module_ main_mod = py::module_::import("__main__");
        py::object  ns       = main_mod.attr("__dict__");
        ns["helper"] = py::cast(&helper, py::return_value_policy::reference);

        py::eval_file("spikes/pybind/agent.py", ns);

        auto* p = ns["made"].cast<pybindspike::Player*>();
        pybindspike::Move m = p->nextMove();

        // Gate assertions (see §6).
        const bool ok =
            m.x == 7 && m.y == 9 && m.dir == pybindspike::Direction::E
            && helper.cells_view.size() == 3
            && helper.cells_view.back() == helper.cells_view.front();
        // The std::list<Move> assertion is exercised inside agent.py (it would
        // raise AssertionError on failure, which py::eval_file surfaces here).

        if (ok) { std::cout << "PYBIND ROUNDTRIP OK\n"; return 0; }
        std::cerr << "PYBIND ROUNDTRIP FAIL\n"; return 1;
    } catch (const py::error_already_set& e) {
        std::cerr << "PYBIND ROUNDTRIP FAIL (python): " << e.what() << "\n";
        return 2;
    }
}
```

The hardcoded path `spikes/pybind/` is relative to the run CWD (project root); identical to how `consola` resolves `lib/Reglas.so`. The agent reads `helper` from the namespace because `eval_file` execs in the same dict it was injected into.

## 6. Gate (exact assertions)

The embedder exits 0 and prints `PYBIND ROUNDTRIP OK` iff **all** of the following hold; any failure → non-zero exit and no gate string.

1. **Mechanism 1 (trampoline + bound value type + enum):** the `Move` returned by `p->nextMove()` has `x == 7`, `y == 9`, `dir == Direction::E` — i.e., the Python override fired from a C++ call site, constructed a bound value type with a bound enum, and the value crossed back into C++ intact.
2. **Mechanism 2 (stl.h by-value path):** the assertion inside `agent.py` that `helper.moves()` produced a Python list of length 2 with the seeded values — failure raises `AssertionError`, surfaces through `py::eval_file` as `py::error_already_set`, and the embedder exits non-zero.
3. **Mechanism 2 (opaque bind_vector + vector<T*> identity):** after the round-trip, C++ observes `helper.cells_view.size() == 3` (started at 3 → Python erased index 1 → Python appended `cs[0]` → 3 again) AND `helper.cells_view.back() == helper.cells_view.front()` (the pointer Python appended is bit-identical to the existing first-element pointer — proves identity, not copy, crossed the boundary).
4. **Mechanism 3 (cross-`.so` + embed + cast back):** the cast `ns["made"].cast<Player*>()` succeeded against a `PybindSpike.so` imported at runtime, and the subsequent virtual dispatch on `p` reached the Python override. The build invariant that `PybindSpike.so` is not linked into `pybind_spike` is verified by `otool -L` post-build (see §7).

## 7. Verification (post-build sanity, before claiming the gate)

Before reporting D1.5 as passing:

```sh
nix develop -c bash -c 'cmake -S . -B build -G Ninja && cmake --build build --target pybind_spike PybindSpike'
otool -L build/spikes/pybind/pybind_spike    | tee /tmp/embedder.lddlike
otool -L build/spikes/pybind/PybindSpike.so  | tee /tmp/extension.lddlike
nix develop -c bash -c './build/spikes/pybind/pybind_spike'; echo "exit=$?"
```

Required invariants:
- `/tmp/embedder.lddlike` lists `libpybindspike_domain.dylib` and `libpython3.11.dylib` (or the pinned variant) but **not** `PybindSpike.so`.
- `/tmp/extension.lddlike` lists `libpybindspike_domain.dylib` and does **not** list `libpython3.11.dylib` (extension uses `-undefined dynamic_lookup` and resolves Python symbols from the embedder at load time).
- The run prints `PYBIND ROUNDTRIP OK` and `exit=0`. Capture `$?` directly — never `| tail` (cerebrum lesson, D1 SIGTRAP misdiagnosis, 2026-05-27).

## 8. Build / env changes (additive — leaves D1 alone)

### `flake.nix`
Add pybind11 to the devShell `packages`. Try `pkgs.python311.pkgs.pybind11` first (ties to the already-pinned interpreter); fall back to top-level `pkgs.pybind11` if that attr does not expose the CMake config files that `find_package(pybind11 CONFIG)` needs. `git add flake.nix` before `nix develop` — flakes only see staged files. `boostPython` and the rest of the existing inputs stay; D1.5 does not load Boost.Python.

### `spikes/CMakeLists.txt`
Append `add_subdirectory(pybind)` after the existing pyembed/engine targets. No changes to pyembed.

### `spikes/pybind/CMakeLists.txt` (new, ~30 lines)
```cmake
find_package(pybind11 CONFIG REQUIRED)

add_library(pybindspike_domain SHARED domain.cpp)
target_include_directories(pybindspike_domain PUBLIC .)

pybind11_add_module(PybindSpike MODULE bindings.cpp)
# pybind11_add_module sets PREFIX "" / SUFFIX ".so" / position-independent code.
# Verify the output name is exactly "PybindSpike.so" (set OUTPUT_NAME if needed).
set_target_properties(PybindSpike PROPERTIES OUTPUT_NAME "PybindSpike")
target_link_libraries(PybindSpike PRIVATE pybindspike_domain)
if(APPLE)
  target_link_options(PybindSpike PRIVATE -Wl,-undefined,dynamic_lookup)
endif()

add_executable(pybind_spike main.cpp)
target_link_libraries(pybind_spike PRIVATE pybindspike_domain pybind11::embed)
add_dependencies(pybind_spike PybindSpike)
```

No source changes outside `spikes/pybind/` and the two lines added to `spikes/CMakeLists.txt` + the `flake.nix` package addition.

## 9. Risks & Mitigations

- **R1 — pybind11 nixpkgs attr path.** `pkgs.python311.pkgs.pybind11` may not expose the CMake config that `find_package(pybind11 CONFIG)` needs. *Mitigation:* try it first; on failure, swap to top-level `pkgs.pybind11`. Both are in nixos-24.11. Reversible inside the same task; not a blocker.
- **R2 — `bind_vector<T*>` mutation API surface.** pybind11's default `bind_vector` may not expose Python `del` / `.append(...)` cleanly for pointer-type vectors. *Mitigation:* add explicit `.def("append", …)` / `.def("erase", …)` lambdas in `bindings.cpp`. This is the kind of friction D1.5 exists to discover and document for the D1′ migration — log the resolution in `.wolf/cerebrum.md`.
- **R3 — `pybind11_add_module` output name.** Some toolchain combinations produce `PybindSpike.cpython-311-darwin.so` instead of `PybindSpike.so`. *Mitigation:* set `OUTPUT_NAME "PybindSpike"` explicitly; verify the actual filename in the build dir before claiming the gate.
- **R4 — `eval_file` namespace semantics differ from boost::python::exec_file.** The agent must read `helper` from the same dict the embedder injected it into. *Mitigation:* use `py::eval_file(path, ns)` where `ns` is `__main__.__dict__`; this matches D1's `ModuloPython.cpp` pattern.
- **R5 — Linker invariants drift.** A subagent might accidentally `target_link_libraries(pybind_spike PRIVATE PybindSpike)` to "fix" symbol resolution, which would break the cross-`.so` invariant silently. *Mitigation:* the §7 `otool -L` check is mandatory; the spec-compliance reviewer subagent must run it and refuse to mark the gate green if `PybindSpike.so` appears in the embedder's link map.

## 10. Success criteria

D1.5 is complete when:

1. The branch builds clean under `nix develop -c cmake --build build` on macOS arm64 with the new pybind11 package in `flake.nix`.
2. `./build/spikes/pybind/pybind_spike` prints `PYBIND ROUNDTRIP OK` and exits 0.
3. `otool -L` invariants from §7 hold.
4. The pyembed and engine spikes still build and pass (additive change — no regression).
5. A PR is opened against `main` (per §5 of `ONBOARDING.md` — repo-local `fabzter` identity; switch active gh account to `fabzter` before push, then switch back).

## 11. Out of scope (deferred to D1′)

- Migrating `src/AgenteWrapper/` and `src/Scripting/` off Boost.Python.
- Removing `boostPython` from `flake.nix`.
- Porting `container_conversions.cpp`'s full set (`Barrera`, `Celda` value, `vector<int>`, `vector<Jugada>`) — D1′ does these per real type.
- Replacing the live `AgentePythonWrapper` indirection — D1′ design decision.
- Any change to game logic, the console runner, or the agents in `bin/`.
