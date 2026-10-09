# Updating MAME

`mame/` holds only the part of MAME the Model 2 driver needs, plus a few
Switch changes in MAME's SDL front end (each marked `Model 2 NX`). To move to a
newer MAME:

1. Clone MAME next to this repository and check out the version you want:

   ```sh
   git clone --depth 1 https://github.com/mamedev/mame ../mame-upstream
   ```

2. Let MAME's build generate its projects for a Model 2-only build
   (nothing is compiled):

   ```sh
   scripts/prepare_mame.sh ../mame-upstream
   ```

3. Save the Switch changes, re-import and put them back:

   ```sh
   git diff -- mame > ../model2nx-mame.patch
   python3 scripts/import_mame.py --mame ../mame-upstream
   git apply --3way ../model2nx-mame.patch
   ```

   `import_mame.py` replaces `mame/` with the files the driver needs and
   rewrites `mame.mk` (the source lists, defines and include paths of each
   MAME library).

4. Build for the PC first (`make PLATFORM=linux`), then for the Switch, and
   commit with the MAME commit in the message, as in
   `Import MAME 4fc3226e (Model 2 driver and its dependencies)`.

Changes in the Switch front end live in:

- `mame/src/osd/sdl/sdlmain.cpp`, `sdlopts.cpp`, `sdlprefix.h`, `video.cpp`
- `mame/src/osd/osdsync.cpp`
- `mame/src/osd/modules/sound/sdl_sound.cpp`

Everything else Switch-specific is outside `mame/`: `switch/`, `osd/nx/`,
`config/switch/` and the `Makefile`.
