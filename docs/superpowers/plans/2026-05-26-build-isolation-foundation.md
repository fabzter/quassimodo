# Build Isolation Foundation (Phases A–C) Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Stand up an isolated, reproducible Devbox/Nix + CMake build environment on macOS arm64 and prove the two highest-risk capabilities — the Minetest Irrlicht fork opens a native window, and embedded Python + Boost.Python share one type registry across the C++↔Python boundary.

**Architecture:** A `flake.nix` pins the whole toolchain + dependencies (clang, CMake, Ninja, Python 3, Boost with Python enabled, the Irrlicht fork via a custom derivation); Devbox wraps the flake for a friendly `devbox shell`. Two standalone "spike" programs live under `spikes/`, built with CMake, validating the engine and the Python-embedding linkage rules from the spec (one shared boost_python, explicit linking, extension as a `MODULE` with `-undefined dynamic_lookup`).

**Tech Stack:** Nix (flakes) + Devbox, CMake + Ninja, clang, Python 3, Boost.Python, Minetest's Irrlicht fork.

**Scope:** Phases A–C of `docs/superpowers/specs/2026-05-26-build-isolation-design.md`. Phases D (source port) and E (runtime bring-up) are a separate plan written after this one's spikes pass.

**Spike caveat:** Tasks 5–8 are validation spikes against dependencies we have not yet built on this platform. Each provides concrete starting artifacts plus an explicit verification gate. When a build/derivation fails, treat the error as expected discovery: use `superpowers:systematic-debugging`, adjust the artifact, and re-run the same verification — do not skip the gate.

---

## File Structure

```
/flake.nix                     # pinned toolchain + deps; exposes a devShell + the irrlicht-fork package
/flake.lock                    # reproducibility lock (committed)
/devbox.json                   # wraps the flake's devShell for `devbox shell`
/nix/irrlicht-fork.nix         # custom derivation: Minetest Irrlicht fork on darwin
/CMakeLists.txt                # top-level; options to build spikes
/cmake/                        # (reserved) helper modules
/spikes/CMakeLists.txt         # builds the two spikes
/spikes/engine/main.cpp        # opens an Irrlicht window
/spikes/pyembed/widget.hpp     # C++ type with a virtual method (domain-type stand-in)
/spikes/pyembed/widget.cpp     # its implementation -> built into libwidget (shared)
/spikes/pyembed/module.cpp     # Boost.Python MODULE wrapping Widget -> Spike.so
/spikes/pyembed/main.cpp       # embedder: round-trips a Widget C++<->Python<->C++
/spikes/pyembed/agent.py       # Python subclass of Widget used by the round-trip
/BUILDING.md                   # how to enter the env and build
.gitignore                     # add Nix/CMake build outputs
```

Responsibilities:
- `flake.nix` / `nix/irrlicht-fork.nix` — the entire isolated dependency closure. Single source of "what versions."
- `spikes/pyembed/*` — mirrors the real app's linkage topology in miniature: `libwidget` (= `libReglas`), `Spike.so` (= `Reglas.so`), `main` embedder (= `aplicacion`/`libScripting`).
- `spikes/engine/main.cpp` — proves the engine + native window.

---

## Task 1: Install Devbox and Nix

**Files:** none (host tooling).

- [ ] **Step 1: Check whether Devbox/Nix already exist**

Run:
```bash
command -v devbox && devbox version; command -v nix && nix --version
```
Expected: likely "not found" for both (confirmed earlier). If both exist, skip to Task 2.

- [ ] **Step 2: Install Devbox (installs Nix if missing)**

Run:
```bash
curl -fsSL https://get.jetify.com/devbox | bash
```
Follow prompts; it installs Nix (multi-user) if absent. This is a host-level install — the only non-isolated step in the plan.

- [ ] **Step 3: Verify and enable flakes**

Run:
```bash
nix --version && devbox version
mkdir -p ~/.config/nix && grep -q 'experimental-features' ~/.config/nix/nix.conf 2>/dev/null || echo 'experimental-features = nix-command flakes' >> ~/.config/nix/nix.conf
```
Expected: both print versions; nix.conf enables flakes.

- [ ] **Step 4: Commit (nothing to commit — host tooling)**

No repo change. Proceed.

---

## Task 2: Pin the toolchain + base deps in a flake

**Files:**
- Create: `flake.nix`
- Create: `nix/irrlicht-fork.nix` (stub for now; real derivation in Task 7)
- Modify: `.gitignore`

- [ ] **Step 1: Write `flake.nix` with a devShell (no engine yet)**

```nix
{
  description = "Quassimodo isolated build environment";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.11";

  outputs = { self, nixpkgs }:
    let
      systems = [ "aarch64-darwin" "x86_64-darwin" "x86_64-linux" "aarch64-linux" ];
      forAll = f: nixpkgs.lib.genAttrs systems (s: f (import nixpkgs { system = s; }));
    in {
      # Boost built WITH python enabled — required for boost_python (one shared lib).
      packages = forAll (pkgs: {
        boostPython = pkgs.boost.override {
          enablePython = true;
          python = pkgs.python311;
        };
      });

      devShells = forAll (pkgs: {
        default = pkgs.mkShell {
          packages = [
            pkgs.cmake
            pkgs.ninja
            pkgs.pkg-config
            pkgs.python311
            (pkgs.boost.override { enablePython = true; python = pkgs.python311; })
          ];
        };
      });
    };
}
```

- [ ] **Step 2: Write a placeholder `nix/irrlicht-fork.nix` so the path exists**

```nix
# Real derivation lands in Task 7. Placeholder keeps the file referenced.
{ lib }: lib.warn "irrlicht-fork.nix not yet implemented" null
```

- [ ] **Step 3: Add build outputs to `.gitignore`**

Append to `.gitignore`:
```
# Build outputs
/build/
/spikes/**/build/
result
result-*
.devbox/
```

- [ ] **Step 4: Lock and enter the shell**

Run:
```bash
nix flake lock
nix develop -c bash -c 'which cmake; which clang++; cmake --version | head -1; python3 --version'
```
Expected: `cmake` and `clang++` paths under `/nix/store/...`; versions print. If clang isn't present (Linux), add `pkgs.clang` to the devShell packages and re-run.

- [ ] **Step 5: Verify isolation (host tools NOT used)**

Run:
```bash
nix develop -c bash -c 'case "$(which cmake)" in /nix/store/*) echo ISOLATED;; *) echo "LEAK: $(which cmake)";; esac'
```
Expected: `ISOLATED`.

- [ ] **Step 6: Commit**

```bash
git add flake.nix flake.lock nix/irrlicht-fork.nix .gitignore
git commit -m "build: pin toolchain + boost-python in a nix flake devShell"
```

---

## Task 3: Wrap the flake with Devbox

**Files:**
- Create: `devbox.json`

- [ ] **Step 1: Write `devbox.json` referencing the flake devShell**

```json
{
  "$schema": "https://raw.githubusercontent.com/jetify-com/devbox/main/.schema/devbox.schema.json",
  "shell": {
    "init_hook": [
      "echo 'Quassimodo dev shell (isolated). cmake: '$(command -v cmake)"
    ]
  },
  "include": [
    "path:flake.nix"
  ]
}
```

- [ ] **Step 2: Verify `devbox shell` enters the isolated env**

Run:
```bash
devbox run -- bash -c 'case "$(which cmake)" in /nix/store/*) echo ISOLATED;; *) echo LEAK;; esac'
```
Expected: `ISOLATED`. If `include`/`path:` is unsupported by the installed Devbox version, fall back to listing packages directly in `devbox.json` (`"packages": ["cmake","ninja","pkg-config","python311"]`) and keep the flake for `boostPython`; document the chosen approach in BUILDING.md (Task 9).

- [ ] **Step 3: Commit**

```bash
git add devbox.json
git commit -m "build: add devbox wrapper around the flake devShell"
```

---

## Task 4: Top-level CMake skeleton + a trivial target

**Files:**
- Create: `CMakeLists.txt`
- Create: `spikes/CMakeLists.txt`
- Create: `spikes/hello.cpp` (temporary smoke target, removed in Task 8)

- [ ] **Step 1: Write the temporary smoke source**

`spikes/hello.cpp`:
```cpp
#include <iostream>
int main() { std::cout << "quassimodo build env OK\n"; return 0; }
```

- [ ] **Step 2: Write `CMakeLists.txt`**

```cmake
cmake_minimum_required(VERSION 3.25)
project(quassimodo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(QM_BUILD_SPIKES "Build the de-risking spikes" ON)

if(QM_BUILD_SPIKES)
  add_subdirectory(spikes)
endif()
```

- [ ] **Step 3: Write `spikes/CMakeLists.txt` (smoke target only for now)**

```cmake
add_executable(hello hello.cpp)
```

- [ ] **Step 4: Configure + build inside the isolated env**

Run:
```bash
nix develop -c bash -c 'cmake -S . -B build -G Ninja && cmake --build build && ./build/spikes/hello'
```
Expected: configures, builds, prints `quassimodo build env OK`.

- [ ] **Step 5: Commit**

```bash
git add CMakeLists.txt spikes/CMakeLists.txt spikes/hello.cpp
git commit -m "build: top-level CMake skeleton + smoke target"
```

---

## Task 5: Python round-trip spike — the C++ side library

**Files:**
- Create: `spikes/pyembed/widget.hpp`
- Create: `spikes/pyembed/widget.cpp`

This `Widget` stands in for `libReglas` types: a polymorphic class crossed both ways.

- [ ] **Step 1: Write `spikes/pyembed/widget.hpp`**

```cpp
#pragma once
#include <string>

// Domain type stand-in (mirrors Reglas:: classes). Lives in ONE shared lib.
class Widget {
public:
    virtual ~Widget() = default;
    // C++ default; Python subclasses override this.
    virtual std::string label() const { return "cpp-widget"; }
    int value() const { return value_; }
    void setValue(int v) { value_ = v; }
private:
    int value_ = 0;
};
```

- [ ] **Step 2: Write `spikes/pyembed/widget.cpp`**

```cpp
#include "widget.hpp"
// Out-of-line anchor so libwidget owns Widget's vtable/typeinfo (single RTTI source).
std::string widget_tu_anchor() { return "widget translation unit"; }
```

- [ ] **Step 3: Commit**

```bash
git add spikes/pyembed/widget.hpp spikes/pyembed/widget.cpp
git commit -m "spike(pyembed): Widget domain-type stand-in"
```

---

## Task 6: Python round-trip spike — the Boost.Python module + embedder + test

**Files:**
- Create: `spikes/pyembed/module.cpp`
- Create: `spikes/pyembed/main.cpp`
- Create: `spikes/pyembed/agent.py`
- Modify: `spikes/CMakeLists.txt`

- [ ] **Step 1: Write the Boost.Python module `spikes/pyembed/module.cpp`**

```cpp
#include <boost/python.hpp>
#include "widget.hpp"
using namespace boost::python;

// Wrapper enabling Python subclasses to override label().
struct WidgetWrap : Widget, wrapper<Widget> {
    std::string label() const override {
        if (override f = this->get_override("label")) return f();
        return Widget::label();
    }
    std::string default_label() const { return this->Widget::label(); }
};

BOOST_PYTHON_MODULE(Spike) {
    class_<WidgetWrap, boost::noncopyable>("Widget")
        .def("label", &Widget::label, &WidgetWrap::default_label)
        .def("value", &Widget::value)
        .def("setValue", &Widget::setValue);
}
```

- [ ] **Step 2: Write the embedder + assertion `spikes/pyembed/main.cpp`**

The embedder imports `Spike` as a **separate `.so`** off `sys.path` (mirroring how the
real app loads `Reglas.so`) — NOT linked into this binary. That is what actually tests
cross-shared-object registry + RTTI sharing. The module's directory is passed as `argv[1]`.

```cpp
#include <boost/python.hpp>
#include <iostream>
#include <string>
#include "widget.hpp"
using namespace boost::python;

int main(int argc, char** argv) {
    const std::string moddir = (argc > 1) ? argv[1] : ".";
    Py_Initialize();
    try {
        // Put the directory holding Spike.so on sys.path, then it's importable.
        object sys = import("sys");
        sys.attr("path").attr("insert")(0, moddir);

        object main_mod = import("__main__");
        object ns = main_mod.attr("__dict__");

        // 1) C++ -> Python: push a live C++ Widget into Python (uses boost_python
        //    converters registered by the SEPARATE Spike.so module).
        Widget cpp_w;
        cpp_w.setValue(42);
        ns["cpp_w"] = object(boost::python::ptr(&cpp_w));

        // 2) Run agent.py: imports Spike, subclasses Spike.Widget, reads cpp_w.
        exec_file("spikes/pyembed/agent.py", ns, ns);

        // 3) Python -> C++: pull the subclass instance back as a C++ Widget&
        //    (requires consistent RTTI between Spike.so and this binary via libwidget).
        Widget& py_w = extract<Widget&>(ns["made"]);

        std::string from_cpp_obj = extract<std::string>(ns["cpp_label"]);
        std::string from_py_obj  = py_w.label();
        int round_tripped_value  = extract<int>(ns["seen_value"]);

        std::cout << "cpp_w.label via python = " << from_cpp_obj << "\n";
        std::cout << "py subclass.label via C++ = " << from_py_obj << "\n";
        std::cout << "value seen in python = " << round_tripped_value << "\n";

        bool ok = (from_cpp_obj == "cpp-widget")
                && (from_py_obj == "python-widget")
                && (round_tripped_value == 42);
        std::cout << (ok ? "ROUNDTRIP OK\n" : "ROUNDTRIP FAIL\n");
        return ok ? 0 : 1;
    } catch (error_already_set&) {
        PyErr_Print();
        return 2;
    }
}
```

- [ ] **Step 3: Write `spikes/pyembed/agent.py`**

```python
import Spike

# Read the C++-created object that was injected into this namespace.
cpp_label = cpp_w.label()
seen_value = cpp_w.value()

class PyWidget(Spike.Widget):
    def label(self):
        return "python-widget"

made = PyWidget()
```

- [ ] **Step 4: Add the spike to `spikes/CMakeLists.txt`**

Append to `spikes/CMakeLists.txt`:
```cmake
find_package(Python3 REQUIRED COMPONENTS Development Interpreter)
find_package(Boost REQUIRED COMPONENTS python)

# Single shared domain lib (mirrors libReglas): ONE RTTI source for Widget,
# linked by BOTH the module and the embedder.
add_library(widget SHARED pyembed/widget.cpp)
target_include_directories(widget PUBLIC pyembed)

# Spike.so = the Boost.Python extension, a SEPARATE module (mirrors Reglas.so).
add_library(Spike MODULE pyembed/module.cpp)
set_target_properties(Spike PROPERTIES PREFIX "" SUFFIX ".so")
target_link_libraries(Spike PRIVATE widget Boost::python)
if(APPLE)
  # Resolve Python C-API symbols from the embedding process at load time.
  target_link_options(Spike PRIVATE -Wl,-undefined,dynamic_lookup)
endif()

# Embedder = aplicacion/libScripting stand-in. Links the SAME shared boost_python
# and the SAME libwidget; does NOT link the module — it imports Spike.so at runtime.
add_executable(pyembed_spike pyembed/main.cpp)
target_link_libraries(pyembed_spike PRIVATE widget Boost::python Python3::Python)
add_dependencies(pyembed_spike Spike)
```

- [ ] **Step 5: Build the spike (module + embedder)**

Run:
```bash
nix develop -c bash -c 'cmake -S . -B build -G Ninja && cmake --build build --target Spike pyembed_spike'
```
Expected: builds `build/spikes/Spike.so` and `build/spikes/pyembed_spike`. If `find_package(Boost COMPONENTS python)` fails, the flake's boost lacks python — confirm Task 2 used the `enablePython` override and that the devShell exposes it; re-lock and rebuild.

- [ ] **Step 6: Run the round-trip and verify**

Run (pass the directory containing `Spike.so`; run from repo root so `agent.py`'s relative path resolves):
```bash
nix develop -c bash -c './build/spikes/pyembed_spike build/spikes'
```
Expected output ends with `ROUNDTRIP OK` and exit code 0:
```
cpp_w.label via python = cpp-widget
py subclass.label via C++ = python-widget
value seen in python = 42
ROUNDTRIP OK
```
This proves, across a SEPARATE `.so`: one Boost.Python registry shared (the embedder's `boost::python::ptr(&cpp_w)` finds the converter registered inside `Spike.so`), RTTI consistent (`extract<Widget&>` of a Python-subclass instance works because `Widget`'s typeinfo comes solely from `libwidget`), and Python overrides reach C++. If it prints `ROUNDTRIP FAIL` or "no to-Python converter", debug with `superpowers:systematic-debugging` — most likely a duplicated registry (boost_python not a single shared lib) or `widget`/`Spike` not sharing the one `libwidget`.

- [ ] **Step 7: Commit**

```bash
git add spikes/pyembed/module.cpp spikes/pyembed/main.cpp spikes/pyembed/agent.py spikes/CMakeLists.txt
git commit -m "spike(pyembed): prove embedded Python + Boost.Python C++<->Python round-trip across a separate .so"
```

---

## Task 7: Engine derivation — Minetest Irrlicht fork on darwin

**Files:**
- Modify: `nix/irrlicht-fork.nix`
- Modify: `flake.nix`

- [ ] **Step 1: Identify the exact fork revision**

Run:
```bash
# The Minetest/Luanti Irrlicht fork. Confirm the current canonical repo + a tagged rev.
nix develop -c bash -c 'true'  # ensure env
echo "Pin: github repo minetest/irrlicht (or luanti-org/irrlicht), a release tag e.g. 1.9.0mt15"
```
Record the chosen `owner/repo`, `rev` (tag), and you will fill `sha256` from the first build error.

- [ ] **Step 2: Write `nix/irrlicht-fork.nix`**

```nix
{ lib, stdenv, fetchFromGitHub, cmake, libGL, libX11
, darwin ? null }:

stdenv.mkDerivation rec {
  pname = "irrlichtmt";
  version = "1.9.0mt15";  # adjust to the chosen tag

  src = fetchFromGitHub {
    owner = "minetest";          # adjust if the canonical repo moved
    repo = "irrlicht";
    rev = version;
    sha256 = lib.fakeSha256;     # replace with the hash nix prints on first build
  };

  nativeBuildInputs = [ cmake ];
  buildInputs = lib.optionals stdenv.hostPlatform.isLinux [ libGL libX11 ]
    ++ lib.optionals stdenv.hostPlatform.isDarwin
       (with darwin.apple_sdk.frameworks; [ Cocoa OpenGL IOKit ]);

  # IrrlichtMt builds a static lib by default; expose headers + lib.
  cmakeFlags = [ "-DBUILD_SHARED_LIBS=OFF" ];

  meta = {
    description = "Minetest's Irrlicht fork";
    platforms = lib.platforms.linux ++ lib.platforms.darwin;
  };
}
```

- [ ] **Step 3: Wire the derivation into `flake.nix`**

In `flake.nix`, add to the `packages` attrset (inside `forAll`):
```nix
        irrlichtFork = pkgs.callPackage ./nix/irrlicht-fork.nix {
          darwin = pkgs.darwin;
        };
```
And add it to the devShell `packages` list:
```nix
            (pkgs.callPackage ./nix/irrlicht-fork.nix { darwin = pkgs.darwin; })
```

- [ ] **Step 4: Build the derivation (expect a hash error first, then iterate)**

Run:
```bash
nix build .#irrlichtFork 2>&1 | tee /tmp/irr-build.log | tail -30
```
Expected on first run: a hash mismatch printing the real `sha256`. Copy it into `nix/irrlicht-fork.nix` (replace `lib.fakeSha256`) and re-run. Then resolve any darwin build errors (frameworks, C++ standard, deprecated GL) using `superpowers:systematic-debugging`, consulting how nixpkgs Luanti patches the fork on darwin.
Gate: `nix build .#irrlichtFork` succeeds and `ls result/include` shows Irrlicht headers, `ls result/lib` shows the library.

- [ ] **Step 5: Commit**

```bash
git add nix/irrlicht-fork.nix flake.nix flake.lock
git commit -m "build: add Minetest Irrlicht fork derivation for darwin"
```

---

## Task 8: Engine window spike

**Files:**
- Create: `spikes/engine/main.cpp`
- Modify: `spikes/CMakeLists.txt`
- Delete: `spikes/hello.cpp`

- [ ] **Step 1: Write `spikes/engine/main.cpp`**

```cpp
#include <irrlicht.h>
#include <iostream>
using namespace irr;

int main() {
    IrrlichtDevice* dev = createDevice(
        video::EDT_OPENGL, core::dimension2d<u32>(640, 480),
        16, false, false, false, nullptr);
    if (!dev) { std::cerr << "createDevice failed\n"; return 1; }

    video::IVideoDriver* drv = dev->getVideoDriver();
    int frames = 0;
    while (dev->run() && frames < 120) {   // ~2s then exit, so it works headless-ish in CI too
        drv->beginScene(true, true, video::SColor(255, 30, 30, 60));
        drv->endScene();
        ++frames;
    }
    dev->drop();
    std::cout << "ENGINE WINDOW OK (" << frames << " frames)\n";
    return 0;
}
```

- [ ] **Step 2: Update `spikes/CMakeLists.txt` — replace the smoke target with the engine spike**

In `spikes/CMakeLists.txt`, remove the `add_executable(hello hello.cpp)` line and append:
```cmake
# Engine spike: provided by the flake's irrlicht-fork derivation.
find_path(IRRLICHT_INCLUDE_DIR irrlicht.h)
find_library(IRRLICHT_LIB NAMES IrrlichtMt Irrlicht)

add_executable(engine_spike engine/main.cpp)
target_include_directories(engine_spike PRIVATE ${IRRLICHT_INCLUDE_DIR})
target_link_libraries(engine_spike PRIVATE ${IRRLICHT_LIB})

if(APPLE)
  target_link_libraries(engine_spike PRIVATE
    "-framework Cocoa" "-framework OpenGL" "-framework IOKit")
endif()
```

- [ ] **Step 3: Delete the temporary smoke source**

Run:
```bash
git rm spikes/hello.cpp
```

- [ ] **Step 4: Build the engine spike**

Run:
```bash
nix develop -c bash -c 'cmake -S . -B build -G Ninja && cmake --build build --target engine_spike'
```
Expected: builds `build/spikes/engine_spike`. If headers/lib aren't found, confirm the irrlicht-fork derivation is on the devShell path (Task 7 Step 3) and that `find_path`/`find_library` match the fork's install layout (adjust names if it installs `IrrlichtMt`).

- [ ] **Step 5: Run and verify a window opens**

Run:
```bash
nix develop -c bash -c './build/spikes/engine_spike'
```
Expected: a 640×480 window appears for ~2 seconds; stdout ends with `ENGINE WINDOW OK (120 frames)`. If `createDevice` returns null, debug the GL/Cocoa device path with `superpowers:systematic-debugging` (likely the fork needs a specific driver enum or a Cocoa main-thread/run-loop nuance on macOS).

- [ ] **Step 6: Commit**

```bash
git add spikes/engine/main.cpp spikes/CMakeLists.txt
git commit -m "spike(engine): open a native Irrlicht window on macOS arm64"
```

---

## Task 9: Document the environment + verify reproducibility

**Files:**
- Create: `BUILDING.md`

- [ ] **Step 1: Write `BUILDING.md`**

```markdown
# Building Quassimodo (isolated dev environment)

All builds run inside an isolated Nix/Devbox environment — your host Homebrew,
system Python, and global toolchain are NOT used.

## Enter the environment
- With Devbox: `devbox shell`
- Or directly with Nix: `nix develop`

Verify isolation: `which cmake` must print a `/nix/store/...` path.

## Build the spikes (Phases A–C)
```
cmake -S . -B build -G Ninja
cmake --build build
./build/spikes/pyembed_spike   # prints ROUNDTRIP OK
./build/spikes/engine_spike    # opens a window, prints ENGINE WINDOW OK
```

## What's pinned
See `flake.nix` (toolchain + deps), `nix/irrlicht-fork.nix` (engine), and
`flake.lock` (exact nixpkgs revision). Reproducible across machines.
```

- [ ] **Step 2: Verify reproducibility from a clean checkout**

Run:
```bash
tmp=$(mktemp -d); git clone . "$tmp/repro" && cd "$tmp/repro" \
  && nix develop -c bash -c 'cmake -S . -B build -G Ninja && cmake --build build --target pyembed_spike && ./build/spikes/pyembed_spike' ; cd - ; rm -rf "$tmp"
```
Expected: a fresh clone builds and prints `ROUNDTRIP OK` using only the locked env.

- [ ] **Step 3: Commit**

```bash
git add BUILDING.md
git commit -m "docs: how to enter the isolated env and build the spikes"
```

---

## Definition of Done (this plan)

- `devbox shell` / `nix develop` enters an env where `which cmake`/`clang++` resolve into the Nix store (host tools provably unused), and `flake.lock` is committed.
- `pyembed_spike` prints `ROUNDTRIP OK` — embedded Python + Boost.Python share one registry; C++↔Python crossing + RTTI work on macOS arm64.
- `engine_spike` opens a native window and prints `ENGINE WINDOW OK`.
- A fresh clone reproduces the build.

When all gates pass, write the **Phases D–E plan** (real source port + runtime bring-up) against the now-validated toolchain and the engine/Boost.Python API surface the spikes exposed.
```
