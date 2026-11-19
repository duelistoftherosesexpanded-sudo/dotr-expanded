#!/bin/bash
# Builds the disc image with the widescreen mod and the DLC (new cards, effects.json, bosses ...),
# then copies it to $TEST (default build/ntsc-u/mods/obelisk-test/dotr-cards.iso). Used by the
# Card Maker's "Build disc image". Unlike mods/obelisk/build.sh it leaves card 132 as the game has
# it. Takes 15-20 minutes. Run from anywhere, inside Linux/WSL.
set -e
cd "$(dirname "$0")/../.."
ANCHORED=build/ntsc-u/mods/widescreen-anchored/dotr-widescreen-anchored.iso
TEST=${TEST:-build/ntsc-u/mods/obelisk-test/dotr-cards.iso}

python3 tools/mod.py widescreen --define WSUI_ANCHOR=1 --variant anchored --make-iso > build/ntsc-u/mods/widescreen-anchored.log
echo "widescreen mod built"
mkdir -p "$(dirname "$TEST")"
cp "$ANCHORED" "$TEST"
ls -la --time-style=+%H:%M:%S "$ANCHORED" "$TEST"
