@echo off
REM ---------------------------------------------------------------------------
REM AUTO-COPIED to SURFix3DPublic by scripts\windows\publish.bat
REM SOURCE OF TRUTH: <private-repo>\publish\  -- edit here, then re-run publish.
REM Do NOT edit the copy inside SURFix3DPublic\ ; it gets overwritten.
REM ---------------------------------------------------------------------------
setlocal
cd /d "%~dp0..\.."
if "%CONFIG%"=="" set "CONFIG=Release"
if not exist build ( echo [SURFix3D] Run scripts\windows\setup.bat first. & exit /b 1 )
echo [SURFix3D] Building (%CONFIG%)...
cmake --build build --config %CONFIG% || goto :error
echo [SURFix3D] Build complete. Binaries in bin\%CONFIG%\
exit /b 0
:error
echo [SURFix3D] Build FAILED (exit code %errorlevel%).
exit /b %errorlevel%
