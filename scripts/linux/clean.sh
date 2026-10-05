#!/usr/bin/env bash
# ---------------------------------------------------------------------------
# AUTO-COPIED to SURFix3DPublic by scripts/<os>/publish.*
# SOURCE OF TRUTH: <private-repo>/publish/  -- edit here, then re-run publish.
# Do NOT edit the copy inside SURFix3DPublic/ ; it gets overwritten.
# ---------------------------------------------------------------------------
# Remove build output. Does NOT touch prebuilt/ (the shipped engine).
set -e
cd "$(dirname "$0")/../.."
echo "[SURFix3D] Removing build/, bin/, lib/ ..."
rm -rf build bin lib
echo "[SURFix3D] Clean complete."
