Scriptname PlayerVoiceFramework_MCM extends SKI_ConfigBase
{This is the Script for the MCM Menu. It handles all the options and stuff.}

Int pack_option
Int pack_idx

Function OnPageReset(String _page)
    SetCursorFillMode(TOP_TO_BOTTOM)
    String active_pack = PlayerVoiceFramework.GetActivePack()
    AddHeaderOption("General")
    pack_option = AddMenuOption("Pack", active_pack)
EndFunction

Function OnOptionMenuOpen(Int _option)
    If (_option == pack_option)
        String[] packs = PlayerVoiceFramework.GetPacks()
        Debug.Trace(packs)
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
        Debug.Trace(packs)
        If (packs.Length != 0 && _index < packs.Length)
            pack_idx = _index
            PlayerVoiceFramework.SetActivePack(packs[_index])
            SetMenuOptionValue(pack_option, packs[_index])
            Debug.Trace("Changed Voice-Pack to '" + packs[_index] + "'")
        EndIf
    EndIf
EndFunction