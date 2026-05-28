# D2 — Graphical Port Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Compile `src/Grafico/` and `src/Aplicacion/` against IrrlichtMt, bring up the graphical `aplicacion` binary, and verify a full AI-vs-AI match plays in the 3D window on macOS arm64.

**Architecture:** Unchanged from 2010. `PartidaGrafica` extends `Partida` (game logic), `Aplicacion` owns the Irrlicht device + main loop, `ManejadorJuego` orchestrates everything. We compile the existing code with IrrlichtMt API fixes.

**Tech Stack:** IrrlichtMt 1.9.0mt15 (already in flake), CMake 3.30 + Ninja, clang 16, macOS frameworks (Cocoa, OpenGL, IOKit).

**Branch:** `feat/d2-graphical`, cut from `main`.

**Companion spec:** [`docs/superpowers/specs/2026-05-28-d2-graphical-port-design.md`](../specs/2026-05-28-d2-graphical-port-design.md)

**Engine spike reference:** `spikes/engine/main.cpp` (proves IrrlichtMt opens a window on macOS arm64).

---

## Critical gotchas (every subagent MUST read)

1. **`nix`/`devbox` are NOT on PATH in non-interactive shells.** Every build command MUST be prefixed:
   ```sh
   . /nix/var/nix/profiles/default/etc/profile.d/nix-daemon.sh && <command>
   ```
   In practice: `. /nix/var/nix/profiles/default/etc/profile.d/nix-daemon.sh && nix develop -c bash -c '...'`.
2. **Nix flakes only see git-tracked files.** `git add <files>` BEFORE `nix develop`.
3. **Never pipe through `| tail`/`| head`** — masks exit code. Capture `$?` directly.
4. **Never amend/rebase.** Commit per task.
5. **No AI/LLM attribution** — not in commits, PRs, or code.
6. **Git identity is repo-local** — plain `git commit` works.
7. **Don't change game logic.** API-translation only.
8. **`consola` must keep working.** Every build must verify the console match still plays.
9. **IrrlichtMt include path:** headers are at `include/irrlichtmt/` with flat structure. `#include <irrlicht.h>` works (umbrella header), but `#include <irrlicht/XXX.h>` does NOT — needs to be `#include <XXX.h>`.
10. **IrrlichtMt uses `createDeviceEx()`** not the old `createDevice()`. The engine spike (`spikes/engine/main.cpp`) shows the correct pattern.
11. **Frameworks on macOS:** Cocoa, OpenGL, IOKit must be linked explicitly on APPLE targets.

---

### Task 1: Branch + flip QM_CONSOLE_ONLY

**Files:**
- Modify: `src/CMakeLists.txt`

- [ ] **Step 1: Cut branch**

```bash
git checkout main && git pull origin main && git checkout -b feat/d2-graphical
```

- [ ] **Step 2: Flip QM_CONSOLE_ONLY and add Grafico/Aplicacion subdirectories**

```cmake
cmake_minimum_required(VERSION 3.25)
project(quassimodo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(QM_BUILD_SPIKES "Build the de-risking spikes" ON)

if(QM_BUILD_SPIKES)
  add_subdirectory(spikes)
endif()

option(QM_BUILD_GAME "Build the real game modules" ON)
if(QM_BUILD_GAME)
  add_subdirectory(src)
endif()
```

Wait — the `QM_CONSOLE_ONLY` option was in `src/CMakeLists.txt`. Let me re-read it.

- [ ] **Step 2 (corrected): Modify src/CMakeLists.txt**

Read current:
```cmake
# D1 builds the non-graphical game. Grafico + the graphical Partida path stay
# excluded while QM_CONSOLE_ONLY is ON (flipped OFF in Phase D2).
option(QM_CONSOLE_ONLY "Build console-only (exclude Grafico/PartidaGrafica)" ON)

add_subdirectory(Reglas)
add_subdirectory(Opciones)
add_subdirectory(Scripting)
add_subdirectory(AgenteWrapper)
add_subdirectory(Consola)
```

Replace with:
```cmake
option(QM_CONSOLE_ONLY "Build console-only (exclude Grafico/Aplicacion)" OFF)

add_subdirectory(Reglas)
add_subdirectory(Opciones)
add_subdirectory(Scripting)
add_subdirectory(AgenteWrapper)
add_subdirectory(Consola)

if(NOT QM_CONSOLE_ONLY)
  add_subdirectory(Grafico)
  add_subdirectory(Aplicacion)
endif()
```

- [ ] **Step 3: Commit**

```bash
git add src/CMakeLists.txt
git commit -m "build: flip QM_CONSOLE_ONLY=OFF, add Grafico/Aplicacion subdirectories"
```

---

### Task 2: Grafico CMakeLists + include fixes

**Files:**
- Create: `src/Grafico/CMakeLists.txt`
- Modify: 22 files in `src/Grafico/` — include path fixes

- [ ] **Step 1: Create Grafico CMakeLists.txt**

```cmake
file(GLOB GRAFICO_SRC CONFIGURE_DEPENDS "*.cpp")
add_library(Grafico SHARED ${GRAFICO_SRC})
target_include_directories(Grafico PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(Grafico PUBLIC Reglas Opciones IrrlichtMt::IrrlichtMt)
if(APPLE)
  target_link_libraries(Grafico PRIVATE "-framework Cocoa" "-framework OpenGL" "-framework IOKit")
endif()
```

- [ ] **Step 2: Fix Grafico includes — every `<irrlicht/XXX.h>` → `<XXX.h>`**

Files to fix (pattern: `s|<irrlicht/\([^>]*\)>|<\1>|g`):

| File | Old | New |
|---|---|---|
| `Antorcha.cpp` | `<irrlicht/ISceneManager.h>`, `<irrlicht/ILightSceneNode.h>`, `<irrlicht/IParticleSystemSceneNode.h>` | `<ISceneManager.h>`, `<ILightSceneNode.h>`, `<IParticleSystemSceneNode.h>` |
| `Barrera.hpp` | `<irrlicht/vector3d.h>` | `<vector3d.h>` |
| `GUI.hpp` | `<irrlicht/irrlicht.h>` | `<irrlicht.h>` |
| `IAnimatorCameraTokayo.hpp` | `<irrlicht/ISceneNodeAnimator.h>` | `<ISceneNodeAnimator.h>` |
| `Jugador.cpp` | `<irrlicht/IAnimatedMeshSceneNode.h>`, `<irrlicht/IShadowVolumeSceneNode.h>` | `<IAnimatedMeshSceneNode.h>`, `<IShadowVolumeSceneNode.h>` |
| `JumpAnimator.hpp` | `<irrlicht/irrlicht.h>`, `<irrlicht/ISceneNodeAnimator.h>` | `<irrlicht.h>`, `<ISceneNodeAnimator.h>` |
| `ManejadorGUI.hpp` | `<irrlicht/irrlicht.h>` | `<irrlicht.h>` |
| `Menu.hpp` | `<irrlicht/ICameraSceneNode.h>`, `<irrlicht/vector3d.h>` | `<ICameraSceneNode.h>`, `<vector3d.h>` |
| `Pieza.cpp` | `<irrlicht/ISceneNode.h>` | `<ISceneNode.h>` |
| `Skin.hpp` | `<irrlicht/irrlicht.h>` | `<irrlicht.h>` |
| `tokayoCamera.hpp` | `<irrlicht/ISceneNodeAnimator.h>`, `<irrlicht/ICameraSceneNode.h>`, `<irrlicht/SViewFrustum.h>` | `<ISceneNodeAnimator.h>`, `<ICameraSceneNode.h>`, `<SViewFrustum.h>` |
| `Video.hpp` | `<irrlicht/irrlicht.h>` | `<irrlicht.h>` |

Files already using flat includes (NO CHANGE): `CGUIProgressBar.cpp`, `CGUIProgressBar.h`, `CImageGUISkin.cpp`, `CImageGUISkin.h`, `SkinLoader.cpp`.

Files using just `<irrlicht.h>` (NO CHANGE): `Antorcha.hpp`, `Celda.hpp`, `Jugador.hpp`, `Pieza.hpp`, `Tablero.hpp`, `Video.cpp`.

- [ ] **Step 3: Commit**

```bash
git add src/Grafico/
git commit -m "build: Grafico CMakeLists + IrrlichtMt include path fixes (22 files)"
```

---

### Task 3: Aplicacion CMakeLists + include fixes

**Files:**
- Create: `src/Aplicacion/CMakeLists.txt`
- Modify: 5 files in `src/Aplicacion/` — include path fixes

- [ ] **Step 1: Create Aplicacion CMakeLists.txt**

```cmake
file(GLOB APLICACION_SRC CONFIGURE_DEPENDS "*.cpp")
add_executable(aplicacion ${APLICACION_SRC})
target_include_directories(aplicacion PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(aplicacion PRIVATE Grafico Reglas Scripting Opciones IrrlichtMt::IrrlichtMt)
if(APPLE)
  target_link_libraries(aplicacion PRIVATE "-framework Cocoa" "-framework OpenGL" "-framework IOKit")
endif()
```

- [ ] **Step 2: Fix Aplicacion includes**

| File | Old | New |
|---|---|---|
| `Aplicacion.hpp` | `<irrlicht/irrlicht.h>` | `<irrlicht.h>` |
| `ManejadorJuego.hpp` | `<irrlicht/irrList.h>` | `<irrList.h>` |
| `PartidaGrafica.cpp` | `<irrlicht/vector3d.h>` | `<vector3d.h>` |
| `PartidaGrafica.hpp` | `<irrlicht/irrlicht.h>` | `<irrlicht.h>` |

`EventReceiver.hpp` uses `<irrlicht.h>` — NO CHANGE.

- [ ] **Step 3: Commit**

```bash
git add src/Aplicacion/
git commit -m "build: Aplicacion CMakeLists + IrrlichtMt include path fixes (5 files)"
```

---

### Task 4: Build — compile both targets

**Files:** None (build verification)

- [ ] **Step 1: CMake configure**

```bash
. /nix/var/nix/profiles/default/etc/profile.d/nix-daemon.sh && nix develop -c bash -c 'cmake -S . -B build -G Ninja -DQM_CONSOLE_ONLY=OFF 2>&1 | tail -10'
```
Expected: Configuring done, no errors.

- [ ] **Step 2: Build all**

```bash
. /nix/var/nix/profiles/default/etc/profile.d/nix-daemon.sh && nix develop -c bash -c 'cmake --build build --target aplicacion consola 2>&1 | tail -30'
```

- [ ] **Step 3: Triage compile errors**

Common expected errors and fixes:

**`IAnimatorCameraTokayo.hpp`**: If `ISceneNodeAnimator.h` has a different virtual method signature, compare with IrrlichtMt header. The animator overrides `animateNode()` — signature must match exactly.

**`CImageGUISkin`**: If `IGUISkin` in IrrlichtMt added new pure virtual methods, implement stubs. Check `IGUISkin.h` in IrrlichtMt for methods not in the original.

**`CGUIProgressBar::draw()`**: Must match `IGUIElement::draw()` signature exactly.

**`irrList.h`** (ManejadorJuego.hpp): If IrrlichtMt removed `irrList.h`, replace `core::list<T>` with `std::list<T>` in any file that uses it. Check `ManejadorJuego.hpp` usage — likely just `core::list<Partida*>` → `std::list<Partida*>`.

**`createDevice` in Video.cpp**: If signature mismatch, compare with engine spike (`spikes/engine/main.cpp`) which uses `createDevice(EDT_OPENGL, ...)` successfully.

- [ ] **Step 4: Commit fixes if any, or report DONE**

---

### Task 5: Runtime — first window

**Files:** None (runtime verification)

- [ ] **Step 1: Stage Reglas.so for Python embedding**

```bash
mkdir -p lib && ln -sf ../build/src/AgenteWrapper/Reglas.so lib/Reglas.so
```

- [ ] **Step 2: Run aplicacion**

```bash
. /nix/var/nix/profiles/default/etc/profile.d/nix-daemon.sh && nix develop -c bash -c './build/src/Aplicacion/aplicacion 2>&1; echo "EXIT=$?"'
```

- [ ] **Step 3: Triage runtime errors**

Common expected errors:

**3DS mesh not loading:** Error "Cannot load mesh file X.3ds". IrrlichtMt may not include a 3DS loader. Options:
1. Convert `.3ds` → `.obj` or `.b3d` (Blender quick-export)
2. Check if IrrlichtMt's `IMeshLoader` list includes 3DS: `smgr->getMesh("test.3ds")` returns null → no loader
3. Asset files are in `conf/` directory — `opciones.conf` references them

**Texture paths:** Game loads textures via relative paths from `conf/`. Verify the working directory is correct when running `aplicacion`.

**`createDevice` fail:** If `createDevice(EDT_OPENGL, ...)` returns null on macOS, try `EDT_BURNINGSVIDEO` (software renderer) as fallback. IrrlichtMt's OpenGL driver works on macOS (proven by engine spike).

**Segfault in GUI init:** `CImageGUISkin` constructor calls virtual methods — ensure the vtable is fully constructed. If crash is in `drawElementStyle`, check `IVideoDriver` methods.

- [ ] **Step 4: Document findings, commit any fixes**

---

### Task 6: Gate — console regression + graphical match

**Files:** None (verification only)

- [ ] **Step 1: Console regression**

```bash
ln -sf ../build/src/AgenteWrapper/Reglas.so lib/Reglas.so
. /nix/var/nix/profiles/default/etc/profile.d/nix-daemon.sh && nix develop -c bash -c 'build/src/Consola/consola bin/agenteCamina.py bin/agenteCamina2.py </dev/null 2>&1 | tail -3; echo "EXIT=$?"'
```
Expected: `Hay un ganador!`, exit 0.

- [ ] **Step 2: Graphical match attempt**

```bash
. /nix/var/nix/profiles/default/etc/profile.d/nix-daemon.sh && nix develop -c bash -c './build/src/Aplicacion/aplicacion 2>&1; echo "EXIT=$?"'
```
Expected: Window opens, 3D board visible, AI agents move pieces, match completes to "Hay un ganador!" dialog.

- [ ] **Step 3: otool -L verification**

```bash
otool -L build/src/Aplicacion/aplicacion | grep -iE 'irrlicht|Grafico|Reglas|Scripting|boost_python|Reglas\.so'
```
Expected: lists IrrlichtMt, libGrafico, libReglas, libScripting. Does NOT list boost_python or Reglas.so.

- [ ] **Step 4: Commit if any fix, otherwise report DONE**

---

### Task 7: ROADMAP + cerebrum + push + PR

**Files:**
- Modify: `docs/superpowers/ROADMAP.md`
- Modify: `.wolf/cerebrum.md`

- [ ] **Step 1: Update ROADMAP**

Flip D2 from `⬜ NEXT` → `✅ Done`.

- [ ] **Step 2: Log to cerebrum**

Record key decisions: IrrlichtMt API compatibility findings, include path strategy, asset format resolution, any GUI API drift fixes.

- [ ] **Step 3: Verify gh account, push, open PR**

```bash
gh api user --jq .login  # must be "fabzter"
git push origin feat/d2-graphical
gh pr create --base main --head feat/d2-graphical \
  --title "D2: Graphical port — IrrlichtMt + Aplicacion" \
  --body "Ports Grafico and Aplicacion to IrrlichtMt 1.9.0mt15.\\n\\n- 27 files: include path fixes, 2 new CMakeLists\\n- QM_CONSOLE_ONLY flipped OFF\\n- Graphical AI-vs-AI match plays in 3D window\\n- Console regression verified\\n\\nCloses D2."
gh auth switch --user fabriziohernandez
```

---

### Task 8: Cleanup after merge

After the user merges on GitHub:

```bash
git checkout main && git pull origin main
git branch -d feat/d2-graphical
git push origin --delete feat/d2-graphical
```
