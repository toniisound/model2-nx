/*
 * Model 2 NX - Sega Model 2 for Nintendo Switch
 * Copyright (C) ToniiSound
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 */

/*
 * SwitchPlatform.h
 *
 * Nintendo Switch (libnx) support for MAME's SDL front end. Ported from
 * SuperModel NX (Src/OSD/Switch/SwitchPlatform.*).
 *
 * Everything that needs <switch.h> lives in SwitchPlatform.cpp, which does not
 * include MAME headers, so the two never share a translation unit.
 */

#ifndef MODEL2NX_SWITCHPLATFORM_H
#define MODEL2NX_SWITCHPLATFORM_H

#include <cstddef>

// Root folder on the SD card. MAME runs with this as its current directory,
// so all of its relative paths (roms, cfg, nvram, ...) live here.
#define MODEL2NX_ROOT    "sdmc:/switch/model2nx"

// Call first thing in main(): creates the folders, makes the root the current
// directory and installs the bundled mame.ini from RomFS on first launch.
void SwitchPlatformInit();
void SwitchPlatformShutdown();

// Brings up SDL's joystick and game controller subsystems and gives the
// Switch pads a Nintendo-layout mapping (the button printed "A" is "A").
// Call before MAME starts: the reference taken here keeps the mappings alive
// while MAME starts and stops its own input modules between games.
void SwitchInitGamepads();

// Moves the calling thread to `core` (0-2). The kernel never migrates a
// homebrew thread by itself, and new threads start on the creator's core.
void SwitchPinCurrentThread(int core);

// Core for MAME's work queue thread number `index` (poly renderer, etc.):
// spreads them over cores 1 and 2, away from the main emulation thread.
int SwitchCoreForWorker(int index);

// Number of CPU cores an application can use (3).
int SwitchCoreCount();

// Call from SDL's audio thread (the audio callback), once. libnx starts every
// pthread at priority 0x3B on core 0, below the main thread on that same
// core, so a long frame starves it and the audio device runs dry
// (crackling). Moves it to core 2 at a priority above the emulator threads.
void SwitchSetupAudioThread();

// True once each time − and + start being held together on player 1 / the
// handheld Joy-Cons. Read straight from the HID state, so it works whatever
// the MAME input mapping is. Call once per frame.
bool SwitchExitComboPressed();

// Closing the software from the HOME menu. SwitchPlatformInit() locks the
// exit, so the system asks instead of killing the process: SDL turns the
// request into SDL_QUIT, which calls SwitchRequestQuit(). MAME then stops
// the running machine normally (saving NVRAM and settings) and calls
// SwitchFinishQuitIfRequested(), which lets the system end the process.
void SwitchRequestQuit();
bool SwitchQuitRequested();
void SwitchFinishQuitIfRequested();

#endif // MODEL2NX_SWITCHPLATFORM_H
