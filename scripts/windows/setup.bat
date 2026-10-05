@echo off
REM ---------------------------------------------------------------------------
REM AUTO-COPIED to SURFix3DPublic by scripts\windows\publish.bat
REM SOURCE OF TRUTH: <private-repo>\publish\  -- edit here, then re-run publish.
REM Do NOT edit the copy inside SURFix3DPublic\ ; it gets overwritten.
REM ---------------------------------------------------------------------------
REM Public repo setup: init submodules + generate the Visual Studio solution.
setlocal
cd /d "%~dp0..\.."
if "%GENERATOR%"=="" set "GENERATOR=Visual Studio 17 2022"
if "%ARCH%"=="" set "ARCH=x64"
echo [SURFix3D] Initializing submodules (glm / glfw / imgui)...
echo [SURFix3D] Generating "%GENERATOR%" (%ARCH%) -^> build\  (engine from prebuilt\)
cmake -S . -B build -G "%GENERATOR%" -A %ARCH% || goto :error
echo [SURFix3D] Setup complete. Open build\SURFix3D.sln in Visual Studio.
exit /b 0
:error
echo [SURFix3D] Setup FAILED (exit code %errorlevel%).
exit /b %errorlevel%
