@echo off

if not exist "%MO2_SSE_MODS_PATH%" (
    echo "Please set the environment variable 'MO2_SSE_MODS_PATH' to your mo2 mods directory"
    exit /B 1
)

set TARGET_DIR=%MO2_SSE_MODS_PATH%\Player-Voice-Events
if not exist "%TARGET_DIR%" mkdir "%TARGET_DIR%"
powershell -Command "Expand-Archive -Path '%~dp0..\release\PlayerVoiceEvents.zip' -DestinationPath '%TARGET_DIR%' -Force"
echo Mod Created successfully.