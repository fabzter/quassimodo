# Build Isolation & Native macOS Build — Design Spec

- **Date:** 2026-05-26
- **Status:** Approved (design); pending spec review
- **Project:** Quassimodo (Quoridor game, C++ / Irrlicht / Boost.Python)

## 1. Background

Quassimodo is a dormant 2009–2010 C++ game just migrated from Mercurial to Git.
It is a Linux/X11/OpenGL desktop app: an Irrlicht 3D front-end, a rules engine,
and an **embedded CPython interpreter** that loads user-written AI agents via a
Boost.Python extension module (`Reglas`). The original build targeted Ubuntu
10.04-era libraries (Boost 1.40, Python 2.6, Irrlicht 1.6) using NetBeans-generated
Makefiles.

We are reviving it on **macOS, Apple Silicon (arm64)**.

## 2. Goal & Non-Goals

### Goal
Revive Quassimodo so the **real game source compiles, links, and runs a playable
match natively on macOS arm64**, inside an isolated, reproducible, host-independent
build environment. This explicitly includes the source modernization required to
compile under a modern toolchain (Python 2.6 → 3, Boost.Python API, Irrlicht fork API).

### Phases (sequenced in the implementation plan)
- **A. Isolated env** — Devbox/Nix pinned toolchain + deps; CMake skeleton.
- **B. Engine spike** — Irrlicht (Minetest fork) builds under the env and opens a native window.
- **C. Python round-trip spike** — embedded Python + Boost.Python C++↔Python crossing works.
- **D. Source port** — modernize the real modules until they compile and link into the
  shared libs + `Reglas` extension + `aplicacion` executable.
- **E. Runtime bring-up** — until a full match is playable (console path first as a
  logic check, then the native Irrlicht GUI).

Spikes **B** and **C** are early gates: if either fails, stop and revisit the approach
before investing in the port (D).

### Non-Goals
- x86_64 / Linux builds — the design must not preclude them, but they are not validated here.
- Packaging a distributable `.app` / dependency bundling for end users.
- CI build runners (e.g. `tart` macOS VM) — optional later phase.
- Gameplay/feature changes beyond what is needed to build and run.

## 3. Target & Constraints

- **Primary target:** native macOS arm64 (Apple Silicon). Real Cocoa window — no VNC.
- **Ultimate targets:** arm64 + x86_64 (macOS and Linux). Design must keep these reachable.
- **Isolation requirement:** the build must not be affected by the host's Homebrew /
  system installs / env. The whole toolchain *and* dependencies are pinned.
- **No macOS containers exist** (OS-level fact); native isolation = a pinned dependency
  closure, not virtualization. (A full macOS VM via `tart` is the only OS-level
  isolation and is out of scope here.)

## 4. Isolation Mechanism

**Devbox (Nix-backed) + CMake.**

- **Devbox** (`devbox.json` + `devbox.lock`) pins the entire toolchain and all
  dependencies to an exact nixpkgs revision. `devbox shell` enters an environment
  where only the pinned tools are visible; Homebrew/system installs are invisible.
  Verifiable: `which clang`, `which cmake` resolve into the Nix store.
- **Reproducible:** `devbox.lock` committed → a fresh checkout reproduces the
  identical environment on any arm64 macOS (and, later, x86_64 / Linux).
- **Native:** builds and runs natively on macOS; Nix shims to the host Apple SDK for
  the system frameworks a native GUI app requires (Cocoa, OpenGL/Metal) — these are
  intentionally *not* isolated, because a native Mac app must use them.
- **CMake + Ninja** is the build system, replacing the NetBeans Makefiles. One
  cross-platform definition targets macOS now and x86_64/Linux later.

Rationale: Nix is the only option that isolates the *whole toolchain* (compiler +
deps) reproducibly while still producing a native binary. Devbox provides a gentler
UX over raw Nix. CMake is mandated and is the portable, multi-arch build standard.

## 5. Dependency Modernization Map

| Concern | Original (2010) | Modern (this env) |
|---|---|---|
| Build system | NetBeans Makefiles | CMake + Ninja |
| Compiler | gcc/g++ | clang (Nix-provided, host SDK) |
| 3D engine | Irrlicht 1.6 (X11) | **Irrlicht fork (Minetest's), darwin-capable** via custom Nix derivation |
| Python | 2.6 (embedded) | Python 3 (nixpkgs) |
| Bindings | Boost.Python (py26) | Boost.Python for Python 3 (single shared lib) |
| Boost libs | 1.40, `-mt` tag | current Boost (no `-mt` suffix) |
| Windowing | X11 + libGL + libXxf86vm | native Cocoa + system GL/Metal |

Note: stock nixpkgs `irrlicht` is marked **broken on Darwin** (last successful build
2023). The Minetest Irrlicht fork *does* build on macOS (Luanti ships on darwin/arm64
in nixpkgs) but is not a standalone package → we provide it via a small custom Nix
derivation modeled on Luanti's build. **Engine choice confirmed: Minetest Irrlicht fork.**

## 6. Build & Linkage Architecture (the critical part)

The original "works" partly via Linux loader behavior macOS will not tolerate. These
rules are **non-negotiable** and baked into the CMake design.

### Linkage topology
```
aplicacion (embeds CPython: Py_Initialize)
 ├─ libReglas      ← C++ domain types (Tablero, Celda, Agente…)  [single RTTI source]
 ├─ libGrafico     → Reglas, Irrlicht, GL
 ├─ libScripting   → python3, boost_python (shared); uses Reglas:: types
 └─ libOpciones
Reglas (Boost.Python MODULE → Reglas.so)
 ├─ libReglas      ← same shared lib
 └─ boost_python   ← same shared lib
```
At runtime: `Py_Initialize()` → `import Reglas` → agents subclass `Reglas.Agente`.
Live C++ objects cross C++→Python (`object(boost::python::ptr(&tablero))`, a
`new AyudanteDeAgente`) and the agent instance crosses Python→C++.

### Hazards observed in the original
1. **Dual Boost.Python registry:** `Reglas.so` statically links boost_python
   (`-Bstatic -lboost_python`) while `libScripting` links it dynamically → two
   registries. Survived on Linux via global symbol visibility; **fails on macOS**.
2. **Implicit symbol resolution:** `libScripting` uses `Reglas::` symbols without
   linking `-lReglas`, relying on Linux load-time resolution. macOS two-level
   namespace forbids undefined symbols.
3. **No `setdlopenflags(RTLD_GLOBAL)`** — a Linux-ism that was a band-aid for (1).

### Rules
1. **One shared Boost.Python everywhere.** Never static-link into the extension.
   Both `Reglas` and `libScripting` link the same `libboost_python` → single registry
   by construction.
2. **`libReglas` is a single shared lib** linked by all consumers → single-source RTTI
   for cross-boundary casts.
3. **Explicit dependencies** in CMake (`target_link_libraries`) — no reliance on
   implicit load-time resolution.
4. **The extension is a CMake `MODULE`** producing `Reglas.so`, built with
   `-undefined dynamic_lookup` so Python C-API + registry symbols bind to the host
   process's already-loaded dylibs.
5. **Registry sharing is achieved by a single shared boost_python + explicit linking.**
   `RTLD_GLOBAL` is a Linux-only contingency, never a planned step.
6. The Python-embedding **round-trip spike** (§7) must pass before the source port (D).

## 7. Risks & De-Risking Spikes (front-loaded)

These two spikes are the make-or-break of the whole revival and run **first**.

- **Engine spike (B):** get the Minetest Irrlicht fork building under Devbox/Nix on
  macOS arm64 and open a blank window. If unachievable, pivot early (patch upstream
  irrlicht, or evaluate an alternative engine) — the game uses Irrlicht scene nodes,
  GUI, and `.3ds`/`.x` mesh loaders, so staying on an Irrlicht fork minimizes rewrite.
- **Python round-trip spike (C):** a minimal embedder that `Py_Initialize()`s, builds a
  tiny Boost.Python module exposing one C++ class, pushes a C++ object into Python and
  pulls a Python-subclass instance back into C++ — proving registry + RTTI sharing on
  macOS arm64 with the §6 rules.

If either spike fails, we stop and revisit before sinking effort into the port (D).

## 8. Repo Layout (new files)

```
/devbox.json              # pinned toolchain + deps
/devbox.lock              # reproducibility lock (committed)
/flake.nix                # if a Devbox-generated flake is used (decided in the plan)
/nix/irrlicht-fork.nix    # custom derivation for the Minetest Irrlicht fork on darwin
/CMakeLists.txt           # top-level build
/src/<module>/CMakeLists.txt   # per-module (Reglas, Grafico, Scripting, Opciones, Aplicacion, AgenteWrapper)
/cmake/                   # toolchain/helper modules if needed
/spikes/                  # engine-window + python-roundtrip smoke programs
/BUILDING.md              # how to enter the env and build
```

The existing NetBeans `nbproject/` makefiles are left in place initially (reference);
they are removed once CMake fully replaces them.

## 9. Success Criteria (definition of done)

1. `devbox shell` enters a fully pinned environment; `which clang`/`cmake` resolve into
   the Nix store (host tools provably unused). `devbox.lock` committed; a fresh clone
   reproduces the identical environment.
2. **Engine spike** builds and opens a native window on macOS arm64.
3. **Python round-trip spike** passes (C++ ↔ Python object crossing under the §6 rules).
4. **All real modules compile and link** under the isolated env into their shared libs
   (`libReglas`, `libGrafico`, `libScripting`, `libOpciones`), the `Reglas` extension,
   and the `aplicacion` executable.
5. The app **runs and a full match is playable** on macOS arm64 — an AI-vs-AI match
   completes via the console path, and the native Irrlicht GUI renders a playable match.

## 10. Open Questions / Future

- **x86_64 / Linux:** validate the same CMake/Devbox env on those targets; `RTLD_GLOBAL`
  contingency only if a visibility edge case appears.
- **CI / reproducible runners:** optional `tart` macOS VM layer later.
- **Distribution:** `.app` bundling / `install_name_tool` rpath fixup — out of scope now.
