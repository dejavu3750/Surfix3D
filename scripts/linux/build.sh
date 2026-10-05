#!/usr/bin/env bash
# ---------------------------------------------------------------------------
# AUTO-COPIED to SURFix3DPublic by scripts/<os>/publish.*
# SOURCE OF TRUTH: <private-repo>/publish/  -- edit here, then re-run publish.
# Do NOT edit the copy inside SURFix3DPublic/ ; it gets overwritten.
# ---------------------------------------------------------------------------
set -e
cd "$(dirname "$0")/../.."
if [ ! -d build ]; then echo "[SURFix3D] Run scripts/linux/setup.sh first." >&2; exit 1; fi
cmake --build build -j "$(nproc 2>/dev/null || echo 4)"
echo "[SURFix3D] Build complete. Binaries in bin/"
