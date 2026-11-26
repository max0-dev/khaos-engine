# Khaos Engine

A game engine written in C++20.

## Modules

| Module    | Type           | Description                                          |
|-----------|----------------|------------------------------------------------------|
| `core`    | static library | Engine core (`khaos::core`)                          |
| `editor`  | executable     | `KhaosEditor` — Dear ImGui editor (GLFW + OpenGL)    |
| `tests`   | executable     | `khaos_tests` — Catch2 unit tests, run via CTest     |

Dependencies (via vcpkg manifest): GLFW, GLEW, Dear ImGui (docking), spdlog, Catch2.

## Requirements

- Windows, Visual Studio 2026 (latest MSVC, "Desktop development with C++")
- CMake ≥ 3.25 and Ninja (both ship with Visual Studio)

## Getting started

```bash
git clone --recursive https://github.com/max0-dev/khaos-engine
cd khaos-engine
# if you cloned without --recursive:
git submodule update --init
```

Build from a *Developer PowerShell for VS* (the first configure builds the vcpkg packages and takes a few minutes):

```bash
cmake --preset msvc                 # one configure (Ninja Multi-Config)
cmake --build --preset debug        # or: release, relwithdbg
ctest --preset debug
```

Everything (exe, dll, lib, pdb) lands in `out/<Config>/`.

## VSCode

Install the recommended extensions (clangd, C/C++, CMake Tools). Pick the `msvc` configure preset and a
`debug` / `release` / `relwithdbg` build preset in the CMake Tools status bar; clangd provides IntelliSense from
`build/compile_commands.json`, and **F5** debugs with the MSVC debugger.
