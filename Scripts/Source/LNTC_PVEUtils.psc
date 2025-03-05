Scriptname LNTC_PVEUtils

;
; NOTE TO MYSELF: NEVER CHANGE DESCRIPTOR, OR ELSE A LOT OF PEOPLE WILL CRY.
;
; Function PlaySound(String _category, String _subCategory = "", Bool _addToQueue = false) Global
;

Function PlaySound(String _category, String _subCategory = "") Global
    Int categoriesMap = LNTC_PVEStorageUtils.GetJContainer("LNTC_PVE", "sounds")
    Int categoryCooldowns = LNTC_PVEStorageUtils.GetJContainer("LNTC_PVE", "categoryCooldowns", JArray.object())
    Int categoryMap = JMap.getObj(categoriesMap, _category)
    Int subCategoryMap = JMap.getObj(categoriesMap, _subCategory)
    String s = ""
    Int i = 0
    If _subCategory != "" && subCategoryMap != 0 && JArray.count(JMap.GetObj(subCategoryMap, "files", JArray.object())) > 0
        s = _subCategory
        i = subCategoryMap
    ElseIf _category != "" && categoryMap != 0 && JArray.count(JMap.getObj(categoryMap, "files", JArray.object())) > 0
        s = _category
        i = categoryMap
    Else
        LNTC_PVEUtils.LogDebug("Category '" + _category + "' Ignored...")
        Return
    EndIf
    If LNTC_PVEStorageUtils.GetInt("LNTC_PVE", "isSoundPlaying", 0) == 0 && JArray.countString(categoryCooldowns, _category) == 0
        Int categoryChance = JMap.getInt(i, "chance", 100) ; Default
        Float categoryCooldown = JMap.GetFlt(i, "cooldown", 0.15) ; Default
        String[] files = JArray.asStringArray(JMap.GetObj(i, "files", JArray.object()))
        If Utility.RandomInt(0, 99) < categoryChance
            String fileEntry = files[Utility.RandomInt(0, files.Length - 1)]
            String[] split = StringUtil.Split(fileEntry, ":")
            String filePath = split[0]
            Float fileLength = 0.15 ; Default
            If split.Length >= 2
                fileLength = split[1] as Float
            EndIf
            LNTC_PVEStorageUtils.SetInt("LNTC_PVE", "isSoundPlaying", 1)
            LNTC_PVEStorageUtils.AddStringToJArray("LNTC_PVE", "categoryCooldowns", _category)
            ConsoleUtil.ExecuteCommand("player.speaksound \"FX/LNTC_PlayerVoiceEvents/" + filePath + "\"")
            LNTC_PVEUtils.LogDebug("Played Sound '" + s + "'")
            Utility.Wait(fileLength)
            LNTC_PVEStorageUtils.SetInt("LNTC_PVE", "isSoundPlaying", 0)
            Utility.Wait(categoryCooldown)
            LNTC_PVEStorageUtils.RemoveStringFromJArray("LNTC_PVE", "categoryCooldowns", _category)
            ProcessQueue()
        EndIf
    ElseIf JMap.getInt(i, "addToQueue", 0)
        LNTC_PVEStorageUtils.AddStringToJArray("LNTC_PVE", "queuedSounds", _category + "|" + _subCategory)
        LNTC_PVEUtils.LogDebug("Sound '" + _category + "/" + _subCategory + "' added to queue")
    EndIf
EndFunction

Function ProcessQueue() Global
    Int i = LNTC_PVEStorageUtils.GetJContainer("LNTC_PVE", "queuedSounds", JArray.object())
    If JArray.count(i) > 0
        String snd = JArray.getStr(i, 0)
        String[] split = StringUtil.Split(snd, "|")
        String s = ""
        String s1 = ""
        If split.Length > 0
            s = split[0]
            If split.Length > 1
                s1 = split[1]
            EndIf
        EndIf
        LNTC_PVEStorageUtils.RemoveStringFromJArray("LNTC_PVE", "queuedSounds", snd)
        PlaySound(s, s1)
    EndIf
EndFunction


; Returns true if the file `.LNTC_PVE_DEV_MODE` exists in "<Game>/Data"
; Used for more verbose logging and testing.
Bool Function IsDeveloperMode() Global
    Return JContainers.fileExistsAtPath("Data/.LNTC_PVE_DEV_MODE")
EndFunction

; Traces a message _msg to the script-log (if papyrus-logging is enabled in the skyrim ini, it isn't by default!), Prefixed by the Mod-Name for easier reading.
Function Log(String _msg) Global
    Debug.Trace("[Player Voice Events] " + _msg)
EndFunction

; Same as above, but this only runs when developer-mode is active (see the top of this file) to avoid cluttering user's log files.
Function LogDebug(String _msg) Global
    If IsDeveloperMode()
        Log(_msg)
    EndIf
EndFunction