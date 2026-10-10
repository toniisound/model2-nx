# Model2 NX roadmap

## 1.0 — boots and plays (done)

- MAME's Model 2 driver with MAME's SDL front end and game list.
- Switch platform layer from SuperModel NX: SD card folders, bundled ini files,
  Nintendo-layout pads, − and + to leave a game, thread placement, the audio thread
  fix, NVRAM saved when closed from HOME.
- Speed work measured on hardware with the profile in `model2nx.log`
  (Daytona USA, stock clocks, about 14 → 20 emulated frames per second):
  - sound UART clocks at one pulse per bit instead of 16 (scheduler timers);
  - no comm board simulation (it tried to open its sockets every frame);
  - geometrizer on its own thread, one frame behind;
  - `video accel` in the games (`ini/source/model2.ini`), `video soft` in MAME's
    menus (accel draws every UI character as its own texture).

## Next — recompilers

The emulated CPUs take about 70 % of the time, nearly all of it in interpreters.

1. **MAME's ARM64 back end on libnx JIT memory.** MAME already has an ARM64
   recompiler back end (`src/devices/cpu/drcbearm64.cpp`); Model 2B (SHARC) and 2C
   (MB86235) use it. libnx's code memory is mapped twice (written through one
   address, run from another), so MAME's `drc_cache` has to write through the
   writable view. Until then those CPUs run through the slow C back end.
2. **TGP (MB86234) recompiler** for Model 2/2A, through the same back end.
3. **i960 recompiler**: the main CPU of every Model 2 board, and the largest piece.

## Later

- Polygon rendering on the GPU instead of `model2_renderer` (OpenGL through Mesa,
  as SuperModel NX does for Model 3).
- A Switch renderer for MAME's UI that batches its quads, so the menus can use the
  GPU too.
- SuperModel NX-style front end: ImGui game list with covers, settings on +, credits.
