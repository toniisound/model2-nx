#!/bin/sh
#
# Model 2 NX - prepares a MAME checkout for scripts/import_mame.py
#
# Runs MAME's own build system for a Model 2-only subtarget, but only as far
# as it needs to: generating the gmake projects (one per MAME library, with
# their sources, defines and include paths) and MAME's generated sources
# (version.cpp, the driver list, compiled layouts). Nothing is compiled.
#
# Usage: scripts/prepare_mame.sh <mame checkout>
#
# Needs what MAME's build needs to generate projects: make, python3 and a host
# C++ compiler (GENie is built from source the first time).
#

set -e

if [ $# -ne 1 ] || [ ! -f "$1/makefile" ]; then
	echo "usage: $0 <mame checkout>" >&2
	exit 1
fi

MAME_DIR=$1
MAKE_ARGS="SUBTARGET=model2 SOURCES=src/mame/sega/model2.cpp"
PROJECT=build/projects/sdl/mamemodel2/gmake-linux

cd "$MAME_DIR"

echo "Generating MAME's projects for the Model 2 subtarget..."
make $MAKE_ARGS generate "$PROJECT/Makefile"

echo "Generating the driver list and layouts..."
make -C "$PROJECT" config=release64 precompile

echo
echo "Done. Now run:"
echo "  python3 scripts/import_mame.py --mame $MAME_DIR"
