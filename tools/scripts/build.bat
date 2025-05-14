@echo off
setlocal EnableDelayedExpansion

:: Failsafe
if not exist "%cd%\CMakeLists.txt" (
    echo Invalid working directory. Please run this script from the project directory via ".\tools\scripts\build.bat"
    exit 1
)

:: Setup Environment
for /f "usebackq tokens=*" %%i in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do (
    call "%%i\VC\Auxiliary\Build\vcvars64.bat"
    if ERRORLEVEL 1 exit /b 1
    goto :build
)
:build

:: Build SKSE Plugin
cmake --build "build\release" --target all -j 14
if ERRORLEVEL 1 exit /b 1

:: Move new stuff to run directory
if exist "run" (
    rmdir /S /Q "run"
)
mkdir "run"
echo D | xcopy /Q /Y ".\build\release\PlayerVoiceFramework.dll" ".\run\SKSE\Plugins"
echo D | xcopy /Q /Y ".\build\release\PlayerVoiceFramework.pdb" ".\run\SKSE\Plugins"
xcopy /Q /Y /E ".\Data\*" ".\run"

:: Compile Scripts
call "tools\scripts\compile-scripts.bat"
if ERRORLEVEL 1 exit /b 1

if "%GITHUB_ACTIONS%" NEQ "true" (
    :: Symlink "run" to a new mo2 mod
    set "MODS_DIR=%MO2_SSE_MODS_PATH%"
    if "!MODS_DIR!"=="" (
        echo Could not find Mods directory, skipping deploy...
        exit /b
    )
    if exist "!MODS_DIR!\PlayerVoiceFramework" (
        rmdir /S /Q "!MODS_DIR!\PlayerVoiceFramework"
    )
    mklink /J "!MODS_DIR!\PlayerVoiceFramework" "run"
    if errorlevel 1 (
        echo Failed to create symlink...
        exit /b 1
    )
)

endlocal