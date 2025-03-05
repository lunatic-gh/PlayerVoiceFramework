Scriptname LNTC_PVECellChangeDetectorMarker extends ObjectReference  

Actor property playerRef auto

Event OnCellDetach()
    Utility.Wait(0.1)
    MoveTo(playerRef)
    LNTC_PVEUtils.PlaySound("PVEChangeCell" + PO3_SKSEFunctions.GetFormEditorID(playerRef.GetParentCell()))
EndEvent