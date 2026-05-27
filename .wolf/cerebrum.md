# Cerebrum

> OpenWolf's learning memory. Updated automatically as the AI learns from interactions.
> Do not edit manually unless correcting an error.
> Last updated: 2026-05-26

## User Preferences

- [2026-05-26] NEVER include references to LLM/AI authorship or generation anywhere — no "Co-Authored-By: Claude", no "Generated with Claude Code", no AI mentions in git commit messages, pushes, PR titles/bodies, code comments, or any other artifact. Keep all output attribution-free.

## Key Learnings

- **Project:** quassimodo — Quoridor game (C++ / Irrlicht / Boost::Python) where two Python AI agents compete. Dormant since 2010, being resurrected.
- **VCS history:** Two Mercurial clones, NOT named branches. `quassimodo/` (main, hg tip `ee96566b`, rev 232) and `quassimodo.concurso/` (concurso, hg tip `920aa5f`, rev 332). Both on hg branch `default`. Shared root `08318f39` (2010-03-08), 232 shared changesets. They fork at last-common changeset `110401bfd` (main rev 231 = concurso rev 250). Topology is INTERLEAVED — concurso pulled from main repeatedly, so main's commits are woven through concurso's DAG.
- **Build artifacts in history:** `.o`, `.so`, `.pyc` were committed early (first commit msg: "incluidos los binarios :S") then removed; they persist in history. Game assets (textures .jpg/.png/.bmp, models .3ds, fonts, doc .odt) are legitimate project files.
- **Build stack (from NetBeans Makefile-Debug.mk):** GNU toolchain gcc/g++; Linux + X11 + OpenGL. Deps: **Irrlicht** (`-lIrrlicht`, `/usr/include/irrlicht`, links `libGL.so` + `libXxf86vm.so`), **Python 2.6** (`-lpython2.6`, `/usr/include/python2.6`), **Boost 1.40** with `-mt` tag: `boost_thread-mt`, `boost_filesystem-mt`, `boost_system-mt`, `boost_program_options-mt`, `boost_python-mt-py26`. Bundled libs were `libboost_*.so.1.40.0`.
- **CRITICAL — Python embedding / linkage topology (study before touching the build):**
  - The app **embeds** CPython (`Py_Initialize` in `Scripting/InterpretePython.cpp`), then `import Reglas`.
  - `Reglas.so` (built by `src/AgenteWrapper`, hand-written Makefile) is a **Boost.Python extension** wrapping C++ classes (Tablero, Celda, Agente, AyudanteDeAgente, container_conversions, …). Agents `import Reglas` and subclass `Reglas.Agente`.
  - **Type-domain C++ classes live in `libReglas.so`** (single shared lib). Both the app (`Aplicacion`, `Grafico` link `-lReglas`) AND `Reglas.so` (links `-lReglas`) use it → RTTI/`type_info` single-sourced (required for cross-boundary casts).
  - **Cross-boundary object passing:** `ModuloPython.cpp` (in `libScripting`) does `object(boost::python::ptr(&tablero))` and wraps a `new AyudanteDeAgente` to inject `tablero`/`ayudante` into the agent namespace. Bidirectional: C++→Py (tablero/ayudante) and Py→C++ (agent instance via `boost::python::wrapper`/`AgentePythonWrapper`).
  - **HAZARD 1 — dual Boost.Python registry:** `Reglas.so` links boost_python **statically** (`-Bstatic -lboost_python -Bdynamic`) while `libScripting` links it **dynamically** (`-lboost_python-mt-py26`). Two registries → converters registered in the extension may be invisible to the embedder. The original got away with it on Linux via global symbol visibility / luck. **Modern fix: link boost_python as ONE shared lib everywhere; never static into the extension.**
  - **HAZARD 2 — implicit runtime symbol resolution:** `libScripting` uses `Reglas::` symbols but does NOT link `-lReglas`; it relies on Linux resolving undefined symbols at load time against the app's `libReglas.so`. macOS two-level namespace forbids this → must link deps explicitly (CMake `target_link_libraries`) or use flat namespace.
  - **No `sys.setdlopenflags(RTLD_GLOBAL)` anywhere** — Linux defaulted to making it work. macOS: build the extension as a MODULE with `-undefined dynamic_lookup`; resolve libpython/boost_python from the host process; share a single boost_python dylib. Plan must handle registry/RTTI sharing per-platform.
- **Build layout:** NetBeans-generated makefiles per module. 4 modules build `.so` (Grafico, Reglas, Scripting, Opciones), then Aplicacion links them via rpath into `bin/aplicacion`. Top-level Makefile orchestrates. No `-m32` flag (NetBeans "GNU-Linux-x86" is just a label; 64-bit build is fine). **Ubuntu 10.04 LTS shipped exactly Boost 1.40 + Python 2.6 + Irrlicht 1.6 — near-perfect period match.** Runtime loads assets from `conf/` (opciones.conf references .3ds models + textures); Python agents live in `bin/*.py`.

## Do-Not-Repeat

<!-- Mistakes made and corrected. Each entry prevents the same mistake recurring. -->
<!-- Format: [YYYY-MM-DD] Description of what went wrong and what to do instead. -->

- [2026-05-26] hg→git: do NOT use hg-fast-export.sh on this machine — the active python3 (pyenv 3.13) cannot `import mercurial`. Use git-cinnabar instead (self-contained, no mercurial python module needed).
- [2026-05-26] Don't convert the two clones to git separately and graft — topology is interleaved. Unify into one hg repo first (`hg pull`), then convert once so the fork point is reconstructed correctly from the DAG.
- [2026-05-26] Do NOT add "Co-Authored-By: Claude" or any AI/LLM authorship trailer to commits (corrected by user mid-commit). See User Preferences.

## Decision Log

<!-- Significant technical decisions with rationale. Why X was chosen over Y. -->

- [2026-05-26] hg→git migration strategy (user-approved decisions):
  - **Binaries:** Strip regeneratable build artifacts (.o/.so/.pyc, NetBeans private/) from history via `git filter-repo`; KEEP game assets (textures/models/fonts/.odt). Add .gitignore going forward.
  - **Layout:** Flatten project to git repo root (hg paths are already root-relative, so natural).
  - **Location:** Convert in place at `/Users/fabzter/src/quassimodo`; keep `.hg` clones as a safety net until conversion is verified, then remove.
  - **Branches:** main clone tip → `main`; concurso clone tip → `concurso`; they diverge at `110401bfd`.
  - **Tooling:** git-cinnabar (hg→git) + git-filter-repo (artifact strip). Unify clones with `hg pull` first.
- [2026-05-26] Build-revival scope (user-confirmed): definition of done = the REAL game source **compiles, links, and runs a playable match** natively on macOS arm64 in the isolated env — NOT just de-risking spikes. The source port (Python 2.6→3, Boost.Python/Irrlicht API) is in scope. Isolation = Devbox(Nix) + CMake. Engine = **Minetest Irrlicht fork** (confirmed; stock nixpkgs irrlicht is broken on darwin). Spec: docs/superpowers/specs/2026-05-26-build-isolation-design.md.
- [2026-05-26] Merged `main` into `concurso` to sync. Only conflict was `src/Grafico/Antorcha.cpp::setVectPosicion` — main's sole unique source commit ("mmm cambios minimos") was a throwaway debug snapshot (hardcoded X/Z=0 + two `std::cout` debug lines); concurso had a more evolved version. Resolved by keeping concurso's version and dropping main's debug code. main is now an ancestor of concurso.
- [2026-05-26] Untracked volatile OpenWolf runtime state from git (memory.md, anatomy.md, _session.json, token-ledger.json, cron-state.json, suggestions.json, designqc-report.json) — they churn every action and blocked branch switches. Kept stable files tracked: cerebrum.md, config.json, hooks/*.js, OPENWOLF.md, buglog.json, identity.md, reframe-frameworks.md, cron-manifest.json.
