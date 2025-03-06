# PlayerVoiceEvents

This is basically my first SkyrimSE Mod, and something like my wet dream.
This Repo will be way more up-to-date than the Official Mod-Page.

See the Mod-Page at [Nexusmods](https://www.nexusmods.com/skyrimspecialedition/mods/143616)
<br>
<br>
<br>
This Mod is essentially a modern replacement for both PCHT and Dragonborn Voice-Barks. It allows for way more customization, with way less issues.

### Requirements:
- SKSE - Nowadays every mod requires this. So does mine, otherwise the simplest functions as #getName wouldn't exist.
- ConsoleUtilSSE NG - Used for playing the sounds.
- JContainers SE - Used for everything file-related, and for help with data-storage.
- powerofthree's Papyrus Extender - For another set of helpful functions

### How to use this?
- You can get a ready-to-use version on the Modpage above
- If you want to use this development version:
  - Download the repo (either through `git clone <url> <dest-path>` or by `Code > Download Zip`)
  - Setup a development environment in vscode (I use [PapyrusLang](https://marketplace.visualstudio.com/items?itemName=joelday.papyrus-lang-vscode) and [Workspace Explorer](https://marketplace.visualstudio.com/items?itemName=tomsaunders-code.workspace-explorer))
  - Change the following settings depending on your environment:
      - ![image](https://github.com/user-attachments/assets/2c24b11b-64a8-4d98-bacc-c454b836947b)
      - ![image](https://github.com/user-attachments/assets/f157974f-b57d-4496-a5b4-cf6aa3ddb259)
      - ![image](https://github.com/user-attachments/assets/aa551927-f0c0-48e8-a6a0-e0c24eb4eafb)
  - Also change the following inside the `skyrimse.ppj`:
      -  ![image](https://github.com/user-attachments/assets/259b6548-7458-47e4-a993-a546b8d39fb1)
          - NOTE: The last entry corresponds to the Vanilla + SKSE-Scripts. You can get them by downloading the CreationKit.
  - Click `Terminal > Run Build Task`.
  - This will compile the scripts into `Scripts/*.pex`
  - The mod consists of everything EXCEPT the following:
      - ![image](https://github.com/user-attachments/assets/53bf4429-f2af-4245-aad0-2810d5935c86)
          - (Some of these might not exist for you anyway)
