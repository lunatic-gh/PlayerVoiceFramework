Scriptname PlayerVoiceFramework_MCM extends SKI_ConfigBase
{This is the Script for the MCM Menu. It handles all the options and stuff.}

Int pack_option
Int debug_mode_option
Int pack_idx

Function OnPageReset(String _page)
    SetCursorFillMode(TOP_TO_BOTTOM)
    String active_pack = PlayerVoiceFramework.GetActivePack()
    AddHeaderOption("General")
    pack_option = AddMenuOption("Pack: ", active_pack)
    debug_mode_option = AddToggleOption("Debug Mode: ", PlayerVoiceFramework.IsDebugMode())
EndFunction

Function OnOptionMenuOpen(Int _option)
    If (_option == pack_option)
        String[] packs = PlayerVoiceFramework.GetPacks()
        If (packs.Length != 0)
            SetMenuDialogOptions(packs)
            SetMenuDialogStartIndex(pack_idx)
            SetMenuDialogDefaultIndex(0)
        EndIf
        
    EndIf
    
EndFunction

Function OnOptionMenuAccept(Int _option, Int _index)
    If (_option == pack_option)
        String[] packs = PlayerVoiceFramework.GetPacks()
        If (packs.Length != 0 && _index < packs.Length)
            pack_idx = _index
            PlayerVoiceFramework.SetActivePack(packs[_index])
            SetMenuOptionValue(pack_option, packs[_index])
            If (PlayerVoiceFramework.IsDebugMode())
                Debug.Trace("Changed Voice-Pack to '" + packs[_index] + "'")
            EndIf
            
        EndIf
        
    EndIf
    
EndFunction

Function OnOptionSelect(Int _option)
    If (_option == debug_mode_option)
        Bool debug_state = PlayerVoiceFramework.IsDebugMode()
        PlayerVoiceFramework.SetDebugMode(!debug_state)
        SetToggleOptionValue(debug_mode_option, !debug_state)
    EndIf
    
EndFunction

Function OnOptionHighlight(Int _option)
    If (_option == pack_option)
        SetInfoText("Changes the Active Voice-Pack")
    EndIf
    
EndFunction

