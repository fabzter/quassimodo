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
./build/spikes/pyembed_spike build/spikes   # prints ROUNDTRIP OK
./build/spikes/engine_spike                  # opens a window, prints ENGINE WINDOW OK
```

Note: `pyembed_spike` takes the module directory (`build/spikes`) as its first
argument so it can locate `Spike.so` at runtime.

## Build & run the console game (Phase D1)

The non-graphical game builds as the `consola` executable (rules engine + embedded
Python 3 + the `Reglas` Boost.Python extension; no graphics). Two Python AI agents
play a full Quoridor match in the terminal.

```
# Build everything (libReglas, libScripting, Reglas.so, consola, …)
cmake -S . -B build -G Ninja
cmake --build build

# The embedded interpreter looks for the Reglas extension on ./lib (and ../lib,
# ../../lib). Stage it once, run from the repo root:
mkdir -p lib && ln -sf "$PWD/build/src/AgenteWrapper/Reglas.so" lib/Reglas.so

# Run an AI-vs-AI match (agent paths are argv; stdin from /dev/null skips the
# between-moves pause). Prints the board each turn, then "Hay un ganador!".
./build/src/Consola/consola bin/agenteCamina.py bin/agenteCamina2.py </dev/null
```

Any two agents from `bin/*.py` work, e.g. `bin/agenteMiniMax2.py`. The graphical
client (`Aplicacion` + Irrlicht) is Phase D2 and is not built yet.

## What's pinned

See `flake.nix` (toolchain + deps), `nix/irrlicht-fork.nix` (the Minetest Irrlicht
fork engine), and `flake.lock` (exact nixpkgs revision). Reproducible across machines.
