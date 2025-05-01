#include "../../include/PVE/EventHandler.h"

#include "../../include/PVE/ConditionManager.h"
#include "../../include/PVE/SoundManager.h"

namespace PVE {
    RE::BSEventNotifyControl PublicEventSink::ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) {
        static bool paused = false;
        if (a_event->opening && !paused) {
            paused = true;
            if (const auto soundManager = SoundManager::GetSingleton(); soundManager->IsSoundEventPlaying()) {
                if (a_event->menuName == "Loading Menu") soundManager->StopCurrentSoundEvent();
                if (soundManager->GetCurrentSoundHandle().has_value()) {
                    soundManager->GetCurrentSoundHandle()->Pause();
                }
            }
        } else if (!a_event->opening && paused && (RE::UI::GetSingleton() && !RE::UI::GetSingleton()->GameIsPaused())) {
            paused = false;
            if (const auto soundManager = SoundManager::GetSingleton(); !soundManager->IsSoundEventPlaying()) {
                if (soundManager->GetCurrentSoundHandle().has_value()) {
                    soundManager->GetCurrentSoundHandle()->Play();
                }
            }
        }
        return RE::BSEventNotifyControl::kContinue;
    }
    RE::BSEventNotifyControl PublicEventSink::ProcessEvent(const SKSE::CameraEvent* a_event, RE::BSTEventSource<SKSE::CameraEvent>* a_eventSource) {
        if (a_event) {
            if (a_event->oldState && a_event->newState) {
                const auto oldState = a_event->oldState->id;
                const auto newState = a_event->newState->id;
                const auto player = RE::PlayerCharacter::GetSingleton();
                if (newState == RE::CameraState::kVATS && player && player->IsInKillMove()) {
                    ProcessKillmoveEvent(0);
                } else if (oldState == RE::CameraState::kVATS) {
                    ProcessKillmoveEvent(1);
                }
            }
        }
        return RE::BSEventNotifyControl::kContinue;
    }

    RE::BSEventNotifyControl PublicEventSink::ProcessEvent(const SKSE::ActionEvent* a_event, RE::BSTEventSource<SKSE::ActionEvent>* a_eventSource) {
        static std::atomic<bool> cooldownFlag(false);
        static auto cooldownTime = std::chrono::steady_clock::now();
        const auto now = std::chrono::steady_clock::now();
        if (cooldownFlag.load() && now < cooldownTime) {
            return RE::BSEventNotifyControl::kContinue;
        }
        cooldownFlag.store(true);
        cooldownTime = now + std::chrono::milliseconds(200);

        if (!a_event || !a_event->actor) {
            return RE::BSEventNotifyControl::kContinue;
        }

        if (!a_event->actor || !a_event->actor->IsPlayerRef()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const SKSE::ActionEvent::Type type = a_event->type.get();
        const auto playerActor = a_event->actor;
        const auto source = a_event->sourceForm;

        if (type == SKSE::ActionEvent::Type::kWeaponSwing) {
            ConditionManager::GetSingleton()->RegisterCondition("PVEAttackMelee", "AttackType", [playerActor]() {
                if (const auto currentProcess = playerActor->GetActorRuntimeData().currentProcess; currentProcess && currentProcess->high && currentProcess->high->attackData) {
                    if (const auto flags = currentProcess->high->attackData->data.flags; flags.any(RE::AttackData::AttackFlag::kPowerAttack)) return 1;
                }
                return 0;
            });
            ConditionManager::GetSingleton()->RegisterCondition("PVEAttackMelee", "WeaponType", [source]() { return source && source->IsWeapon() ? static_cast<int>(source->As<RE::TESObjectWEAP>()->GetWeaponType()) : 0; });
            ConditionManager::GetSingleton()->RegisterCondition("PVEAttackMelee", "WeaponForm", [source]() { return source; });
            ConditionManager::GetSingleton()->RegisterCondition("PVEAttackMelee", "WeaponKeywords", [source]() { return FormUtil::ToKeywordString(source); });
            SoundManager::GetSingleton()->SendSoundEvent("PVEAttackMelee");
        } else if (type == SKSE::ActionEvent::Type::kBeginDraw) {
            if (source && source->Is(RE::FormType::Weapon)) {
                ConditionManager::GetSingleton()->RegisterCondition("PVEUnsheatheWeapon", "WeaponForm", [source] { return source; });
                ConditionManager::GetSingleton()->RegisterCondition("PVEUnsheatheWeapon", "WeaponKeywords", [source]() { return FormUtil::ToKeywordString(source); });
                SoundManager::GetSingleton()->SendSoundEvent("PVEUnsheatheWeapon");
            }
        } else if (type == SKSE::ActionEvent::Type::kEndDraw) {
            if (source && source->Is(RE::FormType::Spell)) {
                ConditionManager::GetSingleton()->RegisterCondition("PVEUnsheatheSpell", "SpellForm", [source] { return source; });
                ConditionManager::GetSingleton()->RegisterCondition("PVEUnsheatheSpell", "SpellKeywords", [source]() {
                    if (const auto spell = source->As<RE::SpellItem>()) {
                        return FormUtil::ToKeywordString(spell->GetAVEffect());
                    }
                    return std::string("");
                });
                SoundManager::GetSingleton()->SendSoundEvent("PVEUnsheatheSpell");
            }
        } else if (type == SKSE::ActionEvent::Type::kBeginSheathe) {
            if (source && source->Is(RE::FormType::Weapon)) {
                ConditionManager::GetSingleton()->RegisterCondition("PVESheatheWeapon", "WeaponForm", [source] { return source; });
                ConditionManager::GetSingleton()->RegisterCondition("PVESheatheWeapon", "WeaponKeywords", [source]() { return FormUtil::ToKeywordString(source); });
                SoundManager::GetSingleton()->SendSoundEvent("PVESheatheWeapon");
            }
        } else if (type == SKSE::ActionEvent::Type::kEndSheathe) {
            if (source && source->Is(RE::FormType::Spell)) {
                ConditionManager::GetSingleton()->RegisterCondition("PVESheatheSpell", "SpellForm", [source] { return source; });
                ConditionManager::GetSingleton()->RegisterCondition("PVESheatheSpell", "SpellKeywords", [source]() {
                    if (const auto spell = source->As<RE::SpellItem>()) {
                        return FormUtil::ToKeywordString(spell->GetAVEffect());
                    }
                    return std::string("");
                });
                SoundManager::GetSingleton()->SendSoundEvent("PVESheatheSpell");
            }
        }
        return RE::BSEventNotifyControl::kContinue;
    }

    RE::BSEventNotifyControl PublicEventSink::ProcessEvent(const RE::TESPlayerBowShotEvent* a_event, RE::BSTEventSource<RE::TESPlayerBowShotEvent>* a_eventSource) {
        if (a_event) {
            ConditionManager::GetSingleton()->RegisterCondition("PVEAttackBow", "WeaponForm", [a_event] { return RE::TESForm::LookupByID(a_event->weapon); });
            ConditionManager::GetSingleton()->RegisterCondition("PVEAttackMelee", "WeaponKeywords", [a_event]() { return FormUtil::ToKeywordString(RE::TESForm::LookupByID(a_event->weapon)); });
            ConditionManager::GetSingleton()->RegisterCondition("PVEAttackBow", "DrawPower", [a_event] { return a_event->shotPower; });
            SoundManager::GetSingleton()->SendSoundEvent("PVEAttackBow");
        }
        return RE::BSEventNotifyControl::kContinue;
    }

    RE::BSEventNotifyControl PublicEventSink::ProcessEvent(const RE::TESDeathEvent* a_event, RE::BSTEventSource<RE::TESDeathEvent>* a_eventSource) {
        if (a_event && !a_event->dead) {
            if (const auto actor = a_event->actorDying; actor && actor->IsPlayerRef()) {
                ConditionManager::GetSingleton()->RegisterCondition("PVEDeath", "KillerForm", [a_event] { return a_event->actorKiller.get(); });
                SoundManager::GetSingleton()->SendSoundEvent("PVEDeath");
            }
        }
        return RE::BSEventNotifyControl::kContinue;
    }
    RE::BSEventNotifyControl PublicEventSink::ProcessEvent(const RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>* a_eventSource) {
        const auto cause = a_event->cause;
        const auto target = a_event->target;
        const auto source = RE::TESForm::LookupByID(a_event->source);
        if (cause && cause->IsPlayerRef()) {
            return RE::BSEventNotifyControl::kContinue;
        }
        if (!target || !target->IsPlayerRef() || target->IsDead()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        ConditionManager::GetSingleton()->RegisterCondition("PVEHit", "IsPowerAttack", [a_event] { return a_event->flags.any(RE::TESHitEvent::Flag::kPowerAttack); });
        ConditionManager::GetSingleton()->RegisterCondition("PVEHit", "IsBashAttack", [a_event] { return a_event->flags.any(RE::TESHitEvent::Flag::kBashAttack); });
        ConditionManager::GetSingleton()->RegisterCondition("PVEHit", "IsAttackBlocked", [a_event] { return a_event->flags.any(RE::TESHitEvent::Flag::kHitBlocked); });
        ConditionManager::GetSingleton()->RegisterCondition("PVEHit", "IsHealingAttack", [cause, source] {
            if (cause && cause->As<RE::Actor>() && cause->As<RE::Actor>()->IsPlayerTeammate() && source && source->Is(RE::FormType::Spell) && FormUtil::HasKeyword(source, "MagicRestoreHealth")) {
                return true;
            }
            return false;
        });
        if (!cause || !cause->As<RE::Actor>() /* || !cause->As<RE::Actor>()->IsInKillMove()*/) {
            SoundManager::GetSingleton()->SendSoundEvent("PVEHit");
        }
        return RE::BSEventNotifyControl::kContinue;
    }
    RE::BSEventNotifyControl PublicEventSink::ProcessEvent(const RE::TESSpellCastEvent* a_event, RE::BSTEventSource<RE::TESSpellCastEvent>* a_eventSource) {
        static std::atomic isCasting(false);
        if (const auto baseForm = RE::TESForm::LookupByID(a_event->spell)) {
            if (const auto spell = baseForm->As<RE::SpellItem>()) {
                if (const auto obj = a_event->object; obj && obj->IsPlayerRef()) {
                    const auto playerActor = obj->As<RE::Actor>();
                    if (isCasting.load()) {
                        return RE::BSEventNotifyControl::kContinue;
                    }
                    isCasting.store(true);
                    ConditionManager::GetSingleton()->RegisterCondition("PVESpellCast", "SpellForm", [spell] { return spell; });
                    ConditionManager::GetSingleton()->RegisterCondition("PVESpellCast", "SpellKeywords", [spell]() { return FormUtil::ToKeywordString(spell->GetAVEffect()); });
                    SoundManager::GetSingleton()->SendSoundEvent("PVESpellCast");
                    std::thread([playerActor, spell] {
                        float f = 1.0f;
                        while (f > 0) {
                            if (playerActor->IsCasting(spell)) {
                                f = 1.0f;
                            }
                            f -= 0.05f;
                            std::this_thread::sleep_for(std::chrono::milliseconds(50));
                        }
                        isCasting.store(false);
                    }).detach();
                }
            }
        }
        return RE::BSEventNotifyControl::kContinue;
    }
    void PublicEventSink::ProcessKillmoveEvent(const int state) {
        ConditionManager::GetSingleton()->RegisterCondition("PVEFinisher", "FinisherState", [state] { return state; });
        ConditionManager::GetSingleton()->RegisterCondition("PVEFinisher", "FinisherTargetForm", [] {
            if (const auto player = RE::PlayerCharacter::GetSingleton()) {
                if (const auto proc = player->GetHighProcess()) {
                    if (const auto target = proc->lastTarget.get()) {
                        return target.get();
                    }
                }
            }
            return static_cast<RE::TESObjectREFR*>(nullptr);
        });
        SoundManager::GetSingleton()->SendSoundEvent("PVEFinisher");
    }

    RE::BSEventNotifyControl DynamicEventSink::ProcessEvent(const RE::BSAnimationGraphEvent* e, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource) {
        if (e) {
            if (const RE::TESObjectREFR* holder = e->holder; holder && holder->IsPlayerRef()) {
                if (e->tag == "SoundPlay.NPCHumanCombatShieldBash") {
                    SoundManager::GetSingleton()->SendSoundEvent("PVEAttackBash");
                } else if (e->tag == "JumpUp") {
                    SoundManager::GetSingleton()->SendSoundEvent("PVEJump");
                } else if (e->tag == "FootSprintRight") {
                    SoundManager::GetSingleton()->SendSoundEvent("PVESprintIdle");
                }
            }
        }
        return RE::BSEventNotifyControl::kContinue;
    }
}