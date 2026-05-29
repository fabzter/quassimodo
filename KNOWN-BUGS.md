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
