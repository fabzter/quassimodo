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

**Status (2026-06-02) — always re-derive from `git log` + `ROADMAP.md`; this is a snapshot:**
- ✅ Hg→git migration; build-isolation foundation (PR #1); **D1** console AI-vs-AI match (PR #2).
- ✅ **D1.5** pybind11 round-trip spike (PR #3) — proved trampoline + STL + cross-`.so` embed.
- ✅ **D1′** pybind11 migration (PR #4) + regression fixes (PR #6); `boost_python` dropped.
  Pre-existing bugs catalogued in **`KNOWN-BUGS.md`** (KB-001 Minimax SIGSEGV, etc.).
- ✅ **D2.0** render spike (PR #7) — proved the `.3ds`→`.obj` (assimp) asset pipeline and
  rendered a real `Reglas::Tablero` + a real move on screen; surfaced the big IrrlichtMt
  API removals (no 3DS loader, `getMesh(IReadFile*)` only, no `setMaterialFlag`/material
  flags, no dynamic lights, no animator subsystem, no `addWindow`/`addMessageBox`).
- ✅ **D2.1** minimal graphical match (PR #8) — `aplicacion agentA.py agentB.py` plays a
  full AI-vs-AI match in an IrrlichtMt window to "Hay un ganador!" with *instant* moves
  (animator subsystem was deleted from the fork). Lean driver reuses `PartidaGrafica`;
  menu/`Aplicacion`/`ManejadorJuego`/`EventReceiver` excluded. The PR comment is a full
  **"equivalency compromise ledger"** of everything lost to the fork (read it before D2.2).

**Roadmap (see `ROADMAP.md` for the live table):** D2 was split into **D2.2a** (the
"look & feel" pass — animation + camera + atmosphere) and **D2.2b** (the "interactivity"
pass — menu/GUI + input + interactive camera). D2.2a is itself two milestones:
- **D2.2a·M1 — movement & framing (NEXT — plan written, not executed).** Re-implement the
  deleted animator as a general `Animador` (tween manager) + an **eager** command/event
  queue (`EventoJugada`) + a dt driver loop → animated pawn jumps + barrier slides,
  restored move-pacing, a static 3/4 camera behind a `CamaraController` seam. **The full
  7-task plan is written & committed (see §7).**
- **D2.2a·M2 — atmosphere & polish** (billboard flames, blob shadows, ground+sky, cosmetic).
- **D2.2b — interactivity** (menu/GUI rebuilt from primitives, input, interactive camera).

---

## Operating character — *behave like the prior sessions* (read before §2)

§2–§5 are the *mechanics*. This is the *judgment* — hold it so you act as a continuation
of the same mind, not a fresh contractor doing one ticket.

- **Hold the whole arc, not the task.** You are mid-revival of a 16-year-dormant game;
  the destination is a maintainable, reproducible, **playable** Quassimodo on modern
  macOS (and cross-platform). Re-derive the big picture from `ROADMAP.md` + the spec
  before diving, and weigh each move against that destination — don't tunnel on the
  immediate task. When a task's framing smells off, step back to the arc (that's how the
  D1.5 mis-framing got caught).
- **Truth over agreement — verify, don't assume, push back when warranted (even on the
  user).** No sycophancy, no reflexive "you're absolutely right." This project's good
  moments were corrections: the user fixed a wrong "convert in place" detail of mine; a
  subagent corrected a stale assumption of mine; we reframed D1.5 when it was wrong. If
  evidence disagrees with the user, say so with the evidence.
- **Evidence before claims.** Never trust a subagent's report or a tool's surface
  signal. Rebuild, run the gate, `otool -L`, read the *actual* exit code (a `| tail`
  pipe once masked a crash as `exit 0`). "It builds/works" requires having watched it.
- **Confirm the genuine forks; decide the rest.** Use `AskUserQuestion` (one at a time,
  with a recommendation) for real, hard-to-reverse decisions (binary strategy, pybind11,
  repo visibility/account). For reversible or obvious calls, just proceed. Don't
  over-ask; don't under-ask on irreversible things.
- **Care with irreversible / host / destructive actions.** Get consent before host
  installs (Nix), creating a public repo, force/reset/delete; keep safety nets (we kept
  the `.hg` clones until the migration was verified); **verify the active gh account
  before every push**.
- **Decompose ruthlessly; ship value early; front-load risk.** Split the uncertain
  chunk into its own phase and deliver the low-risk playable slice first (console before
  graphics; spikes before ports). A working thing now beats a perfect thing later.
- **Curate subagents, don't delegate blindly.** Self-contained context per task, bake in
  the gotchas (§4), right-size the model, demand *BLOCKED-not-thrash*, watch for
  aborted-subagent residue, review (controller for trivial, two-stage for substantial).
- **Keep memory alive.** Record decisions/gotchas in `.wolf/cerebrum.md`, keep
  `ROADMAP.md`/`ONBOARDING.md` current, log bugs. The next session — a fresh *you* —
  depends on it; this handover is itself an instance of that discipline.
- **User conventions are hard constraints** (§5: no AI attribution, repo-local `fabzter`
  identity, …) — they override any default behavior.

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

### Anti-drift discipline (proven across D1.5 / D1′ / D2 — this is *the* thing that worked)

- **Inline the full, corrected code into the prompt — never "go read the file."** The
  implementer has zero context. Paste the exact code/edits, exact commands, exact expected
  output, and the per-file `file:line` of what to change. (The deepseek D2 attempt failed
  precisely because it "compiled and triaged" with thin context and improvised mocks.)
- **Verify every API claim against the real headers BEFORE baking it into a prompt, then
  propagate.** D2 was a minefield of IrrlichtMt removals (`getMesh` sig, `setMaterialFlag`,
  dynamic lights, the whole animator subsystem, `addWindow`). Each was *discovered by a
  task, verified by the controller against `$IRR/include/irrlichtmt/*.h`, then folded into
  the NEXT task's prompt* so the next subagent never rediscovered it. Don't trust a
  subagent's API claim — `grep` the header yourself.
- **Adaptive re-planning between tasks.** Each task's findings reshape the remaining tasks.
  After a task returns, patch the downstream plan (and commit the patch) before dispatching
  the next subagent. Plans are living, not frozen.
- **Verify gates with your OWN senses — evidence before claims.** Rebuild it yourself, run
  it yourself (capture `$?`, never `| tail`), `otool -L` yourself, and for any visual gate
  **Read the actual screenshot PNG with your own vision** — never accept "it renders" on a
  subagent's word. (Screenshots are transient: env-guard their capture, don't commit them.)
- **Front-load latent risk.** Before a risky integration task, sweep for known traps
  proactively — e.g. a `-Wreturn-type` scan caught fall-off-the-end `bool` functions
  (the D1 SIGTRAP class: clang emits `brk #0x1`) and they were fixed *before* the driver
  task hit them at runtime. Fix the minimal missing thing; never change game logic.
- **Recon before planning a big/uncertain port.** Fan out read-only recon subagents to map
  the real API-drift surface (`file:line` of every offending call) so the plan is grounded
  in evidence, not assumptions. (Three parallel recon agents mapped D2.1 before a line was
  written.)
- **Keep the working tree clean for the next subagent.** `.wolf/buglog.json` auto-churns
  every action; commit it (or `git checkout --` it) so it never gets swept into a source
  commit, and always use **explicit `git add <files>`**, never `git add -A`.

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
- `KNOWN-BUGS.md` — curated registry of unresolved confirmed defects (e.g. the
  C++ minimax MOVIMIENTO SIGSEGV, the Camina `SinHijo` edge case). Read it
  before touching code in an affected area; add an entry when you surface a
  bug you can't or shouldn't fix in your current PR.
- `src/` — `Reglas` (rules), `Opciones`, `Scripting` (embeds py), `AgenteWrapper`
  (`Reglas.so`), `Consola` (console runner), `Grafico` + `Aplicacion` (graphical, D2).

## 7. Immediate next action

**D2.2a · Milestone 1 — execute the written plan.** The design is already brainstormed,
agreed, spec'd, and planned (no need to re-brainstorm M1):
- **Spec:** `docs/superpowers/specs/2026-06-02-d2.2a-animation-camera-atmosphere-design.md`
  (read the whole thing; M1 is the "movement & framing" milestone, M2 is atmosphere).
- **Plan:** `docs/superpowers/plans/2026-06-02-phase-d2.2a-m1-movement-framing.md` — a
  complete 7-task plan with full code for the new classes (`Animador`, `Animaciones`
  [`SaltoAnim`/`DeslizarAnim`], `EventoJugada`, `CamaraController`) and exact edits to
  `PartidaGrafica`/`Pieza`/the driver, headless self-tests + a **vision gate** (a pawn
  caught mid-jump between cells is the proof moves animate, not teleport).
- **Do:** invoke `superpowers:subagent-driven-development` and execute the plan task-by-task
  (Sonnet for the new-class/mechanical tasks 1,2,4,7; Opus for the `PartidaGrafica`
  refactor + driver + vision gate 3,5,6). Cut `feat/d2.2a-m1-animation` from `main`
  (Task 1 does this). Verify each gate yourself (rebuild, run, **read the screenshots
  with your own eyes**). PR per milestone — M1 then, separately, M2.
- Key facts the plan relies on (already verified against the fork headers): per-frame time
  = `device->getTimer()->getTime()`; `Pieza::setPosicion(int,int,int)` truncates to int so
  the plan adds `setPosicionSuave` for smooth tweens; agents for the gate = the proven
  clean pair `bin/agenteCamina.py bin/agenteCamina2.py` (Minimax SIGSEGVs, KB-001).

After M1 ships: plan + execute **D2.2a·M2** (atmosphere & polish), then **D2.2b**
(interactivity). The original `D1.5 briefing` below (§8) is historical (D1.5 shipped as PR #3).

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
