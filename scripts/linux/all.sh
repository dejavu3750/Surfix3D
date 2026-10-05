#!/usr/bin/env bash
# ---------------------------------------------------------------------------
# AUTO-COPIED to SURFix3DPublic by scripts/<os>/publish.*
# SOURCE OF TRUTH: <private-repo>/publish/  -- edit here, then re-run publish.
# Do NOT edit the copy inside SURFix3DPublic/ ; it gets overwritten.
# ---------------------------------------------------------------------------
set -e
DIR="$(dirname "$0")"
"$DIR/setup.sh"
"$DIR/build.sh"
echo "[SURFix3D] All done."
