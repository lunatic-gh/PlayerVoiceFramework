@echo off

:: Setup VS Environment
for /f "usebackq tokens=*" %%i in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do (
    call "%%i\VC\Auxiliary\Build\vcvars64.bat"
)

:: Build SKSE-Plugin
call "%~dp0\build-plugin.bat"

:: Compile Papyrus Scripts
call "%~dp0\compile-papyrus-scripts.bat"

:: Create Release
call "%~dp0\create-release.bat"