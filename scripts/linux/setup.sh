#!/usr/bin/env bash
# ---------------------------------------------------------------------------
# AUTO-COPIED to SURFix3DPublic by scripts/<os>/publish.*
# SOURCE OF TRUTH: <private-repo>/publish/  -- edit here, then re-run publish.
# Do NOT edit the copy inside SURFix3DPublic/ ; it gets overwritten.
# ---------------------------------------------------------------------------
# Public repo setup: init submodules + configure against the pre-built engine.
set -e
cd "$(dirname "$0")/../.."
BUILD_TYPE="${BUILD_TYPE:-Release}"
echo "[SURFix3D] Initializing submodules (glm / glfw / imgui)..."
git submodule update --init --recursive
echo "[SURFix3D] Configuring -> build/  (engine from prebuilt/)"
if command -v ninja >/dev/null 2>&1; then
    cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
else
    cmake -S . -B build -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
fi
echo "[SURFix3D] Setup complete."
