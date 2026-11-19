#!/bin/bash
# Builds the two test images from scratch, in order:
#   1. build/ntsc-u/mods/widescreen-anchored/dotr-widescreen-anchored.iso  (the widescreen mod)
#   2. build/ntsc-u/mods/obelisk-test/dotr-obelisk-test.iso                (1 + card 132 reworked)
# The mesh, its texture and the card art are your own files, expected in
# out/research/ntsc-u/models/import/ (ignored by git). Run from anywhere, inside Linux/WSL.
set -e
cd "$(dirname "$0")/../.."
IMPORT=out/research/ntsc-u/models/import
ANCHORED=build/ntsc-u/mods/widescreen-anchored/dotr-widescreen-anchored.iso
TEST=${TEST:-build/ntsc-u/mods/obelisk-test/dotr-obelisk-test.iso}
CARD=132

python3 tools/mod.py widescreen --define WSUI_ANCHOR=1 --variant anchored --make-iso > build/ntsc-u/mods/widescreen-anchored.log
echo "widescreen mod built"
mkdir -p "$(dirname "$TEST")"
cp "$ANCHORED" "$TEST"
python3 tools/model_import.py $CARD "$IMPORT/obelisk the tormentor.obj" --into "$TEST" \
    --up=-z --front=-y --turn --texture "$IMPORT/obelsik the tormentor texture.png"
python3 tools/card_edit.py $CARD --into "$TEST" --attack 4000 --defense 4000 \
    --name "Obelisk the Tormentor" --art "$IMPORT/obelisk-card-art.png"
ls -la --time-style=+%H:%M:%S "$ANCHORED" "$TEST"
