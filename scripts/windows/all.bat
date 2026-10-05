@echo off
REM ---------------------------------------------------------------------------
REM AUTO-COPIED to SURFix3DPublic by scripts\windows\publish.bat
REM SOURCE OF TRUTH: <private-repo>\publish\  -- edit here, then re-run publish.
REM Do NOT edit the copy inside SURFix3DPublic\ ; it gets overwritten.
REM ---------------------------------------------------------------------------
REM Set NOPAUSE=1 to skip the final pause (CI / scripted runs).
setlocal
call "%~dp0setup.bat" || goto :error
call "%~dp0build.bat" || goto :error
echo [SURFix3D] All done.
if not defined NOPAUSE pause
exit /b 0
:error
set "ERR=%errorlevel%"
echo [SURFix3D] FAILED (exit code %ERR%).
if not defined NOPAUSE pause
exit /b %ERR%