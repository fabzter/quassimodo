# D2 — Graphical Port Design Spec (v2)

- **Date:** 2026-05-29
- **Status:** Approved (design); pending implementation plan
- **Project:** Quassimodo — Phase D2
- **Supersedes:** [`2026-05-28-d2-graphical-port-design.md`](2026-05-28-d2-graphical-port-design.md) and its plan
  [`2026-05-28-phase-d2-graphical-port.md`](../plans/2026-05-28-phase-d2-graphical-port.md). Those remain in git
  history; this is the canonical D2 design going forward.
- **Companion docs:** [`ROADMAP.md`](../ROADMAP.md), [`ONBOARDING.md`](../../../ONBOARDING.md),
  [`KNOWN-BUGS.md`](../../../KNOWN-BUGS.md), [`.wolf/cerebrum.md`](../../../.wolf/cerebrum.md)

## 0. Why this supersedes the prior D2 spec

The merged D2 spec/plan (`baa7ba8` / `403a44b`) prescribed a "compile the existing code, fix Irrlicht API
errors as they surface" strategy and asserted the IrrlichtMt API surface was "likely compatible, low risk." A
prior attempt on `feat/d2-graphical` (a less-capable model) followed that path and produced incomplete,
mock-laden work. Direct inspection of the installed IrrlichtMt 1.9.0mt15 artifact contradicts the merged spec's
core assumptions:

| Merged spec claim | Verified reality (probed 2026-05-29) |
|---|---|
| 3DS mesh loading "verify format support… low risk" | **No 3DS loader is compiled into the dylib.** `strings libIrrlichtMt.*` → 0 hits for `3ds`. Only `CXMeshFileLoader` (.x), `CB3DMeshFileLoader` (.b3d), `COBJMeshFileLoader` (.obj) exist. Every Quassimodo mesh is `.3ds` → `getMesh()` returns null at runtime, guaranteed. |
| Particle/light/shadow "present in IrrlichtMt headers" | `IParticleSystemSceneNode.h`, `IShadowVolumeSceneNode.h` are **absent**; `addParticleSystemSceneNode` / `addShadowVolumeSceneNode` are **absent** from the fork's `ISceneManager` / `IAnimatedMeshSceneNode`. `Antorcha.cpp` (torch flames) and `Jugador.cpp` (piece shadows) cannot compile as-is. |
| `irrList.h` "may be deprecated" | **Absent.** `ManejadorJuego.hpp` includes `<irrlicht/irrList.h>` → hard error. |
| "IrrlichtMt uses `createDeviceEx()` NOT `createDevice()`" | **False.** `createDevice()` is still exported (`irrlicht.h:270`) and the engine spike already runs a window through it. `Video.cpp`'s `createDevice()` calls are fine; "fixing" them is wasted motion. |

The lesson from D1 → D1.5 → D1′ holds: **de-risk the unknown in a focused spike first, then port in subsystem
chunks with a gate per chunk** — never "compile and triage" across a large API-drift surface. This spec applies
that pattern.

## 1. Goal & Non-Goals

### Goal
Bring up a **graphical, playable AI-vs-AI Quassimodo match** in a native IrrlichtMt window on macOS arm64,
inside the existing isolated Nix env, by porting `src/Grafico/` and `src/Aplicacion/` (the dormant 2010 graphical
code) against IrrlichtMt 1.9.0mt15. This completes spec-D (graphical) + spec-E (GUI) from the master design.

### Non-Goals
- New gameplay/features beyond the 2010 design.
- Replacing the engine, or supporting stock Irrlicht.
- Reviving the stripped subsystems with new tech in D2 itself (torch particles, piece shadows) — they are
  **explicitly stubbed** (see §6), with replacement deferred to a future D2.x.
- Fixing pre-existing game-logic bugs (e.g. `KB-001` Minimax SIGSEGV). D2 is an API/rendering port; it must not
  change game logic.
- x86_64 / Linux validation (must remain reachable; not validated here).

## 2. Staging — three sub-phases, two PRs

| Sub-phase | What | PR |
|---|---|---|
| **D2.0 — Render-from-real-state spike** | Under `spikes/d2_render/`. Build a real `Reglas::Tablero`, derive a minimal IrrlichtMt scene (board + cells + 2 pieces + 1 barrier) from it, render a static frame, apply one real move, re-derive and render the new frame. Proves the asset pipeline (converted meshes load + look right), the real-game-state→scene mapping, and the per-move scene-update path. | **PR (D2.0)** |
| **D2.1 — Port Grafico + Aplicacion** | Port the real `src/Grafico/` and `src/Aplicacion/` in subsystem chunks, each with its own gate, full menu included. The final chunk's gate **is** the graphical playable match (what the merged spec called "D2.2"). | **PR (D2.1)** |

D2.1 must not begin chunk work that depends on meshes until D2.0 has settled the asset pipeline. D2.0 should be
merged (or at minimum green on its own branch) before D2.1's rendering chunks start.

## 3. Asset pipeline (the central D2 problem)

All seven game models are `.3ds` (`conf/skin_default/{Tablero,Mono,MonoBlanco,MonoSombra,Lumbrera,Barrera,Celda}.3ds`).
IrrlichtMt cannot load `.3ds`. **Decision: convert the models to `.obj`** (a fork-supported, battle-tested loader
format) as a one-time, reproducible, committed conversion.

- **Tool:** `assimp` (CLI), pinned in `flake.nix`. Conversion command shape:
  `assimp export In.3ds Out.obj` (produces `Out.obj` + `Out.mtl`). Reproducible from the committed `.3ds` sources.
- **Why `.obj` not `.x`/`.b3d`:** Quassimodo's pieces are **transform-animated** (the `JumpAnimator` moves whole
  scene nodes), not skeletally animated, so OBJ's lack of skeletons costs nothing. OBJ is text — trivially
  inspectable when debugging. `.x` is the documented fallback if the spike shows OBJ UV/material fidelity is poor.
- **Textures are applied explicitly** by the `Skin` loader from `opciones.conf` (`[skin.texturas]`), not from the
  mesh's embedded materials. So mesh-format material fidelity matters less than geometry + UV correctness — which
  the D2.0 spike verifies visually.
- **Wiring:** the converted `.obj` files live alongside the `.3ds` in `conf/skin_default/`. `opciones.conf`'s
  `[skin.modelos]` entries change from `*.3ds` to `*.obj` (config edit, **no code-logic change**). The `.3ds`
  originals stay in the repo as conversion provenance.
- **Fidelity is a D2.0 gate, not an assumption.** If a converted mesh renders with wrong geometry/UVs, that is
  caught in the spike's visual check and we iterate (assimp flags, or switch that mesh to `.x`).

## 4. D2.0 — Render-from-real-state spike

### 4.1 Topology
```
spikes/d2_render/
 ├─ scene_builder.{hpp,cpp}  ← minimal Reglas::Tablero → irr scene tree (NO Grafico::, NO GUI, NO menu)
 ├─ main.cpp                 ← drives the spike + gate
 └─ CMakeLists.txt
     d2_render binary links: Reglas (existing SHARED lib) + IrrlichtMt::IrrlichtMt
                             + Cocoa/OpenGL/IOKit (APPLE)
```
The spike reuses the existing shared `Reglas` lib directly (single RTTI source, same as everywhere). It does
**not** use `Grafico::` types, the `Skin`/`CConfigReader` machinery, Python embedding, or the menu — those are
D2.1. `scene_builder` is a deliberately minimal stand-in that proves the *mechanism*, exactly as the D1.5
`pybindspike` stand-ins proved binding mechanisms without the real `Reglas` graph.

### 4.2 Flow (single round-trip)
1. Construct a `Reglas::Tablero` and place two players at starting cells (real Reglas API:
   `Tablero` ctor + `setJugadores` / move helpers; exact construction nailed in the plan).
2. `createDevice(EDT_OPENGL, dimension2d(800,600), 16, false, false, false, nullptr)` — the call the engine spike
   proved on macOS arm64.
3. Load the converted meshes by path (`conf/skin_default/Tablero.obj`, `Celda.obj`, `Mono.obj`, `MonoBlanco.obj`,
   `Barrera.obj`) via `smgr->getMesh()`. **If any returns null, fail fast** with a clear per-asset error — that
   means the conversion (or this format) is wrong, and it must be resolved before anything else.
4. `scene_builder` maps the `Reglas::Tablero` to a scene tree: the board mesh, a `Celda` node per board cell at
   its mapped world position (the board is a 9×9 Quoridor grid), each player's piece at its start cell, and one
   `Barrera` node placed between two adjacent cells.
5. Render ~60 frames of the initial position; capture `build/spikes/d2_render/spike-before.png` via
   `driver->writeImageToFile(driver->createScreenShotFromBackBuffer(), …)`.
6. Apply one **real** move: `tablero.moverJugador(0, x, y)` to an adjacent cell. Re-derive the moved piece's node
   position from the new `Tablero` state (this is the per-move update path D2.1's match loop will reuse).
7. Render ~60 more frames; capture `spike-after.png`.
8. Print the gate and exit.

### 4.3 Gate
Print **`D2 SPIKE OK`** and exit 0 iff all hold:
- Every `getMesh()` returned non-null (assets load).
- Both PNGs exist and are > 5 KB (non-trivial render — sanity that pixels were drawn).
- The two PNGs are **not byte-identical** (the move was actually reflected on screen).

**Plus a human/vision check** (not a string gate, but a required reviewer step): the spike's PNGs are read with the
Read tool and visually confirmed to show a recognizable board with pieces and a barrier, and the piece visibly
moved between them. This is the real proof; the byte checks are just cheap pre-filters. **Screenshots are NOT
committed to git** — they are transient build artifacts, inspected then discarded.

## 5. D2.1 — Port Grafico + Aplicacion in subsystem chunks

Each chunk is one (or a few) plan task(s) with a **specific, bounded gate**. No chunk says "fix errors as they
appear" — an unexpected error is a BLOCKED escalation, not license to improvise a mock. Chunk boundaries are the
levers; **re-planning checkpoints between chunks** let later chunks shrink/grow based on what earlier chunks (and
D2.0) discovered, rather than marching through fixed boxes.

| # | Chunk | Bounded gate |
|---|---|---|
| 1 | **Mechanical include fixes** — `<irrlicht/XXX.h>` → `<XXX.h>` across `src/Grafico/` + `src/Aplicacion/`; `<irrlicht/irrList.h>` → `std::list` migration in `ManejadorJuego.hpp` and any other `core::list` users. | `cmake` configure succeeds; the include graph resolves (TU-level header errors gone). |
| 2 | **Build wiring** — `src/Grafico/CMakeLists.txt`, `src/Aplicacion/CMakeLists.txt`; flip `QM_CONSOLE_ONLY` OFF and gate Grafico/Aplicacion behind it. | `consola` still builds and still plays a console match (D1 regression gate). Grafico TUs attempt compilation. |
| 3 | **Stripped-subsystem stubs** — `Antorcha` particle calls no-op'd; `Jugador` shadow-volume calls skipped; any remaining `core::list`. (See §6.) | `libGrafico.dylib` links cleanly. |
| 4 | **Device + Video** — confirm `Video.cpp`'s `createDevice()` path on the fork; `Aplicacion` opens its window via its own main loop (not the spike's). | `aplicacion` opens a blank IrrlichtMt window from `Aplicacion::run()` and exits cleanly on window close. |
| 5 | **Board + pieces + barriers render** — port `Grafico::{Tablero,Celda,Pieza,Jugador,Barrera}` + `Skin`/`SkinLoader` asset loading (now `.obj`). | `aplicacion` renders the starting position (board, cells, two pieces). Verified by **reading a screenshot** (vision). Agents wired via argv, no menu yet. |
| 6 | **Camera + animation** — `tokayoCamera`/`IAnimatorCameraTokayo`, `JumpAnimator`. | A scripted single move **animates** on screen (verified by reading before/after screenshots). |
| 7 | **GUI skin + custom elements** — `CImageGUISkin` (must implement exactly the fork's `IGUISkin` 12 pure-virtuals), `CGUIProgressBar`, `CConfigReader`/`CConfigMap`, `guiskin.cfg` loading. | A single skinned widget (e.g. a button) renders with the image skin. Verified by reading a screenshot. |
| 8 | **Menu** — `Menu`, `ManejadorGUI`, `GUI`, the `EventReceiver` GUI events. Full 2010 menu per decision. | Menu opens; agent selection works; "Start" launches a `Partida`. Verified by reading screenshots + observing the transition. |
| 9 | **PartidaGrafica integration + full match** — wire `ManejadorJuego` → `PartidaGrafica` → Python-embedded agents (the D1′ pybind11 path, unchanged) → animated match. | **AI-vs-AI match plays to completion in the graphical window**, ending at the "Hay un ganador!" GUI dialog. Verified by reading screenshots at several match stages + a clean exit code. `otool -L aplicacion` shows IrrlichtMt + libGrafico/Reglas/Scripting, and **no** `boost_python`. |
| 10 | **Docs** — `ROADMAP.md` (D2 → Done), `KNOWN-BUGS.md` (KB-D2-001/002), `.wolf/cerebrum.md` (API-drift findings, asset pipeline, GUI skin notes). | Docs current; PR opened. |

## 6. Stripped-subsystem dispositions (explicit — not mocks)

These IrrlichtMt removals are handled by **honest stubs that change rendering, not game logic**, each documented
in `KNOWN-BUGS.md`:

- **`KB-D2-001` — torch particles.** `IParticleSystemSceneNode` and `addParticleSystemSceneNode` are gone.
  `Antorcha::dibujaAntorcha`/the particle block (`Antorcha.cpp` ~lines 67–83: `createBoxEmitter`,
  `createFadeOutParticleAffector`, `addAffector`) are no-op'd. The `Antorcha` object still exists and its light
  (`ILightSceneNode`, still present) may still be created; only the flame particle system is dropped. Visual
  consequence: no animated flame. Future D2.x could use a billboard.
- **`KB-D2-002` — piece shadows.** `IShadowVolumeSceneNode` and `addShadowVolumeSceneNode` are gone. The
  `addShadowVolumeSceneNode()` call in `Jugador.cpp` is skipped. Visual consequence: pieces cast no stencil
  shadow. Cosmetic.
- **`core::list` → `std::list`.** `irrList.h` is gone; `ManejadorJuego.hpp` (and any other user) switches to
  `std::list<T>`. This is a clean type swap, **not** a stub — `std::list` is a strict functional superset for the
  usage here.

These are the only sanctioned deviations from the 2010 behavior. Anything else that doesn't compile/run is a
BLOCKED escalation to be designed, not stubbed silently.

## 7. Verification discipline

- **Visual gates are verified by vision, not by git.** For every chunk whose gate is "renders X", the implementer
  (and the reviewer) capture a screenshot to the build dir and **Read it** to confirm the actual pixels are
  correct. Screenshots are transient — **never committed**. The proof is that an agent with vision looked at the
  frame, not that a blob sits in the repo.
- **Two-stage subagent review is mandatory for every D2.1 chunk** (spec-compliance reviewer, then code-quality
  reviewer) — D2's uncertainty is too high to use the controller-only review that trivial D1.5 tasks allowed. The
  spec reviewer independently rebuilds, re-runs the gate, reads the screenshot, and runs `otool -L` where
  relevant.
- **Exit codes captured directly**, never through `| tail`/`| head` (the D1 SIGTRAP-masking lesson).
- **`consola` regression is re-checked** at every chunk that touches the build (`QM_CONSOLE_ONLY` flip, shared
  libs) — the console match must keep playing to "Hay un ganador!".

## 8. Model selection & monitoring (dynamic)

- **Default to the most capable model (Opus)** for the D2.0 spike and every D2.1 chunk that involves API
  adaptation, rendering, animation, GUI, or integration judgment (chunks 4–9, and 7–8 especially).
- **Sonnet only for purely mechanical chunks** — the include rewrites (chunk 1), CMake wiring (chunk 2), the
  assimp conversion scripting, and the docs chunk (10).
- **Escalate freely mid-chunk:** if a "mechanical" task surfaces non-mechanical drift, re-dispatch on Opus rather
  than letting a weaker model thrash.
- **Subagents get full context inlined** — the relevant spec/plan sections and the actual file contents in the
  prompt, never "go read the file." Bake in the env gotchas (§4 of ONBOARDING), the no-AI-attribution rule, the
  repo-local git identity, and "this is a port — do not change game logic; BLOCK instead of mocking."

## 9. Build / env changes

- **`flake.nix`:** add `pkgs.assimp` to the devShell (for the one-time `.3ds`→`.obj` conversion). No engine
  changes — IrrlichtMt is already pinned. `git add flake.nix` before `nix develop` (flakes only see staged files).
- **`spikes/CMakeLists.txt`:** `add_subdirectory(d2_render)`; new `spikes/d2_render/CMakeLists.txt`.
- **`src/CMakeLists.txt`:** flip `QM_CONSOLE_ONLY` to default OFF; guard `add_subdirectory(Grafico)` /
  `add_subdirectory(Aplicacion)` behind `if(NOT QM_CONSOLE_ONLY)`. `consola` must still build either way.
- **New:** `src/Grafico/CMakeLists.txt`, `src/Aplicacion/CMakeLists.txt` (shared `libGrafico` + `aplicacion`
  executable; link Reglas/Opciones/Scripting/IrrlichtMt + frameworks; explicit `target_link_libraries`).
- **`conf/skin_default/*.obj` (+ `.mtl`):** committed converted assets; `opciones.conf` model paths updated.

## 10. Risks

- **R1 — Converted-mesh fidelity.** assimp `.3ds`→`.obj` may misorder/mis-UV geometry. *Mitigation:* D2.0 reads the
  rendered frame and confirms visually before D2.1; `.x` fallback per-mesh; assimp flags (`--gen-normals`, triangulation).
- **R2 — `IGUISkin` pure-virtual drift.** The fork's `IGUISkin` has 12 pure-virtuals; the 2010 `CImageGUISkin`
  implemented Irrlicht 1.6's set, which may differ. *Mitigation:* chunk 7 diffs the fork's `IGUISkin.h` against
  `CImageGUISkin`'s overrides and implements/aligns exactly the fork's set. Isolated to one chunk.
- **R3 — `EventReceiver` / input API drift.** `IEventReceiver` event struct fields may have changed.
  *Mitigation:* chunk 8; compare against fork `IEventReceiver.h`.
- **R4 — Texture loaders.** `.jpg`/`.png`/`.bmp` are standard; the fork ships image loaders. Low risk; surfaced in
  chunk 5's visual check.
- **R5 — Mesh scale/orientation after conversion.** assimp may change units/axis. *Mitigation:* the 2010 code
  already applies explicit scale/position; D2.0 surfaces gross errors visually, and scale constants are tunable in
  chunk 5.
- **R6 — Python embedding in graphical mode.** `Aplicacion` → `ManejadorJuego` → `Scripting::Manejador` embeds
  Python via the D1′ pybind11 path, already proven. D2 adds rendering around the same code path. Low risk;
  exercised in chunk 9.
- **R7 — `consola` regression** from the `QM_CONSOLE_ONLY` flip / shared-lib changes. *Mitigation:* re-run the D1
  console gate at every build-touching chunk (§7).

## 11. Success criteria

1. D2.0: `d2_render` prints `D2 SPIKE OK`, exit 0; the before/after PNGs visually show a recognizable board with
   pieces + barrier and the piece moving. Asset pipeline (`.obj`) confirmed.
2. D2.1 builds clean under `nix develop -c cmake --build build` with `QM_CONSOLE_ONLY=OFF` on macOS arm64;
   `consola` still plays a console match (no regression).
3. `aplicacion` opens a native IrrlichtMt window, shows the menu, agent selection launches a match.
4. A full AI-vs-AI match plays to completion in the graphical window, ending at the "Hay un ganador!" dialog —
   verified by reading screenshots at multiple stages and a clean exit.
5. `otool -L aplicacion` shows IrrlichtMt + libGrafico/Reglas/Scripting/Opciones and **no** `boost_python`.
6. Two PRs (D2.0, D2.1) opened against `main` with the repo-local `fabzter` identity (switch active gh account to
   `fabzter` before push, back to `fabriziohernandez` after).

## 12. Out of scope / future (D2.x)

- Torch particle flames (replace stub with billboards/shader) — `KB-D2-001`.
- Piece stencil shadows — `KB-D2-002`.
- Pre-existing game-logic bugs (`KB-001` Minimax SIGSEGV, etc.) — unrelated to the graphical port.
- x86_64 / Linux validation of the graphical build.
- `.app` bundling / asset-path packaging for distribution.
