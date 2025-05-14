@echo off

:: Try with tail - It's what i use, since it has support for real-time viewing.
:: You can get it as part of UnxUtils here: https://sourceforge.net/projects/unxutils
echo Trying Tail...
tail -f "%HOMEPATH%\Documents\My Games\Skyrim Special Edition\SKSE\PlayerVoiceFramework.log"
if ERRORLEVEL 1 (
    :: If tail does not exist, use Powershell's Get-Output.
    echo Tail not found - Using Get-Content
    echo NOTE: This will only update once per second. If you want real live output, put tail.exe into your path.
    powershell -command "Get-Content -Wait '%HOMEPATH%\Documents\My Games\Skyrim Special Edition\SKSE\PlayerVoiceFramework.log'"
)