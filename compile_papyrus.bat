@echo off

set argc=0
for %%x in (%*) do Set /A argc+=1

if %argc% NEQ 2 (
    echo Illegal arguments. Syntax:
    echo ^> compile_papyrus.bat "path/to/input/dir" "path/to/output/dir"
    exit /B 1
)

if not exist "%~dp0\build\release\papyrus_include" (
    git clone "https://github.com/MrOctopus/nl_online" "%~dp0\build\release\nl_online"
    xcopy "%~dp0\build\release\nl_online\sse\source\*" "%~dp0\build\release\papyrus_include\" /I /Y
    xcopy "%~dp0\build\release\nl_online\skse64\source\*" "%~dp0\build\release\papyrus_include\" /I /Y
    xcopy "%~dp0\build\release\nl_online\skyui\source\*" "%~dp0\build\release\papyrus_include\" /I /Y
)

"%~dp0\papyrus-compiler.exe" compile -i %1 -o %2 -h "%~dp0\build\release\papyrus_include" -nocache

rmdir "%~dp0\build\release\nl_online" /S /Q
rmdir "%~dp0\build\release\papyrus_include" /S /Q