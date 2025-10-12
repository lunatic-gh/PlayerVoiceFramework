# PlayerVoiceFramework

## Building:

### Requirements:

- Visual Studio Build-Tools with workload `Desktop Development with C++`
- VCPKG (Part of the above by default)

### Steps:

- Open `x64 Native Tools Command Prompt for VS 2022` from the Windows Start Menu
    - DO NOT USE `Developer Command Prompt`, IT IS NOTE THE SAME!
- Run the following commands:
- ```cmd 
  git clone "https://github.com/lunatic-gh/PlayerVoiceFramework" "%HOMEPATH%/Downloads/PlayerVoiceFramework"
  
  cd "%HOMEPATH%/Downloads/PlayerVoiceFramework"
  
  .\compile_papyrus.bat
  
  cmake --preset release -S . -B .\build\release
  
  cmake --build .\build\release
  ```
- This will give you a ready-to-use mod-folder at `Downloads/PlayerVoiceFramework/deploy`.
    - If you have the `MO2_DEV_MODS_PATH` environment variable set, it will instead be deployed to
      `<MO2_DEV_MODS_PATH>\PlayerVoiceFramework`
