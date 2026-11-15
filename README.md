# template

C++20 project template for Linux: CMake + Ninja + clang, vcpkg dependencies, clangd for the editor.

## Layout

```
core/        library with the actual code   (src/*.cpp, headers in src/include/)
app/         executable, thin, links core
tests/       Catch2 v3 tests, link core
cmake/       shared CMake helpers (warnings, sanitizers)
vcpkg/       vcpkg as a git submodule
```

Each target globs its own `src/` folder, so new `.cpp`/`.h` files need no CMake edits.

## First time

```sh
git clone --recurse-submodules <url>      # or: git submodule update --init
sudo pacman -S --needed base-devel clang cmake ninja zip unzip curl   # vcpkg's prerequisites
cmake --workflow --preset debug           # configure + build + test
```

The first configure bootstraps vcpkg and builds the dependencies, which takes a few minutes.
Later configures reuse vcpkg's binary cache in `~/.cache/vcpkg`.

## Everyday commands

| What | Command |
|---|---|
| configure | `cmake --preset debug` (or `relwithdebinfo`, `release`) |
| build | `cmake --build --preset debug` |
| test | `ctest --preset debug` |
| all three | `cmake --workflow --preset debug` |
| run | `./out/debug/app` |

In Neovim (cmake-tools.nvim) pick a preset with `Space m p` and build or run with `Space m b` / `Space m r`.

The `debug` preset turns on AddressSanitizer + UndefinedBehaviorSanitizer. Use `relwithdebinfo` for gdb or perf on optimized code.

## clangd

clangd reads `compile_commands.json` from the project root. cmake-tools.nvim links it there from the
selected preset on every configure. Configuring only from the shell? Link it once yourself:
`ln -sf build/debug/compile_commands.json .`. `.clangd` and `.clang-tidy` hold the diagnostics settings,
and `.clang-format` holds the formatting style.

## Starting a new project from this

1. Copy or "Use this template", then `git submodule update --init`.
2. Rename `template` in `project()` (root `CMakeLists.txt`) and `"name"` in `vcpkg.json`.
3. Replace `Greeter` with your own code and the `Core` namespace with your own.
4. Add dependencies to `vcpkg.json`, then `find_package` + `target_link_libraries` in the target's `CMakeLists.txt`.
5. To move to newer library versions, update the submodule (`git -C vcpkg pull`) and set `builtin-baseline` in `vcpkg.json` to its commit (`git -C vcpkg rev-parse HEAD`).
