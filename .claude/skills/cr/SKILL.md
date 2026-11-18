---
name: cr
description: Code review + test for a feature in khaos-engine. Builds the project in a background agent while reviewing the target code for bugs, then writes Catch2 tests that cover the behaviour and the bugs found. Use when the user runs /cr <feature or files>, or asks to "review and test" a part of the engine.
argument-hint: <feature or file paths, e.g. "model/mesh" or core/src/Rendering/OpenGL/GLModel.cpp>
---

# /cr: review, build and test

Target: `$ARGUMENTS`. If empty, use the uncommitted changes (`git status`, `git diff`) as the target.

Follow these steps in order. Steps 1 and 2 run at the same time.

## 1. Start the build in the background (first tool call)

Spawn one background agent (`Agent` tool, `subagent_type: general-purpose`, `run_in_background: true`) with this task, report-only:

> In <repo root>, do NOT edit, create or delete files. Run `cmake --preset debug`, then
> `cmake --build --preset debug --clean-first 2>&1`. If it builds, run
> `ctest --preset debug --output-on-failure 2>&1`. Report: build success/failure; every
> compiler error and warning in project files (ignore vcpkg/ and build/), deduplicated and
> verbatim with file:line:col and the -W flag, calling out the target files <list them>;
> linker errors verbatim; test pass/fail counts and the full output of failing tests,
> including ASan/UBSan/LSan reports. Facts only, no fixes.

Don't edit any source file while that agent is running: a half-written file would break its build. Writing outside the source tree (e.g. this skill) is fine.

## 2. Review the code while it builds

Find the target files (headers in `core/src/include/khaos/...`, sources in `core/src/...`) and read them fully, plus whatever they depend on (`GLHandle.h`, the buffer/VAO classes, `KLog`, third-party headers in `build/debug/vcpkg_installed/x64-linux/include/` when the API contract matters).

Look for, in roughly this order:
- **Lifetime/ownership:** dangling pointers to data owned by a local (e.g. `aiScene` owned by a local `Assimp::Importer`), use-after-free, GL objects deleted while still referenced, copy/move of RAII GL wrappers.
- **Out-of-bounds/UB:** assumptions about third-party data that aren't guaranteed (face index counts, optional arrays like normals/UVs/colours), uninitialised variables, integer overflow, signed/unsigned mixing.
- **Dead or unfinished code:** functions never called, members never read, results discarded, error paths that return a half-built object nobody can detect.
- **Wrong API use:** flags that don't do what the comment says, wrong GL target/enum, missing bind, state leaks.
- **Error handling:** silent failures, log messages without the path/reason, no way for the caller to tell success from failure.
- **Interface:** third-party types leaking into public headers, missing accessors needed to use/test the class, non-`explicit` single-arg constructors.
- **Project conventions** (CLAUDE.md): `mVariable` members, PascalCase, `project_setup_target`, includes that clangd/clang-tidy would flag.

Verify every finding against the code before reporting it. For anything about third-party behaviour, check the header/docs rather than relying on memory.

## 3. Merge in the build results

When the build agent's notification arrives, add its errors/warnings for the target files to the findings. If the build fails in the target files, that's the top finding.

## 4. Write Catch2 tests

- Put them in `tests/src/<area>_test.cpp` (snake_case, like `gl_texture_test.cpp`). Never in `core` or `editor`. The tests glob picks up new files; no CMake edits.
- GL code: create `const KhaosTests::GLContext context;` first in each test (from `GLTestUtils.h`; it SKIPs without a display/GL 4.6). Check `glGetError() == GL_NO_ERROR` at the end.
- Make fixtures in the test itself: write small files to `std::filesystem::temp_directory_path()` with a `khaos_` prefix (see `WritePng`), or build third-party structs in memory. Don't add binary assets to the repo for tests.
- Tag tests `[area][subarea]` like the existing ones, and give them names that say what should happen.
- Cover the normal path, the edge cases, and **one test per confirmed bug that asserts the correct behaviour**, so it fails now and passes once the bug is fixed. Note in a comment which finding it covers.
- If the class has no way to observe its result, add the smallest read-only accessor needed (e.g. a `const&` getter) and say so in the report. Don't change behaviour or fix bugs unless the user asked for that.
- A test that triggers a sanitizer abort still only fails its own ctest entry (catch_discover_tests runs one process per test case), so a memory-safety bug can have a test. Say in the report which ones abort.

Then build and run only the new tests: `cmake --build --preset debug && ctest --preset debug -R "<tag or name>" --output-on-failure`. Run `clang-format -i` on new and edited files.

## 5. Report

Keep it short:
1. **Build:** result, plus warnings in the target files.
2. **Findings:** ranked most severe first, each with `file:line`, what goes wrong and when, and the fix in one line.
3. **Tests:** file, what each test covers, which pass and which fail (and which finding each failure points to).
4. Ask whether to fix the findings. Don't fix them unprompted.
