Scriptname LNTC_PVECombatMeleeAttack extends ActiveMagicEffect

; This magic effect is on a spell, which is triggered on weapon-swing.
; This is the only way (that i know of) to distinguish between normal & power-attacks, since papyrus has no #isPowerAttack, so we handle this with effect-conditions.
Event OnEffectStart(Actor akTarget, Actor akCaster)
    Int staminaPercent = (akTarget.GetActorValuePercentage("Stamina") * 100) as Int
    If staminaPercent == 0
        LNTC_PVEUtils.PlaySound("PVEAttackMelee", "PVEAttackMeleeStaminaOut")
    ElseIf staminaPercent < 25
        LNTC_PVEUtils.PlaySound("PVEAttackMelee", "PVEAttackMeleeStaminaLow")
    Else
        LNTC_PVEUtils.PlaySound("PVEAttackMelee")
    EndIf
EndEvent