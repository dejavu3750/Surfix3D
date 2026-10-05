@echo off
REM ---------------------------------------------------------------------------
REM AUTO-COPIED to SURFix3DPublic by scripts\windows\publish.bat
REM SOURCE OF TRUTH: <private-repo>\publish\  -- edit here, then re-run publish.
REM Do NOT edit the copy inside SURFix3DPublic\ ; it gets overwritten.
REM ---------------------------------------------------------------------------
REM Remove build output. Does NOT touch prebuilt\ (the shipped engine).
setlocal
cd /d "%~dp0..\.."
echo [SURFix3D] Removing build\, bin\, lib\ ...
if exist build rmdir /s /q build
if exist bin   rmdir /s /q bin
if exist lib   rmdir /s /q lib
echo [SURFix3D] Clean complete.
exit /b 0
