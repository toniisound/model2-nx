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
 * SwitchPlatform.cpp
 *
 * Nintendo Switch (libnx) support for MAME's SDL front end. Deliberately does
 * not include MAME headers (see SwitchPlatform.h).
 */

#include "SwitchPlatform.h"

#include <switch.h>
#include <SDL2/SDL.h>

#include <cstdio>
#include <cstring>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

static bool s_nxlink = false;

static bool FileExists(const std::string &path)
{
  struct stat st;
  return stat(path.c_str(), &st) == 0 && S_ISREG(st.st_mode);
}

static void MakeDir(const std::string &path)
{
  mkdir(path.c_str(), 0777);   // fails harmlessly if it already exists
}

static bool CopyFile(const std::string &from, const std::string &to)
{
  FILE *in = fopen(from.c_str(), "rb");
  if (!in)
    return false;
  FILE *out = fopen(to.c_str(), "wb");
  if (!out)
  {
    fclose(in);
    return false;
  }
  char buf[64 * 1024];
  size_t n;
  bool ok = true;
  while ((n = fread(buf, 1, sizeof(buf), in)) > 0)
  {
    if (fwrite(buf, 1, n, out) != n)
    {
      ok = false;
      break;
    }
  }
  fclose(in);
  fclose(out);
  return ok;
}

void SwitchPlatformInit()
{
#ifdef SWITCH_NXLINK
  // Debug builds (make NXLINK=1): send stdout/stderr to `nxlink -s` on the PC.
  if (R_SUCCEEDED(socketInitializeDefault()))
    s_nxlink = nxlinkStdio() >= 0;
#endif

  const std::string root = MODEL2NX_ROOT;
  MakeDir("sdmc:/switch");
  MakeDir(root);
  // MAME's default folder names (see mame.ini).
  for (const char *sub : { "roms", "cfg", "nvram", "ini", "snap", "diff", "sta", "inp", "comments", "share" })
    MakeDir(root + "/" + sub);

  // MAME's paths are relative (rompath "roms", inipath ".;ini", ...).
  chdir(root.c_str());

  // First launch: install the bundled configuration. Existing files are never
  // overwritten, so the player's settings survive updates of the .nro.
  if (R_SUCCEEDED(romfsInit()))
  {
    for (const char *file : { "mame.ini", "ui.ini" })
    {
      std::string dest = root + "/" + file;
      if (!FileExists(dest))
        CopyFile(std::string("romfs:/") + file, dest);
    }
    romfsExit();
  }
  // No CPU boost here: libnx's boost mode drops the GPU clock to 76.8 MHz.
  // Use an overclocking sysmodule (e.g. sys-clk) for more CPU speed.
}

void SwitchPlatformShutdown()
{
  SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK);
#ifdef SWITCH_NXLINK
  if (s_nxlink)
    socketExit();
#endif
  (void)s_nxlink;
}

void SwitchInitGamepads()
{
  if (SDL_InitSubSystem(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) != 0)
    return;

  // SDL's Switch joystick driver reports buttons in this order:
  //   0 A, 1 B, 2 X, 3 Y, 4 L-stick, 5 R-stick, 6 L, 7 R, 8 ZL, 9 ZR,
  //   10 Plus, 11 Minus, 12 D-left, 13 D-up, 14 D-right, 15 D-down
  // and axes 0/1 left stick, 2/3 right stick. SDL ships a mapping for this
  // device with Xbox-style positions (the bottom button, B, acts as "A").
  // Replace it with the Nintendo layout, as in SuperModel NX.
  static const char *k_layout =
    "a:b0,b:b1,x:b2,y:b3,"
    "leftstick:b4,rightstick:b5,leftshoulder:b6,rightshoulder:b7,"
    "lefttrigger:b8,righttrigger:b9,start:b10,back:b11,"
    "dpleft:b12,dpup:b13,dpright:b14,dpdown:b15,"
    "leftx:a0,lefty:a1,rightx:a2,righty:a3";

  for (int i = 0; i < SDL_NumJoysticks(); i++)
  {
    char guid[64];
    SDL_JoystickGetGUIDString(SDL_JoystickGetDeviceGUID(i), guid, sizeof(guid));
    std::string mapping = std::string(guid) + ",Switch Controller," + k_layout;
    SDL_GameControllerAddMapping(mapping.c_str());   // replaces SDL's own entry
  }
}

void SwitchPinCurrentThread(int core)
{
  if (core < 0 || core > 2)
    return;
  svcSetThreadCoreMask(CUR_THREAD_HANDLE, core, 1u << core);
}

int SwitchCoreForWorker(int index)
{
  // Core 0: main thread (emulation, MAME's UI and SDL rendering).
  // Cores 1 and 2: MAME's work queues (the Model 2 polygon renderer).
  return 1 + (index & 1);
}

int SwitchCoreCount()
{
  return 3;
}

void SwitchSetupAudioThread()
{
  // 0x2B: just above the main thread (0x2C). The audio thread spends nearly
  // all its time blocked waiting for the next audio frame, so it cannot
  // starve anything.
  SwitchPinCurrentThread(2);
  svcSetThreadPriority(CUR_THREAD_HANDLE, 0x2B);
}

bool SwitchExitComboPressed()
{
  // Only reads the pad: padConfigureInput() stays as SDL set it up, so the
  // number of players SDL supports is not changed.
  static PadState pad;
  static bool initialized = false;
  static bool wasHeld = true;     // ignore a combo already held at start
  if (!initialized)
  {
    padInitializeDefault(&pad);
    initialized = true;
  }
  padUpdate(&pad);
  const u64 held = padGetButtons(&pad);
  const bool both = (held & HidNpadButton_Plus) && (held & HidNpadButton_Minus);
  const bool pressed = both && !wasHeld;
  wasHeld = both;
  return pressed;
}
