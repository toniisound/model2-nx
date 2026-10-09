#!/usr/bin/env python3
"""
Imports the part of MAME that Model 2 NX needs into mame/ and writes mame.mk.

MAME itself works out which files the Model 2 driver needs: its build, run
with SOURCES=src/mame/sega/model2.cpp, generates one gmake project per
library. This script reads those projects (source list, defines and include
paths of each library), asks the preprocessor which headers each source uses,
and copies exactly those files.

Usage (see docs/updating-mame.md):
  python3 scripts/import_mame.py --mame <mame checkout> [--out mame]

Before running it, generate the projects and MAME's generated sources in the
MAME checkout (scripts/prepare_mame.sh does both).
"""

import argparse
import concurrent.futures
import subprocess
import os
import re
import shutil
import sys

# MAME libraries (gmake project names) used as they are.
LIBS = [
    'model2', 'mame_model2', 'frontend', 'optional', 'emu', 'dasm', 'formats',
    'utils', 'expat', 'softfloat3', 'wdlfft', 'ymfm', 'residfp', 'jpeg', '7z',
    'asmjit', 'lua', 'lualibs', 'zlib', 'zstd', 'flac', 'utf8proc', 'sqlite3',
]

# The OSD layer is ours to choose: MAME's SDL front end, without the
# renderers, debuggers and audio/network back ends a console has no use for.
# Each module file compiles to a "not supported" stub where its back end is
# absent; osd/nx/nxstubs.cpp covers the ones left out here.
OSD_SOURCES = [
    # core OSD library (ocore)
    'src/osd/osdcore.cpp',
    'src/osd/osdsync.cpp',
    'src/osd/strconv.cpp',
    'src/osd/modules/osdmodule.cpp',
    'src/osd/modules/lib/osdlib_unix.cpp',
    'src/osd/modules/file/posixdir.cpp',
    'src/osd/modules/file/posixfile.cpp',
    'src/osd/modules/file/posixptty.cpp',
    'src/osd/modules/file/posixsocket.cpp',
    # SDL front end
    'src/osd/interface/audio.cpp',
    'src/osd/interface/inputseq.cpp',
    'src/osd/interface/nethandler.cpp',
    'src/osd/modules/debugger/none.cpp',
    'src/osd/modules/diagnostics/none.cpp',
    'src/osd/modules/font/font_none.cpp',
    'src/osd/modules/font/font_sdl.cpp',
    'src/osd/modules/font/font_osx.cpp',
    'src/osd/modules/font/font_windows.cpp',
    'src/osd/modules/font/font_dwrite.cpp',
    'src/osd/modules/input/assignmenthelper.cpp',
    'src/osd/modules/input/input_common.cpp',
    'src/osd/modules/input/input_none.cpp',
    'src/osd/modules/input/input_sdl.cpp',
    'src/osd/modules/input/input_sdlcommon.cpp',
    'src/osd/modules/input/input_mac.cpp',
    'src/osd/modules/input/input_dinput.cpp',
    'src/osd/modules/input/input_rawinput.cpp',
    'src/osd/modules/input/input_win32.cpp',
    'src/osd/modules/input/input_winhybrid.cpp',
    'src/osd/modules/input/input_xinput.cpp',
    'src/osd/modules/input/input_x11.cpp',
    'src/osd/modules/lib/osdobj_common.cpp',
    'src/osd/modules/midi/none.cpp',
    'src/osd/modules/monitor/monitor_common.cpp',
    'src/osd/modules/monitor/monitor_sdl.cpp',
    'src/osd/modules/monitor/monitor_win32.cpp',
    'src/osd/modules/monitor/monitor_dxgi.cpp',
    'src/osd/modules/monitor/monitor_mac.cpp',
    'src/osd/modules/netdev/netdev_common.cpp',
    'src/osd/modules/netdev/none.cpp',
    'src/osd/modules/osdwindow.cpp',
    'src/osd/modules/output/console.cpp',
    'src/osd/modules/output/none.cpp',
    'src/osd/modules/output/win32_output.cpp',
    'src/osd/modules/render/drawnone.cpp',
    'src/osd/modules/render/drawsdl.cpp',
    'src/osd/modules/render/draw13.cpp',
    'src/osd/modules/render/drawogl.cpp',
    'src/osd/modules/render/drawgdi.cpp',
    'src/osd/modules/sound/none.cpp',
    'src/osd/modules/sound/sound_module.cpp',
    'src/osd/modules/sound/sdl_sound.cpp',
    'src/osd/modules/sound/coreaudio_sound.cpp',
    'src/osd/modules/sound/js_sound.cpp',
    'src/osd/modules/sound/wasapi_sound.cpp',
    'src/osd/modules/sound/xaudio2_sound.cpp',
    'src/osd/sdl/osdsdl.cpp',
    'src/osd/sdl/sdlmain.cpp',
    'src/osd/sdl/sdlopts.cpp',
    'src/osd/sdl/video.cpp',
    'src/osd/sdl/window.cpp',
    'src/osd/watchdog.cpp',
]

# Whole folders copied for the OSD layer (its sources need SDL headers, so
# their dependencies are not worked out like the libraries' ones).
OSD_DIRS = [
    'src/osd/interface', 'src/osd/sdl', 'src/osd/modules/lib',
    'src/osd/modules/file', 'src/osd/modules/input', 'src/osd/modules/font',
    'src/osd/modules/sound', 'src/osd/modules/render', 'src/osd/modules/monitor',
    'src/osd/modules/netdev', 'src/osd/modules/output', 'src/osd/modules/midi',
    'src/osd/modules/debugger', 'src/osd/modules/diagnostics',
]
OSD_DIR_SKIP = ('src/osd/modules/render/bgfx', 'src/osd/modules/render/d3d',
                'src/osd/modules/debugger/qt', 'src/osd/modules/debugger/win',
                'src/osd/modules/debugger/osx')

# Files of src/osd itself (not in a subfolder) and modules/ top level.
OSD_TOP_FILES = True

# Extra defines for every library (the OSD and platform ones come from the
# Makefile, per platform).
COMMON_DEFINES = ['-DNDEBUG', '-DCRLF=2', '-DLSB_FIRST', '-DFLAC__NO_DLL',
                  '-DPUGIXML_HEADER_ONLY', '-DASMJIT_STATIC']


def parse_project(path):
    text = open(path).read()
    m = re.search(r'ifeq \(\$\(config\),release64\)(.*?)\nendif', text, re.S)
    block = m.group(1) if m else text
    defines = re.search(r'^  DEFINES\s+\+=(.*)$', block, re.M)
    includes = re.search(r'^  INCLUDES\s+\+=(.*)$', block, re.M)
    objs = list(dict.fromkeys(re.findall(r'\$\(OBJDIR\)/(\S+?)\.o\b', block)))
    defs = defines.group(1).split() if defines else []
    incs = re.findall(r'-I"([^"]+)"', includes.group(1)) if includes else []
    return objs, defs, incs


def rel_to_root(p, gmake_dir, root):
    a = os.path.normpath(os.path.join(gmake_dir, p))
    return os.path.relpath(a, root)


def find_source(root, obj):
    for ext in ('.cpp', '.c', '.cc', '.cxx'):
        for base in ('', 'build'):
            p = os.path.join(base, obj + ext)
            if os.path.exists(os.path.join(root, p)):
                return p
    return None


def header_deps(root, src, defs, incs):
    """Headers `src` includes, from the preprocessor (g++ -MM)."""
    is_c = src.endswith('.c')
    cmd = ['gcc' if is_c else 'g++', '-std=gnu99' if is_c else '-std=c++20', '-MM']
    cmd += COMMON_DEFINES + defs + ['-DOSD_SDL', '-DSDLMAME_UNIX', '-DUSE_OPENGL=0']
    cmd += ['-I' + os.path.join(root, i) for i in incs]
    cmd.append(os.path.join(root, src))
    r = subprocess.run(cmd, capture_output=True, text=True)
    if r.returncode != 0:
        raise RuntimeError('%s\n%s' % (' '.join(cmd), r.stderr[-2000:]))
    deps = r.stdout.replace('\\\n', ' ').split(':', 1)[1].split()
    return [os.path.relpath(os.path.abspath(d), root) for d in deps]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--mame', required=True)
    ap.add_argument('--out', default='mame')
    ap.add_argument('--mk', default='mame.mk')
    ap.add_argument('--gmake', default='build/projects/sdl/mamemodel2/gmake-linux')
    a = ap.parse_args()

    root = os.path.abspath(a.mame)
    gdir = os.path.join(root, a.gmake)
    files = set()
    libs = {}
    jobs = []

    for lib in LIBS:
        objs, defs, incs = parse_project(os.path.join(gdir, lib + '.make'))
        incs = [rel_to_root(i, gdir, root) for i in incs]
        srcs = []
        for o in objs:
            src = find_source(root, o)
            if not src:
                sys.exit('%s: no source for %s' % (lib, o))
            srcs.append(src)
        jobs.extend((lib, src, defs, incs) for src in srcs)
        defs = [d for d in defs if d not in COMMON_DEFINES]
        libs[lib] = (srcs, defs, incs)

    with concurrent.futures.ThreadPoolExecutor(os.cpu_count() or 2) as ex:
        for deps in ex.map(lambda j: header_deps(root, j[1], j[2], j[3]), jobs):
            files.update(d for d in deps if not d.startswith('..'))

    # OSD layer
    for s in OSD_SOURCES:
        files.add(s)
    for d in OSD_DIRS:
        for dirpath, _, names in os.walk(os.path.join(root, d)):
            relp = os.path.relpath(dirpath, root)
            if relp.startswith(OSD_DIR_SKIP):
                continue
            for n in names:
                files.add(os.path.join(relp, n))
    for n in os.listdir(os.path.join(root, 'src/osd')):
        if os.path.isfile(os.path.join(root, 'src/osd', n)):
            files.add(os.path.join('src/osd', n))
    for n in os.listdir(os.path.join(root, 'src/osd/modules')):
        if os.path.isfile(os.path.join(root, 'src/osd/modules', n)):
            files.add(os.path.join('src/osd/modules', n))
    # License texts
    for n in ('COPYING', 'docs/LICENSE', 'README.md'):
        if os.path.exists(os.path.join(root, n)):
            files.add(n)

    out = os.path.abspath(a.out)
    if os.path.exists(out):
        shutil.rmtree(out)
    for f in sorted(files):
        dst = os.path.join(out, f)
        os.makedirs(os.path.dirname(dst), exist_ok=True)
        shutil.copy2(os.path.join(root, f), dst)

    # mame.mk
    def var(name):
        return 'MAME_' + name.upper()

    with open(a.mk, 'w') as mk:
        mk.write('# Generated by scripts/import_mame.py from MAME\'s own build projects\n')
        mk.write('# (SOURCES=src/mame/sega/model2.cpp). Do not edit by hand.\n\n')
        mk.write('MAME_LIBS := %s\n\n' % ' '.join(LIBS))
        mk.write('MAME_COMMON_DEFINES := %s\n\n' % ' '.join(COMMON_DEFINES))
        for lib in LIBS:
            srcs, defs, incs = libs[lib]
            mk.write('%s_DEFINES := %s\n' % (var(lib), ' '.join(defs)))
            mk.write('%s_INCLUDES := %s\n' % (var(lib), ' '.join('-I$(MAME)/' + i for i in incs)))
            mk.write('%s_SOURCES := \\\n' % var(lib))
            for s in srcs:
                mk.write('\t%s \\\n' % s)
            mk.write('\n')
        mk.write('MAME_OSD_SOURCES := \\\n')
        for s in OSD_SOURCES:
            mk.write('\t%s \\\n' % s)
        mk.write('\n')
        # the OSD libraries use the emu include paths plus their own
        mk.write('MAME_OSD_INCLUDES := -I$(MAME)/src/osd -I$(MAME)/src/osd/sdl -I$(MAME)/src/osd/modules/file '
                 '-I$(MAME)/src/osd/modules/render -I$(MAME)/src/emu -I$(MAME)/src/lib -I$(MAME)/src/lib/util '
                 '-I$(MAME)/src/frontend/mame -I$(MAME)/3rdparty -I$(MAME)/3rdparty/asio/include '
                 '-I$(MAME)/3rdparty/rapidjson/include -I$(MAME)/3rdparty/sol2 -I$(MAME)/3rdparty/lua/src '
                 '-I$(MAME)/3rdparty/expat/lib -I$(MAME)/3rdparty/zlib -I$(MAME)/3rdparty/pugixml/src '
                 '-I$(MAME)/build/generated/emu -I$(MAME)/build/generated/emu/layout\n')

    total = sum(os.path.getsize(os.path.join(out, f)) for f in files)
    print('%d files, %.1f MB -> %s; %s written' % (len(files), total / 1e6, out, a.mk))


if __name__ == '__main__':
    main()
