Scriptname LNTC_PVEVoiceEvents extends ReferenceAlias  

VoiceType Property PVEVoiceType Auto
Spell Property CombatMeleeAttackSpellRef Auto

Actor playerActor
Bool isSneaking

Event OnInit()
    playerActor = Self.GetActorRef()
    RegisterForActorAction(0)
    RegisterForActorAction(1)
    RegisterForActorAction(2)
    RegisterForActorAction(8)
    RegisterForActorAction(9)
    RegisterForActorAction(10)
    RegisterForAnimationEvent(playerActor, "SoundPlay.NPCHumanCombatShieldBash")
    RegisterForAnimationEvent(playerActor, "JumpUp")
    RegisterForAnimationEvent(playerActor, "FootSprintRight")
    RegisterForCameraState()
    RegisterForSleep()
    Load()
EndEvent

Event OnPlayerLoadGame()
    Load()
EndEvent

Function Load()
    LNTC_PVEUtils.Log("Loading Sounds...")
    Int sounds = JMap.getObj(JValue.readFromFile("Data/Sound/FX/LNTC_PlayerVoiceEvents/config.json"), "sounds", JMap.object())
    LNTC_PVEStorageUtils.SetJMap("LNTC_PVE", "sounds", sounds)
    LNTC_PVEStorageUtils.setInt("LNTC_PVE", "isSoundPlaying", 0)
    LNTC_PVEStorageUtils.setInt("LNTC_PVE", "isHitOnCooldown", 0)
    LNTC_PVEStorageUtils.setInt("LNTC_PVE", "isPickupOnCooldown", 0)
    LNTC_PVEUtils.Log("Sounds Loaded...")
    OnUpdate()
EndFunction

Event OnUpdate()
    UpdateVoiceTypes()
    RegisterForSingleUpdate(5)
EndEvent

Function UpdateVoiceTypes()
    ; Periodically fix voice types if they got reset (e.g. on race change)
    Bool b = false
    If playerActor.GetRace().GetDefaultVoiceType(true) != PVEVoiceType
        playerActor.GetRace().SetDefaultVoiceType(true, PVEVoiceType)    
        b = true
    EndIf
    If playerActor.GetRace().GetDefaultVoiceType(false) != PVEVoiceType
        playerActor.GetRace().SetDefaultVoiceType(false, PVEVoiceType)    
        b = true
    EndIf
    If b
        LNTC_PVEUtils.Log("Updated Voice Type.")
    Endif
EndFunction

Event OnActorAction(int _actionType, Actor _actor, Form _source, int _slot)
    If _actor == playerActor
        If _actionType == 0
            CombatMeleeAttackSpellRef.Cast(_actor, _actor)
        ElseIf _actionType == 1
            If _source as Spell
                LNTC_PVEUtils.PlaySound("PVECastSpell", "PVECastSpell" + _source.GetName())
            EndIf
        ElseIf _actionType == 2
            If _source as Spell
                LNTC_PVEUtils.PlaySound("PVEFireSpell", "PVEFireSpell" + _source.GetName())
            EndIf
        ElseIf _actionType == 8
            If _source.HasKeywordString("WeapTypeBow")
                LNTC_PVEUtils.PlaySound("PVEUnsheathe", "PVEUnsheatheBow")
            ElseIf _source.GetType() == 22
                LNTC_PVEUtils.PlaySound("PVEUnsheathe", "PVEUnsheatheSpell")
            Else
                LNTC_PVEUtils.PlaySound("PVEUnsheathe", "PVEUnsheatheMelee")
            EndIf
        ElseIf _actionType == 9
            If _source.HasKeywordString("WeapTypeBow")
                LNTC_PVEUtils.PlaySound("PVESheathe", "PVESheatheBow")
            Else
                LNTC_PVEUtils.PlaySound("PVESheathe", "PVESheatheMelee")
            EndIf
        ElseIf _actionType == 10
            If _source.GetType() == 22
                LNTC_PVEUtils.PlaySound("PVESheathe", "PVESheatheSpell")
            EndIf
        EndIf
    EndIf
EndEvent

Event OnAnimationEvent(ObjectReference _source, string _event)
    If _event == "SoundPlay.NPCHumanCombatShieldBash"
        Int staminaPercent = (_source.GetSelfAsActor().GetActorValuePercentage("Stamina") * 100) as Int
        If staminaPercent < 25
            LNTC_PVEUtils.PlaySound("PVEBash", "PVEBashStaminaLow")
        Else
            LNTC_PVEUtils.PlaySound("PVEBash", "")
        EndIf
    ElseIf _event == "JumpUp"
        LNTC_PVEUtils.PlaySound("PVEJump")
    ElseIf _event == "FootSprintRight"
        LNTC_PVEUtils.PlaySound("PVESprintIdle")
    EndIf
EndEvent

Event OnPlayerBowShot(Weapon _weapon, Ammo _ammo, Float _power, Bool _isLookingAtSun)
    If _power < 1.0
        LNTC_PVEUtils.PlaySound("PVEAttackBow", "PVEAttackBowLow")
    Else
        LNTC_PVEUtils.PlaySound("PVEAttackBow", "")
    EndIf
EndEvent

Event OnHit(ObjectReference _attacker, Form _source, Projectile _projectile, bool _isPowerAttack, bool _isSneakAttack, bool _isBashAttack, bool _isHitBlocked)
    If !LNTC_PVEStorageUtils.getInt("LNTC_PVE", "isHitOnCooldown", 0)
        LNTC_PVEStorageUtils.setInt("LNTC_PVE", "isHitOnCooldown", 1)
        If Self.GetActorRef() as Actor
            If !Self.GetActorRef().IsDead()
                If _isHitBlocked
                    If _isPowerAttack
                        LNTC_PVEUtils.PlaySound("PVEBlockReceivedHit", "PVEBlockReceivedPowerHit")
                    Else
                        LNTC_PVEUtils.PlaySound("PVEBlockReceivedHit")
                    EndIf
                    Utility.Wait(0.5)
                Else
                    If _attacker as Actor && (_attacker as Actor).IsPlayerTeammate() && _source as Spell
                        If (_source as Spell).HasKeywordString("MagicRestoreHealth")
                            LNTC_PVEUtils.PlaySound("PVEReceivedFriendlyHeal")
                            Utility.Wait(0.5)
                        EndIf
                    Else
                        If _isPowerAttack
                            LNTC_PVEUtils.PlaySound("PVEReceivedHit", "PVEReceivedPowerHit")
                            Utility.Wait(0.5)
                        Else
                            If (_attacker as Actor && _attacker.GetFormID() != 0x14 && !(_attacker as Actor).IsInKillMove()) || !_attacker as Actor
                                LNTC_PVEUtils.PlaySound("PVEReceivedHit")
                                Utility.Wait(0.5)
                            EndIf
                        EndIf
                    EndIf
                EndIf
            EndIf
        EndIf
        LNTC_PVEStorageUtils.setInt("LNTC_PVE", "isHitOnCooldown", 0)
    EndIf
EndEvent

Event OnPlayerCameraState(int _oldState, int _newState)
    If _newState == 2
        LNTC_PVEUtils.PlaySound("PVEFinisherStart", "")
    ElseIf _oldState == 2
        LNTC_PVEUtils.PlaySound("PVEFinisherEnd", "")
    ElseIf _newState == 5
        LNTC_PVEUtils.PlaySound("PVESit")
    ElseIf _oldState == 5
        LNTC_PVEUtils.PlaySound("PVEGetUp")
    ElseIf _newState == 10
        LNTC_PVEUtils.PlaySound("PVEMountHorse")
    ElseIf _oldState == 10
        LNTC_PVEUtils.PlaySound("PVEDismountHorse")
    ElseIf _newState == 11
        LNTC_PVEUtils.PlaySound("PVEDeath")
    ElseIf _newState == 12
        LNTC_PVEUtils.PlaySound("PVEMountDragon")
    ElseIf _oldState == 12
        LNTC_PVEUtils.PlaySound("PVEDismountDragon")
    EndIf
EndEvent

Event OnSleepStart(float _startTime, float _endTime)
    LNTC_PVEUtils.PlaySound("PVESleepStart")
EndEvent

Event OnSleepStop(bool _interrupted)    
    LNTC_PVEUtils.PlaySound("PVESleepEnd")
EndEvent

Event OnItemAdded(Form _item, int _count, ObjectReference _itemRef, ObjectReference _srcContainer)
    If !_srcContainer
        If !LNTC_PVEStorageUtils.getInt("LNTC_PVE", "isPickupOnCooldown", 0)
            LNTC_PVEStorageUtils.setInt("LNTC_PVE", "isPickupOnCooldown", 1)
            If _item.GetType() == 26
                LNTC_PVEUtils.PlaySound("PVEPickupItem", "PVEPickupItemArmor")
            ElseIf _item.GetType() == 27
                LNTC_PVEUtils.PlaySound("PVEPickupItem", "PVEPickupItemBook")
            ElseIf _item.GetType() == 41
                LNTC_PVEUtils.PlaySound("PVEPickupItem", "PVEPickupItemWeapon")
            ElseIf _item.GetType() == 42
                LNTC_PVEUtils.PlaySound("PVEPickupItem", "PVEPickupItemAmmo")
            ElseIf _item.GetType() == 46
                LNTC_PVEUtils.PlaySound("PVEPickupItem", "PVEPickupItemPotion")
            Else
                LNTC_PVEUtils.PlaySound("PVEPickupItem")
            EndIf
            Utility.Wait(1.0)
            LNTC_PVEStorageUtils.setInt("LNTC_PVE", "isPickupOnCooldown", 0)
        EndIf
    EndIf
EndEvent