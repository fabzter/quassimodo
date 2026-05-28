# Quassimodo Revival — Session Handover & Operating Guide

> **Read this first.** It lets a fresh session continue the work *exactly* as it has
> been going: same methodology, same subagent discipline, same build playbook, same
> conventions. Pair it with `.wolf/cerebrum.md` (cross-session memory, auto-loaded)
> and the docs under `docs/superpowers/`.

---

## 1. The project in one breath

Quassimodo is a 2010 Quoridor game (C++) where two **Python AI agents** compete. It
uses Irrlicht (3D) + Boost.Python (embeds CPython). It was dormant; we migrated it
hg→git and are reviving it to **build + run natively on macOS arm64** inside an
**isolated, reproducible Nix/Devbox + CMake** environment.

**The greater plan:**
[`docs/superpowers/ROADMAP.md`](docs/superpowers/ROADMAP.md) is the **canonical phase
index** — it defines every phase, reconciles the master spec's **Phases A–E** with the
**D1 / D1.5 / D1′ / D2** breakdown ("the Ds"), and tracks status + which plan implements
each. The master *design* behind it is
[`docs/superpowers/specs/2026-05-26-build-isolation-design.md`](docs/superpowers/specs/2026-05-26-build-isolation-design.md);
per-phase plans live in [`docs/superpowers/plans/`](docs/superpowers/plans/), and
`.wolf/cerebrum.md` records the decisions/gotchas. **When in doubt about the big
picture, read `ROADMAP.md` first, then the spec.** (The roadmap below is a summary of
what's left.)

**Status (2026-05-27):**
- ✅ Hg→git migration done. `main` + `concurso` branches (diverge at the real fork).
- ✅ Build-isolation foundation (PR #1, merged): Nix/Devbox + CMake, two de-risking
  spikes pass — Python/Boost.Python round-trip across a separate `.so`, and an
  IrrlichtMt window on macOS arm64.
- ✅ **Phase D1 (PR #2, branch `feat/phase-d-port`): a full AI-vs-AI match plays in
  the terminal.** All non-graphical modules build; agents ported to Python 3.

**Roadmap (user-confirmed):**
- **D1.5 — pybind11 round-trip spike (NEXT).** De-risk replacing Boost.Python with
  pybind11. **The bar is the LIVE D1 system, NOT the toy `pyembed` spike.** D1's console
  game already runs the *full* Boost.Python topology (`libReglas` + `Reglas.so` +
  `Scripting` embed). The toy `spikes/pyembed/` only proved a sliver (abstract `Widget`,
  returns a `string`, no STL) — do NOT treat it as the spec. The pybind11 spike must
  prove it can reproduce **the exact mechanisms the real bindings already use** (see the
  D1.5 briefing in §8). Build it under `spikes/pybind/`; gate string e.g.
  `PYBIND ROUNDTRIP OK`; add pybind11 pinned to the flake.
- **D1′ — pybind11 migration.** If the spike passes, migrate the bindings
  (`AgenteWrapper`/`Reglas.so` + `Scripting`) off Boost.Python, dropping `boost_python`.
- **D2 — graphical port.** Port `src/Grafico` against IrrlichtMt → graphical playable
  match. Highest risk: the Minetest fork has trimmed Irrlicht's GUI; verify which GUI
  API survived *before* committing. `Aplicacion` (graphical, currently untouched) is
  the D2 entry point.

---

## 2. How we work (the methodology — keep doing this)

**Use the superpowers skills — actually invoke them with the `Skill` tool, exactly as
this work has been done.** Do not merely emulate the workflow from memory: invoke the
relevant skill *before* acting. The `superpowers:using-superpowers` skill (loaded at
session start) is the entry point and states the rule — if there's even a ~1% chance a
skill applies, invoke it. Our toolkit (all via the `Skill` tool):

- `superpowers:brainstorming` — before ANY creative/design work; explore intent, ask
  one question at a time, propose 2–3 options, get decisions.
- `superpowers:writing-plans` — turn a spec into a bite-sized, exact-code plan.
- `superpowers:subagent-driven-development` — **how we build**: fresh implementer
  subagent per task + two-stage (spec, then quality) review.
- `superpowers:executing-plans` — alternative inline execution with checkpoints.
- `superpowers:systematic-debugging` — for ANY bug/crash (this found the SIGTRAP).
- `superpowers:test-driven-development`, `superpowers:verification-before-completion`,
  `superpowers:requesting-code-review` / `receiving-code-review`,
  `superpowers:finishing-a-development-branch`.

When the user types `/<name>` or names a skill, invoke it. Announce "Using [skill] to
[purpose]" when you do. The flow for any non-trivial change:

1. **brainstorming** — explore intent + approach, ask one question at a time
   (`AskUserQuestion`), propose 2–3 options with a recommendation, get decisions.
   Don't start building until the design is agreed.
2. **writing a spec** → `docs/superpowers/specs/YYYY-MM-DD-<topic>-design.md`.
3. **writing-plans** → `docs/superpowers/plans/YYYY-MM-DD-<feature>.md`: bite-sized
   tasks, **exact** file paths + code + commands + a pass/fail gate per task. For a
   *port*, make the known fixes concrete and frame the rest as "compile → fix the
   discovered errors with systematic-debugging → gate."
4. **subagent-driven-development** — execute the plan (see §3).
5. **finishing-a-development-branch** — open a **PR per phase** (we do NOT auto-merge;
   the user merges on GitHub).

Decompose ruthlessly: if a phase has an uncertain/high-risk chunk (e.g. the Irrlicht
GUI), split it into its own later plan and ship the low-risk, value-delivering part
first (this is why D1 = console-only, D2 = graphics).

## 3. Subagent management (the "personality")

- **One implementer subagent per task.** Give it the **full task text + rich context**
  — it has zero prior context. Always include: the working dir + branch, the env
  gotchas (§4), the commit rule (§5), "this is a port — don't change game logic," and
  *why* the task matters. Bake in known gotchas so it doesn't rediscover them.
- **Two-stage review after each substantial task:** spec-compliance reviewer (verify
  it built what was asked, independently — rebuild, run the gate, `otool -L`, etc.),
  then code-quality reviewer. For *trivial* tasks (a verbatim-from-plan file, a clean
  gate), the controller reviews directly instead of spending two subagents — but never
  skip verification.
- **Model selection:** cheap/standard model (sonnet) for mechanical/well-specified
  tasks; the most capable (opus) for integration/debugging/judgment-heavy tasks
  (e.g. the IrrlichtMt derivation, the live console-match integration).
- **Escalation:** tell subagents to report BLOCKED with specifics rather than thrash or
  change game logic to force a compile. If blocked: add context / re-dispatch with a
  stronger model / split the task.
- **Continuous execution:** drive tasks back-to-back; pause only for genuine blockers
  or decisions. Keep a `TaskCreate`/`TaskUpdate` tracker.
- **Watch for aborted-subagent residue:** an interrupted subagent can leave uncommitted
  edits. After an interruption, `git status` and `git checkout -- <files>` / remove
  stray files before continuing.

## 4. The isolated build playbook (critical details)

**Environment:** Determinate Nix + Devbox 0.17.2 + CMake/Ninja, all pinned.
- `flake.nix` pins nixpkgs at commit `50ab793…` (nixos-24.11) → clang 16.0.6, cmake
  3.30.5, ninja 1.12.1, python 3.11.11, boost 1.81 (with python). `flake.lock` committed.
- `devbox.json` pins the SAME versions **explicitly** (`cmake@3.30.5`, …) because
  Devbox 0.17.2's search ignores a global `nixpkgs.commit`; it pulls `boostPython` from
  the flake (`path:.#boostPython`). So `devbox` and `nix develop` give identical tools.
- `nix/irrlicht-fork.nix` builds the **Minetest Irrlicht fork** (IrrlichtMt 1.9.0mt15)
  on darwin (stock nixpkgs `irrlicht` is broken on darwin). It needs zlib/libjpeg/libpng
  + Cocoa/OpenGL/IOKit; ships a CMake config (`find_package(IrrlichtMt CONFIG)` →
  target `IrrlichtMt::IrrlichtMt`; headers under `include/irrlichtmt/`).

**CRITICAL gotchas (every subagent must know these):**
1. `nix`/`devbox` are **NOT on PATH** in non-interactive shells. Prefix every command:
   `. /nix/var/nix/profiles/default/etc/profile.d/nix-daemon.sh && …`. Build via
   `nix develop -c bash -c '...'`. "Git tree is dirty" warnings are harmless.
2. **Nix flakes only see git-tracked/staged files.** After editing `flake.nix` /
   `nix/*.nix`, `git add` them before `nix build`/`nix develop`/`nix flake lock` sees
   the change.
3. **Piping a crashing binary through `| tail` masks its exit code** (you'll see a
   false `exit 0`). Capture to a file and check `$?` directly.

**Python-embedding linkage rules (non-negotiable — proven on macOS):**
- ONE shared `libReglas` linked by everyone → single source of RTTI for cross-boundary
  casts. ONE shared `boost_python` everywhere → single converter registry. Never
  static-link boost_python into the extension.
- The Python extension is a CMake `MODULE` named `Reglas.so`
  (`OUTPUT_NAME "Reglas" PREFIX "" SUFFIX ".so"`), linking `Python3::Module` (headers
  only) + `-undefined dynamic_lookup` on macOS (resolve Python C-API from the host
  process). The embedder links `Python3::Python`.
- Explicit `target_link_libraries` everywhere — macOS two-level namespace forbids the
  implicit load-time symbol resolution the original Linux build relied on.
- The embedder adds `./lib`, `../lib`, `../../lib` to `sys.path` then `import Reglas`,
  so stage `Reglas.so` into a `lib/` dir relative to the run CWD.

**Run the console game:**
```
nix develop -c bash -c 'cmake -S . -B build -G Ninja && cmake --build build'
mkdir -p lib && ln -sf "$PWD/build/src/AgenteWrapper/Reglas.so" lib/Reglas.so
nix develop -c bash -c './build/src/Consola/consola bin/agenteCamina.py bin/agenteMiniMax2.py </dev/null'
# → renders the board each turn, ends with "Hay un ganador!"
```

**Porting lesson:** old C++ that "worked" under 2010 gcc can be latent UB under modern
clang. Example: `bool Partida::siguienteJugada()` had no `return` → clang emits
`brk #0x1` → SIGTRAP at runtime. Fix the *recovered/ported* code minimally; never
change game logic to force a compile.

## 5. Conventions & preferences (hard rules)

- **NO AI/LLM attribution anywhere** — no "Co-Authored-By: Claude", no "Generated with
  …", nothing, in commits/PRs/any artifact.
- **Git identity is pinned repo-local** to `fabzter <faboster@gmail.com>` (another
  process on this machine mutates the global git/gh config). Plain `git commit`.
- **GitHub:** repo is `github.com/fabzter/quassimodo`. To push, switch the active gh
  account to `fabzter` (`gh auth switch --user fabzter`), push/PR, then **switch back**
  to `fabriziohernandez`. Verify with `gh api user --jq .login` before pushing.
- **Don't bend the current solution to fit recovered/old code** — adapt the old code to
  the current modules (e.g. the recovered `Ejecutable` → `src/Consola/`).
- **Commit per task**, focused messages, never amend. Don't commit `build/` artifacts.
- **OpenWolf:** this repo uses OpenWolf (`.wolf/`). Honor `.wolf/cerebrum.md`
  (learnings/decisions/do-not-repeat) and log bugs. Volatile `.wolf` files
  (`memory.md`, `anatomy.md`, `_session.json`, `token-ledger.json`, etc.) are gitignored;
  `cerebrum.md`/config/hooks are tracked.

## 6. Key files

- `docs/superpowers/specs/2026-05-26-build-isolation-design.md` — the design spec.
- `docs/superpowers/plans/2026-05-26-build-isolation-foundation.md` — foundation plan (done).
- `docs/superpowers/plans/2026-05-27-phase-d1-console-playable.md` — D1 plan (done).
- `flake.nix` / `flake.lock` / `devbox.json` / `nix/irrlicht-fork.nix` — the pinned env.
- `BUILDING.md` — how to enter the env + build/run.
- `.wolf/cerebrum.md` — accumulated learnings, decisions, gotchas (read it).
- `src/` — `Reglas` (rules), `Opciones`, `Scripting` (embeds py), `AgenteWrapper`
  (`Reglas.so`), `Consola` (console runner), `Grafico` + `Aplicacion` (graphical, D2).

## 7. Immediate next action

**D1.5 — the pybind11 round-trip spike.** Read §8 (the briefing) — it is the spec for
this spike. Then: brainstorm any open type/scope choices with the user (one question at
a time), write a short plan, build it under `spikes/pybind/` subagent-driven, gate on
`PYBIND ROUNDTRIP OK`, open a PR. If it passes, plan D1′ (migrate the real bindings).
Use the §2–§5 methodology and conventions throughout.

## 8. D1.5 briefing — what the pybind11 spike must actually prove

**Framing (do not get this wrong):** the `spikes/pyembed/` Boost.Python spike is a
**toy** (abstract `Widget`, returns a `string`, *no STL*) — it is NOT the bar. The bar
is the **live D1 system**: the console game already runs the full Boost.Python topology
(`libReglas` shared + `Reglas.so` separate extension imported at runtime + `Scripting`
embedder), exercising real mechanisms. D1.5 must prove **pybind11 can reproduce exactly
those mechanisms**, because D1′ will swap them out. Read the real bindings first:
`src/AgenteWrapper/{Agente.cpp,container_conversions.cpp,main.cpp}`,
`src/Scripting/ModuloPython.cpp`, `src/Reglas/{Agente,Jugada,Celda,AyudanteDeAgente}.hpp`.

**The three mechanisms, AS THE REAL BINDINGS USE THEM** (each is a concrete D1′ risk):

1. **Pure-virtual trampoline returning a BOUND VALUE TYPE.** `Reglas::Agente` is
   abstract with `Jugada siguienteJugada()`, `void iniciar(int)`, `void terminar()`.
   Python agents subclass `Reglas.Agente` and override them; C++ calls them and gets a
   `Jugada` (a *bound value type*, not a primitive) constructed in Python.
   → pybind11: trampoline + `PYBIND11_OVERRIDE_PURE(Jugada, Agente, siguienteJugada)`.
   Prove the override fires from C++ and a bound value type crosses Py→C++.

2. **STL converters of BOUND types, BOTH directions.** `container_conversions.cpp`
   registers `std::list<Jugada>`, `std::list<Barrera>`, `std::vector<int>`,
   `std::vector<Celda>`, **`std::vector<Celda*>`** (pointers!), `std::vector<Jugada>`
   with to-python (→tuple) + from-python (sequence) converters (Boost
   `vector_indexing_suite`). `AyudanteDeAgente` hands the agent these lists.
   → pybind11: `pybind11/stl.h` (by-value list↔vector/list) and/or `py::bind_vector` +
   `PYBIND11_MAKE_OPAQUE` for index-suite-style mutable containers. Prove containers of a
   *bound struct* cross both ways, **including a container of pointers** (the hardest:
   identity/lifetime). Note pybind11 `stl.h` copies by value where Boost gave mutable
   views — confirm that's acceptable for how agents use them (they mostly *read* the
   possible-move lists).

3. **Embed + cross-`.so` registry + `extract` the subclass back.** `ModuloPython`
   (in `libScripting`) injects live C++ objects into the agent namespace
   (`object(ptr(&tablero))`, a `new AyudanteDeAgente`), execs the agent `.py`, then
   `extract<Reglas::Agente*>(instance)` pulls the Python subclass instance back as a C++
   pointer — all against `Reglas.so` imported at runtime, RTTI single-sourced via shared
   `libReglas`.
   → pybind11: `pybind11/embed.h` (`scoped_interpreter`, `module_::import`), push C++
   objects via `py::cast(ptr, return_value_policy::reference)`, pull the subclass back via
   `obj.cast<Agente*>()`, with the extension a **separate `.so` imported at runtime**
   (NOT linked into the embedder); pybind11's registry is shared automatically via the
   interpreter.

**Type guidance (the spike's domain model):** use **minimal stand-ins that carry the
real shapes** — neither the toy `Widget` nor the heavy real Reglas types. Suggested:
`Move{int x,y;}` (≈`Jugada`, bound value type), `Cell{...}` (≈`Celda`, bound class),
abstract `Player{ virtual Move nextMove()=0; }` (≈`Agente`), and a `Helper` that returns
`std::vector<Cell>` / `std::vector<Cell*>` / `std::list<Move>` to the agent (≈`Ayudante`).
Wire them into ONE round-trip mirroring the live flow: C++ injects a `Helper` → Python
agent subclass reads its STL containers and returns a `Move` → C++ pulls the agent back
as `Player*` and calls `nextMove()`, receiving the bound `Move`.

**Topology + build (same rules as the proven spikes, §4):** ONE shared domain lib
(single RTTI); the pybind11 extension a CMake `MODULE` `.so` (`-undefined dynamic_lookup`
on macOS, `Python3::Module`), imported at runtime; embedder links `Python3::Python`. Add
**pybind11 pinned to `flake.nix`** (`pkgs.python311.pkgs.pybind11` or the `pybind11`
attr; `git add` the flake before `nix develop`). Gate: assert each of the three, print
`PYBIND ROUNDTRIP OK`. Then a PR.
