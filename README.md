# Model2-NX

A Nintendo Switch homebrew port (`.nro`) of
[Model2EmuEx](https://github.com/octavioCuatrochio/Model2EmuEx) by octavioCuatrochio:
a portable C port of ElSemi's Sega Model 2 Emulator 1.1a, rebuilt from a Ghidra
decompilation. This port is based on its `coop` branch (separate emulation,
sound and drawing threads, the launcher, and two-player linked Daytona USA).

> **Experimental / in development.** Built and linked with devkitA64, not yet
> verified on every game or on every console setup.

## Games

| Game | Set | Status upstream |
|------|-----|-----------------|
| Daytona USA | `daytona` | playable: races, sound, controls, gears, linked 2-player |
| Virtua Fighter 2 | `vf2` | boots and runs attract mode with 2D, 3D and sound |
| Sega Rally Championship | `srallyc` | boots and runs attract mode with 2D, 3D and sound |

Other Model 2 / 2A games in the list may boot but haven't been tried. Model 2B
(SHARC) games are not emulated.

## Installation

1. Copy `Model2-NX.nro` to `sdmc:/switch/Model2-NX/` on the SD card.
2. Put the MAME-style ROM zips (`daytona.zip`, `vf2.zip`, `srallyc.zip`, and
   `model2.zip` if your sets need it) in `sdmc:/switch/Model2-NX/roms/`.
3. Launch it from the Homebrew Menu. Use title takeover (hold R while opening a
   game) rather than the album applet, so the emulator gets all the memory.

The folders are created on the first run if they're missing.

| Path | Contents |
|------|----------|
| `sdmc:/switch/Model2-NX/roms/` | ROM zips |
| `sdmc:/switch/Model2-NX/NVDATA/` | save data (`<game>.DAT`, same format as the PC emulator's) |
| `sdmc:/switch/Model2-NX/m2emu.ini` | settings, written by the launcher |
| `sdmc:/switch/Model2-NX/gamecontrollerdb.txt` | optional extra pad mappings |

## Controls

### Launcher

| Button | Action |
|--------|--------|
| D-pad / left stick | move |
| A | choose |
| B | back |
| + | exit to the Homebrew Menu |
| Touch screen | works as a mouse |

The launcher has two tabs: **Games** (pick a game; for Daytona USA tick
"Two players, linked" for split-screen play) and **Configuration** (widescreen,
render scale, texture filtering, saturation, gamma, shifter type and more).

### In game

| Button | Daytona USA / Sega Rally | Virtua Fighter 2 |
|--------|--------------------------|------------------|
| Left stick | steer | move |
| D-pad | — | move |
| ZR / ZL | accelerate / brake | — |
| R / L | gear up / down | — |
| A B X Y | view buttons (Sega Rally: Y = view) | buttons 1–4 (guard, punch, kick) |
| + | start | start |
| − | coin | coin |
| L3 / R3 (stick click) | service / test | service / test |
| **− and + together** | **back to the launcher** | **back to the launcher** |

Linked play (Daytona USA) needs two controllers: the first one drives player 1
(left or top half), the second one player 2.

## What the Switch port changes

The emulation core (i960, TGP, 68000 sound board, SCSP, MultiPCM, YM3438, 3D
geometrizer and renderer) is the upstream code unchanged. Only the frontend
differs, under `#ifdef __SWITCH__`:

- Paths on the SD card instead of `$HOME` / `XDG` folders.
- Fixed 1280×720 fullscreen window (scaled to 1080p when docked).
- OpenGL 2.1 compatibility context through Mesa (nouveau) and EGL.
- Board thread on core 1, sound thread on core 2, drawing on core 0.
- CPU boost (1785 MHz) while a game runs, back to normal in the launcher; no
  auto-sleep during play.
- Controller-only shortcuts: − + + to leave a game, L3 / R3 for service / test,
  + to quit from the launcher.

## Building

With devkitPro installed (`switch-dev`, `switch-sdl2`, `switch-zlib`, `switch-mesa`):

    make -f Makefile.switch

Or with Docker, no local install needed:

    docker run --rm -v "$PWD":/src -w /src devkitpro/devkita64 make -f Makefile.switch

This writes `Model2-NX.nro`. Every push is also built by GitHub Actions
(`.github/workflows/switch.yml`); the `.nro` is on the run's page under Artifacts.

The desktop build (Linux / Windows) still works with the original `Makefile`.
`docs/PORT_GUIDE.md` is the upstream guide: every option, the threads, the
headless `m2run` tool and how to check that a change leaves the output alone.

## Credits

- **ElSemi**: the original Sega Model 2 Emulator.
- **octavioCuatrochio**: [Model2EmuEx](https://github.com/octavioCuatrochio/Model2EmuEx),
  the decompilation and portable C port this is built on.
- **Toniisound**: Nintendo Switch port.
- Third-party code: Musashi 68000 core (MIT), ymfm (BSD-3), Dear ImGui (MIT),
  Roboto font (Apache 2.0), SDL2, devkitPro / libnx.

Sega, Model 2, Daytona USA, Virtua Fighter and Sega Rally are trademarks of
Sega. No ROMs are included or will be provided.

## Donations

This emulator is completely free and open-source. Optional donations to support the development are always welcome:

[![Ko-fi](https://img.shields.io/badge/Ko--fi-Support-FF5E5B?style=for-the-badge&logo=ko-fi&logoColor=white)](https://ko-fi.com/toniisound)
[![PayPal](https://img.shields.io/badge/PayPal-Donate-00457C?style=for-the-badge&logo=paypal&logoColor=white)](https://paypal.me/toniisound)
