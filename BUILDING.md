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

## What's pinned

See `flake.nix` (toolchain + deps), `nix/irrlicht-fork.nix` (the Minetest Irrlicht
fork engine), and `flake.lock` (exact nixpkgs revision). Reproducible across machines.
