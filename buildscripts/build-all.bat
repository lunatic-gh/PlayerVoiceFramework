@echo off

:: Setup VS Environment
for /f "usebackq tokens=*" %%i in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do (
    call "%%i\VC\Auxiliary\Build\vcvars64.bat"
    if ERRORLEVEL 1 exit /b
)

:: Build SKSE-Plugin
call "%~dp0\build-plugin.bat"
if ERRORLEVEL 1 exit /b

:: Compile Papyrus Scripts
call "%~dp0\compile-papyrus-scripts.bat"
if ERRORLEVEL 1 exit /b

:: Create Release
call "%~dp0\create-release.bat"
if ERRORLEVEL 1 exit /b

:: Create MO2 Mod
call "%~dp0\create-mod.bat"
if ERRORLEVEL 1 exit /b
