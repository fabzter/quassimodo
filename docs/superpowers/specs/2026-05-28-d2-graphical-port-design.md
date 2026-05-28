# D2 — Graphical Port Design

> Ports `src/Grafico/` and `src/Aplicacion/` to IrrlichtMt, bringing up a
> graphical playable match. Completes spec-D (graphical) + spec-E (GUI).

## 1. Goal & non-goals

**Goal:** Get the full graphical application compiling, linking, and running a
playable match against IrrlichtMt on macOS arm64. The existing `PartidaGrafica`
+ `Aplicacion` + `ManejadorJuego` architecture is unchanged — we compile it,
fix Irrlicht API drift, and verify a 3D board with two AI agents plays to
completion.

**Non-goals:** Rewriting the GUI (CImageGUISkin, CGUIProgressBar, menus stay as-is),
adding new features, fixing pre-existing rendering bugs, porting to a different
engine. The IrrlichtMt fork is the target — we don't support stock Irrlicht.

## 2. Architecture (unchanged from 2010)

```
aplicacion (binary)
├── libGrafico.dylib     ← 3D board, pieces, barriers, camera, GUI skin
├── libReglas.dylib      ← game rules (unchanged from D1)
├── libScripting.dylib   ← Python embedder (pybind11 from D1′)
├── libOpciones.dylib    ← config reader (unchanged from D1)
├── IrrlichtMt::IrrlichtMt ← engine
└── frameworks: Cocoa, OpenGL, IOKit
```

`Reglas.so` is still loaded at runtime by `libScripting` — same topology as D1/D1′.

`Aplicacion` creates the Irrlicht device via `Grafico::Video`, then `ManejadorJuego`
orchestrates everything. When graphics are available (`EDT_NULL` check passes),
`ManejadorJuego` uses `PartidaGrafica`; otherwise falls back to `PartidaConsola`
(same code path D1's `consola` binary uses, but embedded in the same process).

## 3. Grafico module — what needs to change

### 3.1 Include path (22 files, mechanical)

Original: `#include <irrlicht/XXX.h>` or `#include <irrlicht.h>`
IrrlichtMt: headers are at `include/irrlichtmt/` with flat structure

Fix: change all `#include <irrlicht/XXX.h>` to `#include <XXX.h>` and
`#include <irrlicht.h>` to `#include <irrlicht.h>` (the umbrella header name
is the same — IrrlichtMt kept it).

Files affected (every `.hpp`/`.h`/`.cpp` in `src/Grafico/` and `src/Aplicacion/`):

| File | Old include | New include |
|---|---|---|
| Antorcha.cpp | `<irrlicht/ISceneManager.h>`, `<irrlicht/ILightSceneNode.h>`, `<irrlicht/IParticleSystemSceneNode.h>` | `<ISceneManager.h>`, `<ILightSceneNode.h>`, `<IParticleSystemSceneNode.h>` |
| Antorcha.hpp | `<irrlicht.h>` | `<irrlicht.h>` (unchanged) |
| Barrera.hpp | `<irrlicht.h>`, `<irrlicht/vector3d.h>` | `<irrlicht.h>`, `<vector3d.h>` |
| CGUIProgressBar.cpp | `<IGUIEnvironment.h>`, `<IGUIFont.h>` | unchanged (already flat) |
| CGUIProgressBar.h | `<IGUIElement.h>`, `<SColor.h>` | unchanged |
| CImageGUISkin.cpp | `<IVideoDriver.h>`, `<ITexture.h>` | unchanged |
| CImageGUISkin.h | `<IGUISkin.h>`, `<irrString.h>` | `<IGUISkin.h>`, `<irrString.h>` |
| Celda.hpp | `<irrlicht.h>` | `<irrlicht.h>` (unchanged) |
| GUI.hpp | `<irrlicht/irrlicht.h>` | `<irrlicht.h>` |
| IAnimatorCameraTokayo.hpp | `<irrlicht/ISceneNodeAnimator.h>` | `<ISceneNodeAnimator.h>` |
| Jugador.cpp | `<irrlicht/IAnimatedMeshSceneNode.h>`, `<irrlicht/IShadowVolumeSceneNode.h>` | `<IAnimatedMeshSceneNode.h>`, `<IShadowVolumeSceneNode.h>` |
| Jugador.hpp | `<irrlicht.h>` | `<irrlicht.h>` (unchanged) |
| JumpAnimator.hpp | `<irrlicht/irrlicht.h>`, `<irrlicht/ISceneNodeAnimator.h>` | `<irrlicht.h>`, `<ISceneNodeAnimator.h>` |
| ManejadorGUI.hpp | `<irrlicht/irrlicht.h>` | `<irrlicht.h>` |
| Menu.hpp | `<irrlicht.h>`, `<irrlicht/ICameraSceneNode.h>`, `<irrlicht/vector3d.h>` | `<irrlicht.h>`, `<ICameraSceneNode.h>`, `<vector3d.h>` |
| Pieza.cpp | `<irrlicht/ISceneNode.h>` | `<ISceneNode.h>` |
| Pieza.hpp | `<irrlicht.h>` | `<irrlicht.h>` (unchanged) |
| Skin.hpp | `<irrlicht/irrlicht.h>` | `<irrlicht.h>` |
| SkinLoader.cpp | `<IReadFile.h>`, `<IVideoDriver.h>` | unchanged |
| Tablero.hpp | `<irrlicht.h>` | `<irrlicht.h>` (unchanged) |
| tokayoCamera.hpp | (various) | flatten |
| Video.hpp | (various) | flatten |

And in `src/Aplicacion/`:
| Aplicacion.hpp | `<irrlicht/irrlicht.h>` | `<irrlicht.h>` |
| EventReceiver.hpp | (various) | flatten |
| ManejadorJuego.hpp | `<irrlicht/irrList.h>` | `<irrList.h>` |
| PartidaGrafica.hpp | `<irrlicht/irrlicht.h>` | `<irrlicht.h>` |

### 3.2 IrrlichtMt API drift (runtime risk — compile first, fix errors)

Known Irrlicht 1.6 → 1.9.0mt15 changes that may affect Grafico:

- **`createDevice`**: IrrlichtMt uses `irr::createDeviceEx()` with `SIrrlichtCreationParameters`
  instead of the old `createDevice()` overloads. `Grafico::Video` calls `createDevice()` —
  may need updating to the new signature.
- **Material types**: `EMT_SOLID`, `EMT_TRANSPARENT_ALPHA_CHANNEL` etc. — likely preserved
  but enum values may have shifted.
- **Mesh loading**: `smgr->getMesh("file.3ds")` — IrrlichtMt may not include a 3DS loader.
  Game uses `.3ds` models for pieces. Verify format support.
- **GUI API**: `IGUISkin`, `IGUIElement`, `IGUIEnvironment` — headers present, API likely
  compatible. The custom `CImageGUISkin` extends `IGUISkin` (present).
  `CGUIProgressBar` extends `IGUIElement` (present). Risk is low but these are
  the Minetest-trimmed classes — verify at compile time.
- **Light/Shadow API**: `ILightSceneNode`, `IShadowVolumeSceneNode`, particle systems —
  present in IrrlichtMt headers. `Antorcha` (torch/flame effect) uses these.
- **Scene node animators**: `ISceneNodeAnimator`, custom `JumpAnimator`,
  `IAnimatorCameraTokayo` — API likely compatible.

**Strategy:** fix includes first, attempt build, fix compile errors one by one.
Most will be header paths; actual API incompatibilities will surface at compile time
and are individually small (renamed enums, changed method signatures).

### 3.3 Aplicacion module

`src/Aplicacion/` already has a `Makefile` and `nbproject/` (NetBeans). We add a
`CMakeLists.txt` that mirrors the structure: link against Grafico, Reglas, Scripting,
Opciones, IrrlichtMt, and frameworks.

The `main()` function lives in `src/Consola/main.cpp` for D1. For D2, we need the
original `Aplicacion` entry point. Check: does `src/Aplicacion/` have a `main.cpp`?

If not, the `Aplicacion` class is instantiated from somewhere — probably a separate
`main()` in the original NetBeans project (likely `src/Aplicacion/main.cpp` was the
original entry point, and D1's `Consola` replaced it). We recover the original
`main()` or create a minimal one that constructs `Aplicacion`.

## 4. Build changes

### 4.1 `src/Grafico/CMakeLists.txt` (new file)

```cmake
file(GLOB GRAFICO_SRC CONFIGURE_DEPENDS "*.cpp")
add_library(Grafico SHARED ${GRAFICO_SRC})
target_include_directories(Grafico PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(Grafico PUBLIC Reglas Opciones IrrlichtMt::IrrlichtMt)
if(APPLE)
  target_link_libraries(Grafico PRIVATE "-framework Cocoa" "-framework OpenGL" "-framework IOKit")
endif()
```

### 4.2 `src/Aplicacion/CMakeLists.txt` (new file)

```cmake
file(GLOB APLICACION_SRC CONFIGURE_DEPENDS "*.cpp")
add_executable(aplicacion ${APLICACION_SRC})
target_include_directories(aplicacion PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(aplicacion PRIVATE Grafico Reglas Scripting Opciones IrrlichtMt::IrrlichtMt)
if(APPLE)
  target_link_libraries(aplicacion PRIVATE "-framework Cocoa" "-framework OpenGL" "-framework IOKit")
endif()
```

### 4.3 `src/CMakeLists.txt` — flip `QM_CONSOLE_ONLY`

Replace the single `QM_CONSOLE_ONLY` option with per-module control:

```cmake
option(QM_CONSOLE_ONLY "Build console-only (exclude Grafico/PartidaGrafica)" OFF)
# ...
if(NOT QM_CONSOLE_ONLY)
  add_subdirectory(Grafico)
  add_subdirectory(Aplicacion)
endif()
```

### 4.4 `flake.nix` — no changes

IrrlichtMt is already in the flake from the engine spike. No new deps.

## 5. Gate (three stages)

### Stage 1 — compile
```bash
cmake -S . -B build -G Ninja -DQM_CONSOLE_ONLY=OFF
cmake --build build --target aplicacion consola
```
Both `aplicacion` and `consola` must compile and link. `consola` must still play
a console match (regression check — D1/D1′ gate unchanged).

### Stage 2 — window opens
```bash
./build/src/Aplicacion/aplicacion
```
An IrrlichtMt window opens with the Quassimodo 3D board, GUI skin, and menu.
May crash at this stage — triage IrrlichtMt API drift errors one at a time.

### Stage 3 — graphical match
Full AI-vs-AI match plays to completion in the graphical window. Two agents
move pieces on the 3D board, barriers appear, winner declared via GUI dialog.
Exit 0 on match completion (or window close).

## 6. Risks

**R1 — 3DS mesh loader removed.** IrrlichtMt may not ship a 3DS loader (Minetest
uses `.b3d` and `.x` formats). If 3DS loading fails, we need to convert game assets
(3 models, textures) to a supported format or add a loader. Mitigation: check
IrrlichtMt's `IMeshLoader` list at compile time.

**R2 — `createDevice` API change.** `Grafico::Video` calls `createDevice()` with
the old signature. IrrlichtMt may use `createDeviceEx()` with a parameters struct.
The engine spike used `createDevice()` successfully — verify the same call site.

**R3 — GUI API incompatibility.** `CImageGUISkin` overrides ~15 virtual methods from
`IGUISkin`. If IrrlichtMt added/removed any pure virtual methods in `IGUISkin`, the
class won't compile. Mitigation: check `IGUISkin.h` in IrrlichtMt for diffs vs. 1.6.

**R4 — Texture format support.** Game assets are `.jpg`/`.png`/`.bmp` — these are
standard and IrrlichtMt should support them via the image loaders. Low risk.

**R5 — `irrList.h` in ManejadorJuego.** IrrlichtMt has `irrList.h` but it may be
deprecated in favor of STL containers. If removed, replace with `std::list`.

**R6 — Python embedding in graphical mode.** `Aplicacion` creates `ManejadorJuego`
which creates `Scripting::Manejador` which embeds Python. D1 proved this works.
D2 runs the same code path — only the rendering is added. The embedding is already
proven. Risk: none.

**R7 — Regression: consola still works.** Flipping `QM_CONSOLE_ONLY=OFF` must not
break the console binary. Verify with the D1 gate command after D2 build.

## 7. Files summary

| Action | File |
|---|---|
| **Create** | `src/Grafico/CMakeLists.txt` |
| **Create** | `src/Aplicacion/CMakeLists.txt` |
| **Modify** | `src/CMakeLists.txt` (flip QM_CONSOLE_ONLY, add Grafico/Aplicacion) |
| **Modify** | 22 files in `src/Grafico/` — include path fixes |
| **Modify** | 4 files in `src/Aplicacion/` — include path fixes |
