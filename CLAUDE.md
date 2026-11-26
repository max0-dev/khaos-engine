# Khaos Engine — notes for Claude

Game engine in C++20. Windows, latest MSVC, CMake + Ninja, vcpkg (git submodule, manifest mode).

## Ownership — read this first
- **`core/` belongs to the user.** They write the engine core and decide the architecture.
  Do not restructure, redesign, or add systems to `core/` on your own. You may fix obvious bugs
  or add tiny helpers the editor needs, but propose them first and keep them minimal.
- **`editor/` is led by Claude** (Dear ImGui, docking branch). Build it out proactively, but:
  - Keep the user updated: after each editor change, summarise *what* changed, *which files*,
    and any assumptions made about core APIs.
  - Keep the code easy for the user to change by hand: small, flat panel classes, no clever
    abstractions, no hidden global state beyond ImGui's own.
  - Consume core only through its public headers (`core/include/khaos/...`). If the editor
    needs something core doesn't expose, ask rather than reaching into core internals.
- **`tests/`**: Catch2 v3. Add tests for new core/editor logic (see `/cr`).

## Layout
- `core/`   → static lib `khaos::core`, public headers in `core/include/khaos/core/`
- `editor/` → executable `KhaosEditor` (GLFW + GLEW + OpenGL 4.5 + ImGui)
- `tests/`  → executable `khaos_tests` (Catch2WithMain, registered with CTest)
- `out/<Config>/` → all outputs (exe, dll, lib, pdb); `build/` → single multi-config build tree
- `khaos::options` → interface target with shared warnings; link it PRIVATE in every target.

## Build (from a VS developer shell, or let VSCode CMake Tools set up the env)
```
cmake --preset msvc                       # configure once (Ninja Multi-Config)
cmake --build --preset debug && ctest --preset debug
cmake --build --preset release            # or relwithdbg
```
From a plain shell, prefix with
`"C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" &&`
(run through `cmd /c`).

New dependency → add it to `vcpkg.json`, then `find_package` + link in the module's CMakeLists.

## Style
- C++20, `namespace khaos` (editor: `khaos::editor`), `PascalCase` types, `camelCase` functions,
  `m_` members, `s_` statics, `#pragma once`.
- Warnings are `/W4 /permissive-`; keep the build warning-free.

## Research
When asked to research (APIs, engine design, ImGui patterns, graphics techniques), use the
`researcher` subagent or WebSearch/WebFetch directly. Prefer primary sources (official docs,
GitHub source, papers). Cite links, note the version a source applies to, and say plainly when
something is uncertain. Present options for core architecture questions — the user decides.
