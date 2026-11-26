---
name: cr
description: Code review of newly added/changed code in the Khaos engine, then add Catch2 tests for it where useful. Use when the user types /cr, optionally with a path, commit, or range (e.g. /cr core/src/Renderer.cpp, /cr HEAD~3).
---

# /cr — review new code and add tests

Argument (optional): a path, a commit, or a git range. Default: all uncommitted changes
(staged + unstaged + untracked) — if there are none, review the last commit.

## 1. Collect the change
- `git status --short` and `git diff HEAD` (or `git diff <range>` / `git show <commit>`).
- Untracked files: read them in full.
- Read enough surrounding code (headers, callers) to understand each change. Don't review
  `vcpkg/`, `build/`, `out/`.

## 2. Review
Look for real problems, most severe first:
- **Correctness**: logic errors, off-by-one, null/dangling pointers, lifetime and ownership bugs,
  iterator invalidation, uninitialized members, integer overflow/sign issues, UB.
- **Resources**: leaks, missing cleanup of GL/GLFW/ImGui objects, wrong init/shutdown order,
  rule-of-five violations.
- **Threading**: data races, missing synchronization.
- **API/architecture fit**: does it match the patterns already in the repo? In `core/` the user
  owns the architecture — flag concerns, never rewrite their design.
- **Performance** only where it is clearly significant (per-frame allocations, needless copies in
  hot paths).
- **Build hygiene**: new files added to the right CMakeLists, new deps in `vcpkg.json`,
  `/W4` warnings.

Skip pure style nitpicks unless they hide a bug. Don't invent problems — if the code is fine,
say so.

Output each finding as:
`[severity] file:line — problem → suggested fix` (severity: bug / risk / nit).

## 3. Fix
- `editor/` and `tests/`: apply clear fixes directly, then list what you changed.
- `core/`: **do not edit**. Report findings and proposed fixes; apply only if the user says so.

## 4. Tests (only when needed)
Add Catch2 tests when the change adds or changes testable logic (math, containers, parsing,
state machines, resource/handle bookkeeping, pure functions, public core APIs). Skip it for
pure UI layout, glue code, or code needing a live GL context — say why you skipped.

- Put tests in `tests/src/<Area>Tests.cpp` (e.g. `MathTests.cpp`); extend an existing file if
  one covers the area.
- Add any new file to `add_executable(khaos_tests ...)` in `tests/CMakeLists.txt`.
- Use Catch2 v3 headers: `#include <catch2/catch_test_macros.hpp>`, plus
  `<catch2/matchers/catch_matchers_floating_point.hpp>` for floats (`WithinAbs`/`WithinRel`).
- Tag tests by module: `[core]`, `[editor]`, plus a sub-tag (e.g. `[core][math]`).
- Cover normal cases, edge cases, and the specific bug you found (regression test).
- Test only through public headers. Testing editor logic is fine only if it doesn't need a
  window — otherwise suggest extracting the logic.

## 5. Verify
Build and run the tests (from a plain shell wrap in `cmd /c` with vcvars64, see CLAUDE.md):
```
cmake --build --preset debug && ctest --preset debug
```
Report the result honestly, including failures and their output.

## 6. Summary
End with: findings (by severity), files changed, tests added, build/test result, and anything
the user should decide (especially for `core/`).
