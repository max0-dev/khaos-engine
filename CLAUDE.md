# CLAUDE.md

C++20 project on Linux: CMake + Ninja + clang, dependencies through vcpkg (manifest `vcpkg.json`, submodule `vcpkg/`).

## Project map
- `core/src` (+ `core/src/include`): library with the logic. Headers in `src/include/`, sources in `src/`, in matching folders.
- `app/src`: the executable, kept thin.
- `tests/src`: Catch2 v3 tests, its own target linking `core`. Never add tests to `core` or `app`.
- `cmake/ProjectOptions.cmake`: `project_setup_target()` adds warnings and sanitizers. Call it on every new target.
- Never edit `build/`, `out/`, or `vcpkg/`.

## Build / test
- `cmake --workflow --preset debug` runs configure, build and test (Debug with ASan + UBSan).
- Other presets: `relwithdebinfo`, `release`. Programs go to `out/<preset>/`.
- Sources are globbed (`CONFIGURE_DEPENDS`), so new files need no CMake edits.

## Conventions
- Member variables are `mVariable`; types and methods are PascalCase.
- Format with `.clang-format`; clang-tidy checks are in `.clang-tidy`.
- Commits are small and focused, with lowercase descriptive messages. Commit only when asked.
