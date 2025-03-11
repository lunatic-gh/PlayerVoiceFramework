# PlayerVoiceEvents

This is basically my first SkyrimSE Mod, and something like my wet dream.
This Repo will be way more up-to-date than the Official Mod-Page, however it will require you to build both the
SKSE-Plugin, and to compile the Papyrus-Scripts.

See the Mod-Page at [Nexusmods](https://www.nexusmods.com/skyrimspecialedition/mods/143616)
<br>
<br>
<br>
This Mod is essentially a modern replacement for both PCHT and Dragonborn Voice-Barks. It allows for way more
customization, with way less issues.

### Requirements:

- SKSE - This mod is like 99% Based on SKSE. Requirement should be obvious...

### Building/Compiling:

- First, you'll need the vanilla papyrus scripts in ``<GameDir>/Data/Scripts/Source``
    - Not ``Source/Scripts``. I fixed the path for this mod.
    - You can get these by downloading the SSE-CreationKit on Steam. It puts a ``scripts.zip`` into the Data folder,
      that you can extract. Make sure that when extracting you fix the path as mentioned above. Bethesda fucked it up...
      Yet Again...
- You also need Visual Studio installed (Yes, the entire god damn fucking dick sucking IDE. NO MATTER IF YOU USE ANOTHER
  IDE, IT IS REQUIRED), together with the ``Desktop Development with C++`` Package.
- Then, Open it in your preferred C++ IDE. I am using CLion.
    - NOTE: When using CLion, it'll likely ask you for a compiler-preset on import. Disable the ``debug`` preset by
      disabling ``enable profile``, and enable it on the ``release`` preset. Then click apply.
- IMPORTANT: Change your Skyrim Game-Path inside ``skyrimse.ppj`` and save the file.
- Build the Project. In CLion, this will be under ``File -> Build -> Build all in 'release'`` - It'll automatically run
  the included batch files, and build/compile both the Papyrus-Scripts and the SKSE-Plugin. This might take a while
  depending on your Hardware.
- Once done, it'll **clearly** say so. It'll give you the zipped mod in the ``release`` folder.
