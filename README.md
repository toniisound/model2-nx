# Model 2 NX

**Sega Model 2 arcade emulator for Nintendo Switch** (homebrew `.nro`), by **ToniiSound**.

Built on [MAME](https://www.mamedev.org)'s Model 2 driver, with MAME's SDL front end
and the Switch platform layer from [SuperModel NX](https://github.com/toniisound/supermodel-nx).

> **Status: 0.1, work in progress.** The goal of this version is that the games boot
> on a real Switch. Speed comes next: see [docs/roadmap.md](docs/roadmap.md).

> No ROMs are included: provide your own, and only use copies of games you own.
> Not affiliated with or endorsed by Sega or Nintendo. Sega, Model 2 and the game
> titles are trademarks of their respective owners.

## Features

- Every game in MAME's Model 2 driver (Daytona USA, Virtua Fighter 2, Sega Rally
  Championship, Virtua Cop, Dead or Alive, Sega Touring Car Championship and more).
- MAME's game list and menus, with Joy-Cons or the Pro Controller (Nintendo layout).
- Press − and + together during a game to go back to the game list.

## Installing

1. Copy `model2nx.nro` to `sdmc:/switch/model2nx/` on the SD card.
2. Put your ROMs, as MAME-style zips, in `sdmc:/switch/model2nx/roms/`
   (for example `daytona.zip`, `vf2.zip`). They must match the MAME version in
   `mame/` (see the commit that imported it).
3. Start it from the Homebrew Menu (title override / full RAM mode recommended).

On first launch it creates its folders and installs `mame.ini` and `ui.ini`
in `sdmc:/switch/model2nx/`. Edit them there; they are never overwritten.

## Building (WSL or Linux with devkitPro)

```sh
sudo dkp-pacman -S switch-dev switch-sdl2 switch-mesa switch-zlib
make -j$(nproc)
```

Output: `build/switch/model2nx.nro`.

| Option | Effect |
| --- | --- |
| `NXLINK=1` | Sends console output to the PC (`nxlink -s build/switch/model2nx.nro`) |
| `DEBUG=1` | `-O1 -g` |
| `V=1` | Shows the compiler command lines |
| `PLATFORM=linux` | PC build (`build/linux/model2nx`), for testing and profiling; needs SDL2 |

`make frontend` builds only the front end and platform layer, a quick check of the
Switch-specific code before the full (long) build.

## How it is put together

| Folder | Contents |
| --- | --- |
| `mame/` | The part of MAME the Model 2 driver needs, imported by `scripts/import_mame.py`. Switch changes in MAME's SDL front end are marked `Model 2 NX`. |
| `mame.mk` | Source lists, defines and include paths of each MAME library (generated). |
| `switch/` | Switch platform layer, from SuperModel NX. |
| `osd/nx/` | Switch versions of MAME's OS-dependent files (memory, sockets, missing modules). |
| `config/switch/` | `mame.ini` and `ui.ini` bundled in the `.nro`. |
| `docs/` | [Roadmap](docs/roadmap.md) and [updating MAME](docs/updating-mame.md). |

## Credits

- **Model 2 NX**: Nintendo Switch port by ToniiSound.
- **MAME**: © Nicola Salmoria and the MAME team. <https://www.mamedev.org>
  The Model 2 driver and the i960, MB86233/MB86234, SHARC and MB86235 cores are the
  work of the MAME developers.
- **SuperModel NX** (ToniiSound and Thorhax): the Switch platform layer.
- devkitPro and libnx, SDL2, Mesa.

## License

GNU General Public License v3 or later: see [LICENSE](LICENSE).

The files in `mame/` keep their own licenses: MAME's are almost all BSD-3-Clause
(a few MIT, LGPL-2.1-or-later or public domain), and the third-party libraries in
`mame/3rdparty/` have theirs (zlib, MIT, BSD, GPL-2.0-or-later, ...). See the header
of each file and `mame/COPYING`. All of them allow distribution under
GPL-3.0-or-later as part of this program.
