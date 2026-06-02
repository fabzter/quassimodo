# Quassimodo Revival — Phase Roadmap

> Canonical map of the revival phases: what each is, its status, and which
> spec/plan implements it. The **master design** is
> [`specs/2026-05-26-build-isolation-design.md`](specs/2026-05-26-build-isolation-design.md)
> (it defines the original arc as **Phases A–E**). This file is the living index and
> reconciles the `A–E` scheme with the `D1/D1.5/D1′/D2` breakdown that evolved during
> execution. Per-phase implementation plans live in [`plans/`](plans/).

## How the naming fits together

The master spec laid out five phases for the whole revival:

| Spec phase | Meaning |
|---|---|
| **A** | Isolated, reproducible build env (Nix/Devbox + CMake) |
| **B** | Engine spike — IrrlichtMt opens a window on macOS arm64 |
| **C** | Python round-trip spike — embedded Python + Boost.Python across a separate `.so` |
| **D** | Source port — modernize the real modules (Py 2.6→3, Boost/Irrlicht API) until they compile, link, and run |
| **E** | Runtime bring-up — a playable match |

In practice **A–C** were delivered together (the "foundation"), and **D + E** are too
large/risky for one pass, so they're delivered incrementally. The incremental units are
named **D1 → D1.5 → D1′ → D2**. (So "the Ds" are the source-port-and-beyond slices of
spec phases D and E.)

## Phase status & plans

| Phase | What | Status | Artifact |
|---|---|---|---|
| **A–C — Foundation** | Isolated Nix/Devbox + CMake env; engine-window spike; Python round-trip spike | ✅ Done | [plan](plans/2026-05-26-build-isolation-foundation.md), **PR #1 (merged)** |
| **D1 — Console-playable** | Port non-graphical modules (Reglas, Opciones, Scripting, AgenteWrapper/`Reglas.so`) + agents to Py3; recover the `Ejecutable` console runner as `consola`. Delivers spec-D (non-graphical) **and** spec-E (a console match plays). | ✅ Done | [plan](plans/2026-05-27-phase-d1-console-playable.md), **PR #2 (merged)** |
| **D1.5 — pybind11 spike** | De-risk replacing Boost.Python with pybind11. ... | ✅ Done | [plan](plans/2026-05-27-phase-d1.5-pybind-spike.md), **PR #3 (merged)** |
| **D1′ — pybind11 migration** | Migrate `AgenteWrapper`/`Reglas.so` + `Scripting` bindings off Boost.Python; drop the `boost_python` dependency. | ✅ Done | [plan](plans/2026-05-28-phase-d1prime-pybind11.md), **PR #4** |
| **D2.0 — Render spike** | IrrlichtMt API recon: 3DS→OBJ asset pipeline; `getMesh(IReadFile*)`; materials; dynamic-light + animator API removals confirmed. Scene from real `Reglas::Tablero` rendered and a real `moverJugador` move reflected on screen. | ✅ Done | **PR #7 (merged)** |
| **D2.1 — Minimal graphical match** | Lean argv-driven graphical match (`aplicacion agentA.py agentB.py`): IrrlichtMt window, textured 3D board + 81 cells + two pawns + static torches, full AI-vs-AI match to "Hay un ganador!". Menu/animation/GUI deferred (KB-D2-001..006). | ✅ Done | **PR #8 (this PR)** |
| **D2.2 — Animation + full menu/GUI** | Tweened piece/barrier moves + orbit camera (re-implement animator subsystem as per-frame helpers); rebuild window/msgbox GUI with composed primitives; cosmetic cleanup (KB-D2-001..006). | ⬜ **NEXT** | plan: TBD |

## Decisions behind the roadmap

- **pybind11 over Boost.Python** (user preference: header-only, drops the large
  `boost_python` dep). Sequenced *after* D1 so we ship a working console game first,
  then swap the binding layer against a known-good baseline. pybind11 replaces
  `boost_python` only; other Boost components (filesystem/system/thread/program_options)
  remain unless a separate "go fully Boost-free" decision is taken.
- **Engine = Minetest Irrlicht fork (IrrlichtMt)** — stock nixpkgs `irrlicht` is broken
  on darwin. The fork's GUI trimming is the main D2 risk.
- **Console before graphics** — the console path is independent of Grafico, so it
  delivers a playable game early and isolates the high-risk GUI work into D2.

See `.wolf/cerebrum.md` (Decision Log) for the full rationale and gotchas.
