Scriptname LNTC_PVECellChangeDetector extends ActiveMagicEffect

Actor property playerRef auto
ObjectReference property invisibleObject auto

Event OnEffectStart(Actor akTarget, Actor akCaster)
	Utility.Wait(0.1)
	invisibleObject.MoveTo(playerRef)
	LNTC_PVEUtils.PlaySound("PVEChangeCell" + PO3_SKSEFunctions.GetFormEditorID(playerRef.GetParentCell()))
EndEvent
