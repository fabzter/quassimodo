# Known Bugs

> Live registry of confirmed but unresolved defects in Quassimodo. Read this
> alongside [`ONBOARDING.md`](ONBOARDING.md) when you pick up debugging work —
> these are the dragons. Intended for both human developers and AI agents.
>
> This file is curated and narrative. The companion machine-readable log at
> [`.wolf/buglog.json`](.wolf/buglog.json) collects OpenWolf hook-detected
> entries (refactors, type/symbol mismatches, etc.); for substantive bugs that
> deserve "why" and "what to do next," **this doc is the canonical record**.
>
> **When you find a defect you can't or shouldn't fix in your current PR,**
> add it here with the template below. **When you fix one,** move it to the
> "Fixed" section with the resolving commit + PR — don't delete it; the
> history of what was once broken is useful for the next person reading code
> in that area.

## Status legend

- 🔴 **OPEN** — confirmed defect, no decision yet on when to fix.
- 🟡 **DEFERRED** — confirmed defect, intentionally not fixed yet; reasons in the entry.
- 🟢 **FIXED** — resolved; commit + PR linked.

---

## 🟡 KB-001 — C++ `Reglas::Minimax` MOVIMIENTO branch SIGSEGVs

**Buglog:** [`bug-020`](.wolf/buglog.json)
**Surfaced by:** [PR #6](https://github.com/fabzter/quassimodo/pull/6) (D1′ fixes)
**Self-documented since:** 2010 (hg commit `1314ada`: *"Casi se termina de implementar el minimax en c++. Hay fuga de memoria y no funca muy bien si se está haciendo MOVIMIENTO"*).
**Affected:** `bin/agenteMiniMax2.py`, `bin/agenteMiniMax4.py` against barrier-placing opponents.

### What breaks

`consola bin/agente_astar.py bin/agenteMiniMax2.py </dev/null` SIGSEGVs (exit 139) ~448 lines of board renders into the match. The crash is inside the C++ `Reglas::Minimax` recursion when called with `TipoDeJugada.MOVIMIENTO`.

### Why this surfaced *now*

PR #6 fixed the A* magic-82 NO_SOLUTION-as-SOLVED hack ([`9ccd53c`](https://github.com/fabzter/quassimodo/commit/9ccd53c)). With honest `NULL`-on-no-path semantics, `MiniMax2`'s `IndexError` fallback fires more often → routes through `Reglas.minimax(TipoDeJugada.MOVIMIENTO)` → exercises the broken 16-year-old recursion → crash. The bug was always there; the old A* hack was incidentally masking it by making the fallback rare.

### Repro

```sh
. /nix/var/nix/profiles/default/etc/profile.d/nix-daemon.sh && \
  nix develop -c bash -c 'timeout 30 ./build/src/Consola/consola \
    bin/agente_astar.py bin/agenteMiniMax2.py </dev/null' \
  > /tmp/repro.out 2> /tmp/repro.err
echo "exit=$?"  # 139
tail -1 /tmp/repro.err  # "Segmentation fault: 11"
```

### What works

`Camina vs Camina`, `Camina vs astar`, `astar vs astar` all complete cleanly. Failure is specific to the MiniMax* agents calling the C++ `minimax()` fallback.

### Next steps for whoever picks this up

1. Build debug symbols and run under `lldb` to capture the stack trace at the SIGSEGV site:
   ```sh
   nix develop -c bash -c 'cmake -S . -B build-dbg -G Ninja -DCMAKE_BUILD_TYPE=Debug && cmake --build build-dbg'
   nix develop -c bash -c 'lldb -- ./build-dbg/src/Consola/consola bin/agente_astar.py bin/agenteMiniMax2.py'
   # in lldb: `run < /dev/null`, then `bt` when it crashes
   ```
2. Review [`src/Reglas/Minimax.cpp`](src/Reglas/Minimax.cpp) lifetime + ownership for the `MOVIMIENTO` branch — the 2010 commit log flagged a memory leak; the SIGSEGV is most likely use-after-free or null-deref in the recursion.
3. Cheap short-term mitigation if a real fix is out of reach: change `bin/agenteMiniMax2.py` and `bin/agenteMiniMax4.py`'s `IndexError` fallbacks to use `random.choice(ayudante.getMovimientosPosibles(self.id))` (same pattern `bin/agente_astar.py` already uses successfully). Sidesteps the broken C++ minimax until it's debugged.

---

## 🟡 KB-002 — `bin/agenteCamina.py:36` unguarded `getHijo()` raises `SinHijo`

**Buglog:** [`bug-021`](.wolf/buglog.json)
**Surfaced by:** [PR #6](https://github.com/fabzter/quassimodo/pull/6) (D1′ fixes)
**Affected:** `bin/agenteCamina.py` against any barrier-placing opponent.

### What breaks

`consola bin/agenteCamina.py bin/agenteMiniMax2.py </dev/null` exits 1 (clean — caught by the top-level catch added in PR #6 fix #2; **no longer SIGABRT**) with `Unhandled C++ exception (...error_already_set...): SinHijo: <EMPTY MESSAGE>` at `bin/agenteCamina.py(36)`.

### Root cause

Camina is a naive walker. On line 36:

```python
if(celdaActual.getHijo(self.direccion).estaLibre()):
```

It calls `getHijo(self.direccion)` with **no guard** for cells where that direction has no child. `Celda::getHijo` throws `Reglas::SinHijo` when the cell is at the board edge in that direction OR when a barrier blocks the direction and no underlying child is set. Camina assumes there is always a child — fine against another Camina (both walk straight forever, never block), broken as soon as a barrier-placer (`MiniMax*`) blocks Camina's lane.

This is a **pre-existing agent-logic bug**, not infrastructure. The original D1 (Boost.Python era) acceptance test was Camina-vs-Camina (which works), not the Camina-vs-MiniMax2 pair pinned aspirationally in [`ONBOARDING.md`](ONBOARDING.md) §4.

### Repro

```sh
. /nix/var/nix/profiles/default/etc/profile.d/nix-daemon.sh && \
  nix develop -c bash -c './build/src/Consola/consola \
    bin/agenteCamina.py bin/agenteMiniMax2.py </dev/null' 2>&1 | tail -5
# → SinHijo: <EMPTY MESSAGE> ... agenteCamina.py(36): siguienteJugada, exit 1
```

### Next steps

Two reasonable patches, pick one:

1. **Catch `SinHijo` in the agent** (minimal):
   ```python
   try:
       hijo = celdaActual.getHijo(self.direccion)
       j.setPosicion(hijo.getPosicion() if hijo.estaLibre()
                     else hijo.getHijo(self.direccion).getPosicion())
   except Exception:  # Reglas.SinHijo or similar
       import random
       j = random.choice(ayudante.getMovimientosPosibles(self.id))
   ```
2. **Probe `estaLibreDireccion` first** (cleaner):
   ```python
   if not celdaActual.estaLibreDireccion(self.direccion):
       # forward blocked, pick any legal move
       import random
       j = random.choice(ayudante.getMovimientosPosibles(self.id))
   else:
       hijo = celdaActual.getHijo(self.direccion)
       ...
   ```

Also worth updating [`ONBOARDING.md`](ONBOARDING.md) §4's example pair to one that completes (e.g. `astar vs astar` or `Camina vs astar`) until Camina is fixed.

---

## 🟡 KB-003 — Cross-DSO RTTI mismatch silently bypasses `catch(pybind11::error_already_set&)`

**Surfaced by:** [PR #6](https://github.com/fabzter/quassimodo/pull/6), commit [`9640e8d`](https://github.com/fabzter/quassimodo/commit/9640e8d) (Fix #2 — top-level catch).
**Affected:** `src/Scripting/AgentePythonWrapper.cpp`, `src/Scripting/UtilsPython.cpp`, `src/Consola/main.cpp` — any module that catches `pybind11::error_already_set` thrown from a different DSO.

### What breaks (silently)

`AgentePythonWrapper`'s [`siguienteJugada()`](src/Scripting/AgentePythonWrapper.cpp) wraps its delegating call in `try { ... } catch(pybind11::error_already_set& e) { manejar_excepcion_python_libre(...); }`. The intent is to translate Python exceptions raised inside Python agent code into the project's own `Scripting::ScriptMalo` C++ exception so callers can handle them uniformly. **That translation never fires.** The `pybind11::error_already_set` propagates uncaught past this `catch` and lands in `consola/main.cpp`'s top-level `catch(std::exception&)` fallback instead. Functional result (exit 1 with diagnostic, no SIGABRT) is fine; the *intended* exception class hierarchy is broken.

### Root cause

pybind11 is header-only; `pybind11::error_already_set` is in an inline-namespace versioned in pybind11's headers. Each DSO that includes pybind11 (`Reglas.so`, `libScripting.dylib`, `consola`) gets its **own** typeinfo for the type. On macOS two-level namespace linking, the C++ runtime uses mangled-symbol-string comparison of `typeinfo` records across DSOs to match catch blocks — and these don't match cross-DSO. A `catch(pybind11::error_already_set&)` in `libScripting.dylib` doesn't match a `pybind11::error_already_set` thrown from `Reglas.so`. The broader `catch(std::exception&)` catches it because `std::exception`'s typeinfo *is* unique-symbol exported.

Confirmation in practice: stderr after the broken-agent fixture shows `Unhandled C++ exception (N8pybind1117error_already_setE)` — the mangled name of `pybind11::error_already_set` — caught by `consola/main.cpp`'s `catch(std::exception&)` block at the *top* of the catch chain, NOT by the matching `catch(pybind11::error_already_set&)` block one step above it.

### Repro

```sh
./build/src/Consola/consola bin/agenteCamina.py bin/agente_test_broken.py </dev/null 2>&1 | tail -2
# → stderr: "Unhandled C++ exception (N8pybind1117error_already_setE): RuntimeError: ..."
#   (caught by the std::exception fallback, NOT by the specific error_already_set catch above it)
```

### Proper fix (out of scope for PR #6)

Three options, in increasing order of intrusiveness:

1. **`-fvisibility=default`** as a global compile flag. Forces all symbols including typeinfo to have default visibility, which makes `typeinfo` for pybind11 types resolve to the same record across DSOs. Trade-off: slightly larger binaries, possible symbol-collision risk if multiple pybind11 versions ever coexist (we ship one).
2. **Per-type `__attribute__((visibility("default")))`** on the specific exception types used cross-DSO. Surgical but requires patching pybind11 or using a wrapper.
3. **Translate at the DSO boundary** — catch `pybind11::error_already_set` immediately at every binding entry/exit point inside the DSO that throws it, and rethrow as a stable cross-DSO type (`Scripting::ScriptMalo`). This is what `AgentePythonWrapper`'s catch was *trying* to do; it just needs to live inside `Reglas.so` (where the throws originate) rather than inside `libScripting.dylib`.

The `consola/main.cpp` top-level `catch(std::exception&)` fallback in PR #6 is the safety net — clean exit, readable diagnostic — and is sufficient for the project's current needs. Promote this entry to active fix when the intent ("agent errors get translated into `ScriptMalo`") becomes important (e.g. for a UI runner that wants typed error handling instead of a generic message).

---

## 🟡 KB-D2-001 — Torch flame/light dropped (IrrlichtMt stripped particle + dynamic-light APIs)

**Surfaced by:** D2.1 graphical port (`feat/d2.1-graphical-match`)
**Affected:** `src/Grafico/Antorcha.cpp` / `Antorcha.h` — flame particles and point light on torch props.

### What breaks

IrrlichtMt 1.9.0mt15 removed `IParticleSystemSceneNode` and `ILightSceneNode`/`addLightSceneNode`. `Antorcha` now renders only its static torch mesh (no flame particles, no point light). Torch props appear as inert props; no warmth or atmospheric lighting.

### Root cause

The Minetest fork stripped the dynamic-light and particle-system scene-node APIs because Minetest implements its own lighting pipeline.

### Next steps

D2.2: consider billboard/animated-texture flame sprite (no `IParticleSystemSceneNode` required). By design for D2.1; cosmetic.

---

## 🟡 KB-D2-002 — Piece shadow volumes dropped (IrrlichtMt stripped `IShadowVolumeSceneNode`)

**Surfaced by:** D2.1 graphical port (`feat/d2.1-graphical-match`)
**Affected:** `src/Grafico/Pieza.cpp` — `Pieza::setSombra`.

### What breaks

IrrlichtMt 1.9.0mt15 removed `IShadowVolumeSceneNode`/`addShadowVolumeSceneNode`. `Pieza::setSombra` is a no-op stub. Pieces cast no shadow.

### Root cause

Shadow-volume API stripped in the Minetest fork.

### Next steps

Cosmetic. D2.2: investigate screen-space or baked shadow alternatives. No gameplay impact.

---

## 🟡 KB-D2-003 — Menu/GUI deferred to D2.2 (`addWindow`/`addMessageBox` removed)

**Surfaced by:** D2.1 graphical port (`feat/d2.1-graphical-match`)
**Affected:** `src/Grafico/GUI.cpp`, `src/Grafico/Menu.cpp`, `src/Grafico/ManejadorGUI.cpp`, `src/Aplicacion/Aplicacion.cpp`, `src/Aplicacion/ManejadorJuego.cpp`, `src/Aplicacion/EventReceiver.cpp`.

### What breaks

IrrlichtMt 1.9.0mt15 removed `IGUIEnvironment::addWindow()`, `addMessageBox()`, `IGUIWindow`, and `EGET_MESSAGEBOX_OK`. The 2010 `Menu`/`GUI`/`ManejadorGUI` (and `Aplicacion`/`ManejadorJuego`/`EventReceiver`) are excluded from the D2.1 build. `aplicacion` is argv-driven (`aplicacion agentA.py agentB.py`) with no in-window agent selector or result dialog.

### Root cause

Minetest stripped the window/dialog GUI widgets it didn't use.

### Next steps

D2.2: replace `addWindow`/`addMessageBox` call sites with composed primitives (panel + child buttons/text) and re-include `Aplicacion`/`ManejadorGUI`/`EventReceiver` in the build.

---

## 🟡 KB-D2-004 — Piece/barrier/camera animation deferred to D2.2 (`ISceneNodeAnimator` subsystem removed)

**Surfaced by:** D2.1 graphical port (`feat/d2.1-graphical-match`)
**Affected:** `src/Grafico/Jugador.cpp` (`Mover`), `src/Grafico/Barrera.cpp` (`ColocaBarrera`), `src/Grafico/TokayoCamera.*`, `src/Grafico/JumpAnimator.*`, `src/Grafico/IAnimatorCameraTokayo.*`.

### What breaks

IrrlichtMt 1.9.0mt15 removed the entire `ISceneNodeAnimator` subsystem — no `ISceneNodeAnimator.h`, no `addAnimator`/`getAnimators`/`removeAnimator` on `ISceneNode`, no `createFlyStraightAnimator`, no `ISceneNodeAnimatorCameraMaya`. `Jugador::Mover`/`Barrera::ColocaBarrera` use instant `setPosition`; camera is fixed at a static vantage.

### Root cause

Minetest removed the animator subsystem; its own scene graph manages animation differently.

### Next steps

D2.2: convert `JumpAnimator`/`TokayoCamera`/`IAnimatorCameraTokayo` to plain per-frame update helpers (no `ISceneNodeAnimator` base). Re-enable tweened moves + orbit camera.

### Update (D2.2a·M1, 2026-06-05 — PR #9): partially resolved

Tweened **pawn moves are restored**: a fresh `Grafico::Animador` (dt-driven tween manager) + `SaltoAnim` (parabolic jump) replaces the deleted `ISceneNodeAnimator` subsystem, driven by an eager `EventoJugada` queue replayed per-frame in `PartidaGrafica::update(dt)`. The camera is now a framed static 3/4 pose behind a `CamaraController` seam (the old `JumpAnimator/TokayoCamera/IAnimatorCameraTokayo` files stay out — their math was reimplemented fresh). Vision-confirmed: a pawn is caught mid-jump and the match paces over time to a winner.
**Still open:** (1) **barrier slide** (`DeslizarAnim`) is implemented but UNEXERCISED — the clean Camina-vs-Camina gate pair are walkers that never place barriers, so the barrier slide is visually unconfirmed; (2) **interactive orbit/pan/zoom camera** is deferred to **D2.2b** (the `CamaraController` seam is in place; D2.2a ships only the static pose).

---

## 🟡 KB-D2-005 — Cosmetic: board base mis-centred, camera framing loose, debug stdout spam

**Surfaced by:** D2.1 graphical port (`feat/d2.1-graphical-match`)
**Affected:** `src/Grafico/Tablero.cpp` (board base placement), `src/Aplicacion/aplicacion_main.cpp` (camera), `src/Grafico/Antorcha.cpp` / `Pieza.cpp` / `Jugador.cpp` (legacy `std::cout` prints).

### What breaks

Three cosmetic issues, no gameplay impact:
1. The board base slab is not centred under the cell grid (offset visible at an angle).
2. The fixed camera framing is loose — significant empty viewport on all sides.
3. Legacy `std::cout` debug lines in `Grafico` (e.g. `setVectPosicion` printing "pos F …" / coordinate numbers) spam stdout during a graphical run, mixing with match output.

### Root cause

Board centring: `Tablero.obj` origin is a far corner; the base node and cell grid offset differently. Camera: placeholder framing from the D2.1 spike. Debug prints: 2010-era `std::cout` left in source, harmless in 2010 but noisy now.

### Next steps

D2.2: centre board base (match cell-grid origin), tune camera FOV/distance, strip `std::cout` debug lines from `Grafico`.

---

## 🟡 KB-D2-006 — Pre-existing `Reglas/Grafo.cpp:105` non-void fall-off (out of D2.1 scope)

**Buglog:** see `.wolf/buglog.json` (`-Wreturn-type` class)
**Surfaced by:** D2.1 build (`feat/d2.1-graphical-match`), compiler `-Wreturn-type` warning
**Affected:** `src/Reglas/Grafo.cpp` line 105 — a control path with no `return` in a non-`void` function.

### What breaks

`-Wreturn-type` flags the function; on modern clang this is latent UB (would emit `brk #0x1` / SIGTRAP if the path is reached). Not exercised to a crash by the working console game or D2.1 graphical match.

### Root cause

Pre-existing 2010 C++ — same class of bug as the `Partida::siguienteJugada` SIGTRAP fixed in D1 and the `PartidaGrafica::SetJugadores`/`Barrera::giraEste` fixes in D2.1. Left untouched in D2.1 to avoid out-of-scope game-logic changes.

### Next steps

Future Reglas pass: add the missing `return` (value TBD from control-flow analysis). Not urgent until a minimax/graph-traversal path that exercises line 105 is confirmed reachable.

---

## How to add an entry

Copy this template at the top of the "Status legend" section as a new entry, fill in, and PR. Cross-link any `bug-NNN` entry in `.wolf/buglog.json` so the two records stay in sync.

```markdown
## 🔴 KB-NNN — <one-line title>

**Buglog:** [`bug-NNN`](.wolf/buglog.json) (if applicable)
**Surfaced by:** PR # / commit / "manual review on 2026-MM-DD"
**Affected:** components / files / agent pairs / build configs that exhibit the bug

### What breaks
<concrete observation — exit code, error message, what the user sees>

### Root cause
<your best understanding; "unknown — needs investigation" is fine if true>

### Repro
<exact commands an agent or human can run to reproduce>

### Next steps
<concrete actions for the eventual fixer; don't leave this empty>
```

## Fixed bugs

*(none yet — when KB-001/KB-002/KB-003 are resolved, move them here with the
fixing commit + PR.)*
