@echo off

:: Cleanup
if exist "tools\papyrus-include" (
    rmdir "tools\papyrus-include" /S /Q
)
if exist "tools\nl_online" (
    rmdir "tools\nl_online" /S /Q
)

if not exist "tools\pyro" (
    :: Download Pyro
    curl -L -o pyro.zip "https://github.com/fireundubh/pyro/releases/download/1656807840/pyro-master-1656807840.zip"
    mkdir "tools\pyro"
    tar -xf pyro.zip -C "tools\pyro"
    IF EXIST "pyro.zip" (
        del pyro.zip
    )
)

:: Download Dependency Scripts
IF EXIST "tools\papyrus-include" (
    rmdir "tools\papyrus-include" /S /Q
)
git clone "https://github.com/MrOctopus/nl_online" "tools\nl_online"
mkdir "tools\papyrus-include"
xcopy "tools\nl_online\sse\source\*" "tools\papyrus-include\" /I /Y
xcopy "tools\nl_online\skse64\source\*" "tools\papyrus-include\" /I /Y
xcopy "tools\nl_online\skyui\source\*" "tools\papyrus-include\" /I /Y

:: Compile Scripts
"tools\pyro\pyro.exe" --compiler-path "tools\papyrus-compiler\PapyrusCompiler.exe" -i skyrimse.ppj

:: Cleanup
:: Keep "papyrus-include" for local Papyrus Development
if exist "tools\nl_online" (
    rmdir "tools\nl_online" /S /Q
)