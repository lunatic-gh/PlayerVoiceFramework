@echo off

:: Cleanup
IF EXIST "papyrus-include" (
    rmdir "papyrus-include" /S /Q
)

:: Download Pyro if not in project
if not exist "tools\pyro" (
    curl -L -o pyro.zip "https://github.com/fireundubh/pyro/releases/download/1656807840/pyro-master-1656807840.zip"
    mkdir "tools\pyro"
    tar -xf pyro.zip -C "tools\pyro"
    IF EXIST "pyro.zip" (
        del pyro.zip
    )
)
:: Download Dependency Scripts
git clone "https://github.com/MrOctopus/nl_online" "tools\nl_online"
mkdir "tools\papyrus-include"
xcopy "tools\nl_online\sse\source\*" "tools\papyrus-include\" /E /I /Y
xcopy "tools\nl_online\skse64\source\*" "tools\papyrus-include\" /E /I /Y

:: Compile Scripts
"tools\pyro\pyro.exe" --compiler-path "tools\papyrus-compiler\PapyrusCompiler.exe" -i skyrimse.ppj

:: Cleanup
IF EXIST "tools\nl_online" (
    rmdir "tools\nl_online" /S /Q
)
IF EXIST "tools\papyrus-include" (
    rmdir "tools\papyrus-include" /S /Q
)