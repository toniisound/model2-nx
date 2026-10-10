# Model 2 NX roadmap

## 0.1 — first boot (current)

- MAME's Model 2 driver with MAME's own SDL front end and game list.
- Switch platform layer from SuperModel NX: SD card folders, bundled
  `mame.ini`/`ui.ini`, Nintendo-layout pad mappings, − and + to leave a game,
  thread pinning, the audio thread fix.
- MAME's recompilers (SHARC on Model 2B, MB86235 on Model 2C) run through the
  **C back end** (`NATIVE_DRC=drcbe_c`, `drc_use_c 1`): correct but slow.
- Polygons drawn in software by MAME's `model2_renderer` on MAME's work queues
  (cores 1 and 2).

Goal: every Model 2 variant boots on hardware. Measure speed per game
(Virtua Fighter 2 and Daytona USA on 2A, Sega Rally on 2B, Sega Touring Car on 2C).

## 0.2 — ARM64 recompiler on libnx's JIT

MAME already has an ARM64 back end (`src/devices/cpu/drcbearm64.cpp`, asmjit).
What is missing is executable memory:

- `osd/nx/osdlib_switch.cpp` gives MAME plain heap memory that can never be
  executable, so the DRC cache stays in W^X mode and nothing runs from it.
- libnx's JIT (`switch/SwitchJit.c`, from SuperModel NX) gives **two views** of
  the same memory: one writable, one executable, at different addresses
  (CodeMemory backend, the usual one under Atmosphère).
- `drc_cache` (`src/devices/cpu/drccache.cpp`) assumes one address that switches
  between writable and executable.

Plan: teach `drc_cache` about a separate write view (write through the RW view,
hand out RX addresses, keep the near cache data reachable from both), then
build with `NATIVE_DRC` unset so `drcuml.cpp` picks `drcbe_arm64`, and set
`drc_use_c 0`.

## 0.3 — speed

Profile first (a `SwitchProfile` like SuperModel NX's: time per CPU device,
renderer and swap). Likely candidates:

- i960 (main CPU) and MB86234 (TGP, Model 2/2A) are interpreters: a recompiler
  for one of them, or cheaper scheduling between them.
- A Switch renderer for MAME's UI and screens that batches its quads (one
  texture atlas): MAME's "accel" SDL renderer makes one draw call per UI
  character and ran the menus at under 2 fps, so 0.1 uses "soft".
- Polygon rendering on the GPU instead of `model2_renderer` (OpenGL through
  Mesa, as SuperModel NX does for Model 3).
- MAME's scheduler quantum and the `autoframeskip` behaviour.

## 1.0 — SuperModel NX-style front end

- ImGui game list with covers instead of MAME's menu.
- Settings on the + button, logo, credits tab.
- Resolution selector and FPS counter.
