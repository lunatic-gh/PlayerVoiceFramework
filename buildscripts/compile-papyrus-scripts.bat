@echo off

:: Cleanup
IF EXIST "papyrus-include" (
    rmdir "papyrus-include" /S /Q
)
IF EXIST "pyro" (
    rmdir "pyro" /S /Q
)

:: Download Pyro
curl -L -o pyro.zip https://github.com/fireundubh/pyro/releases/download/1656807840/pyro-master-1656807840.zip
mkdir "pyro"
tar -xf pyro.zip -C pyro
IF EXIST "pyro.zip" (
    del pyro.zip
)

:: Download Base Scripts (Basegame + SKSE)
git clone "https://github.com/MrOctopus/nl_online"
mkdir "papyrus-include"
xcopy "nl_online\sse\source\*" "papyrus-include\" /E /I /Y
xcopy "nl_online\skse64\source\*" "papyrus-include\" /E /I /Y
IF EXIST "nl_online" (
    rmdir "nl_online" /S /Q
)

:: Compile Scripts
".\pyro\pyro" -i skyrimse.ppj --compiler-path tools/PapyrusCompiler.exe

:: Cleanup
IF EXIST "papyrus-include" (
    rmdir "papyrus-include" /S /Q
)
IF EXIST "pyro" (
    rmdir "pyro" /S /Q
)

:: Done