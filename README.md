# PlayerVoiceEvents

This is basically my first SkyrimSE Mod, and something like my wet dream.
This Repo will be way more up-to-date than the Official Mod-Page.

See the Mod-Page at [Nexusmods](https://www.nexusmods.com/skyrimspecialedition/mods/143616)
<br>
<br>
<br>
This Mod is essentially a modern replacement for both PCHT and Dragonborn Voice-Barks. It allows for way more customization, with way less issues.

### Why?
Pretty simple:
- PC Head-Tracking has the issue that it uses Sound-Descriptors. Those were never meant for Voice-Files, they have no support for lip-sync, and are limited in dynamics. For instance, if you wanted to use more (or less) than 5 files for an event, you cannot, because a file descriptor expects a specific set of wav-file-paths. You could increase/decrease those via xEdit in the ESP, but that would not be a viable solutions to Voicepack-Creators.
- Dragonborn Voice-Barks fixes this issue and uses the console-command `speaksound`, which can directly play any arbitary voice-file from the disk.
However, it comes with both a lot of voice-overlap, and completely lacks any customization. Essentially, all it allows to change is one chance-slider for every category. This will become an issue very quickly.
Also, it does not seem to get any updates anymore, and is practically dead at this point with the words "I might fix it"

Now, my mod uses the same way as DBVB to play files, the console command `speaksound` - However, instead of using hardcoded cooldowns and paths, it uses a config-file from a voice-pack, that defines a list of registered categories.
Every one of those categories has the following:
- A percentage-chance to play a sound whenever the event triggers - This is per-event, and can therefore allow for a lot more immersion, since now combat-events don't always play at the same rate as idle-events. - Defaults to 100% if not specified
- A cooldown that can be used to make sure that an event never plays more often than every X seconds - Really useful for dialogue-type voicelines. - Defaults to 0.0seconds
- A file-queue - If enabled (per event), if this event triggers while another dialogue-file is playing, it gets added to a queue, and played automatically ~1s after the current voice-line is done - This allows for "important" voice-lines to not become ignored.
- NOTE: This queue has no effect on the above cooldown - That cooldown is only for making sure a specific category doesn't play too often.
- A list of voice-file-paths for this category. Each of those files additionally has the following configuration options:
- A delay that specifies how many milliseconds have to pass after a file plays until a new file can play - This essentially makes sure that voice-files don't overlap, since there is technically no option to detect if a dialogue is playing. If the queue-option is enabled for the current event, any even that gets triggered during the playtime of a file will play automatically afterwards, so it doesn't get swallowed.


### How to create a Voicepack?
Pretty simple:
- Create a new mod in your mod-manager. Please for the love of god use a mod manager...
- Create a config file called `config.json` inside the virtual file-tree `<Data>/Sound/FX/LNTC_PlayerVoiceEvents/`
- This file should have the following base:
```json
{
    "sounds": {
    }
}
```
- Now, you want to decide what events you want to have voices for - You can find a list of events [here](./Event_List.jsonc) For this example, we'll use the light-attack swing:
```json
{
    "sounds": {
        "PVEAttackMelee": {
            "chance": 100,
            "cooldown": 0.0,
            "files": [
                "path/to/file00.wav:0.3",
                "path/to/file01.wav:0.3",
                "path/to/file00.wav:0.3"
            ]
        },
        "SomeOtherEvent": {
            "...": ""
        }
    }
}
```

Here, We tell the mod that we want to play sounds for the event `PVEAttackMelee`, which is the light-attack event.
We Also tell it that it has a 100% chance to play (aka always), a cooldown of 0.0seconds (aka none), and a list of wav-files and their relative paths. relative means that the path is going from the config-directory. so in this case, it wants the files in `<Data>/Sound/FX/LNTC_PlayerVoiceEvents/path/to/file**.wav`
Also, we give each file a length of 0.3seconds - This is obviously just an example. You want to set this to whatever length your wav-file has. This makes sure that no file can play as long as this delay hasn't passed.

Now you can save this, place your wav-files in the corresponding directories, and test it out. If you load a save (new or existing), it will give you a custom voice type (to silence vanilla grunts), show you a "loading" notification on the top-left, and once it's done your light-attack-swings should play your wav-files according to the given config.

You can find an example voicepack with a few registered categories in the optional files. It should be enough for pack-creators to get a grasp.
Also: I don't care what you use in your voice-packs, If it's AI or not. But I am not responsible tho for any voice-packs created, and i want nothing to do with any AI-Generated content. Always make sure you have the proper rights for your voice-files to be on the safe side <3

Please if you used this and you notice any issues, bugs, problems, or just have questions or ideas, you can always post them in either the posts-section, or you can join my little empty discord server here.

Credits:
- Credits go to the creators of the following mods:
    - Dragonborn Voice-Barks
    - PC Head-Tracking
    - SKSE
    - Papyrus Extender
    - JContainers
    - ConsoleUtilSSE NG
- Additional Credits go to Bethesda for creating Skyrim!

- If you think i forgot you here, just tell me. It was not intentional <3
