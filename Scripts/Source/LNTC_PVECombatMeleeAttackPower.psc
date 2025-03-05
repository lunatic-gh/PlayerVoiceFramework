Scriptname LNTC_PVECombatMeleeAttackPower extends ActiveMagicEffect  

; This magic effect is on a spell, which is triggered on weapon-swing.
; This is the only way (that i know of) to distinguish between normal & power-attacks, since papyrus has no #isPowerAttack, so we handle this with effect-conditions.
Event OnEffectStart(Actor akTarget, Actor akCaster)
    Int staminaPercent = (akTarget.GetActorValuePercentage("Stamina") * 100) as Int
    If staminaPercent < 25
        LNTC_PVEUtils.PlaySound("PVEPowerAttackMelee", "PVEPowerAttackMeleeStaminaLow")
    Else
        LNTC_PVEUtils.PlaySound("PVEPowerAttackMelee")
    EndIf
EndEvent