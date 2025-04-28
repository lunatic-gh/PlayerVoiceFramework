#include "../../include/PVE/EventHandler.h"

#include "../../include/PVE/ConditionManager.h"
#include "../../include/PVE/SoundManager.h"

namespace PVE {
    RE::BSEventNotifyControl PublicEventSink::ProcessEvent(const SKSE::ActionEvent* e, RE::BSTEventSource<SKSE::ActionEvent>*) {
        static std::atomic<bool> cooldownFlag(false);
        static auto cooldownTime = std::chrono::steady_clock::now();
        auto now = std::chrono::steady_clock::now();
        if (cooldownFlag.load() && now < cooldownTime) {
            return RE::BSEventNotifyControl::kContinue;
        }
        cooldownFlag.store(true);
        cooldownTime = now + std::chrono::milliseconds(200);

        if (!e || !e->actor) {
            return RE::BSEventNotifyControl::kContinue;
        }

        if (!e->actor || !e->actor->IsPlayerRef()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const SKSE::ActionEvent::Type type = e->type.get();
        const auto playerActor = e->actor;
        const auto source = e->sourceForm;

        if (type == SKSE::ActionEvent::Type::kWeaponSwing) {
            ConditionManager::GetSingleton()->RegisterCondition("PVEAttackMelee", "AttackType", [playerActor]() {
                if (const auto currentProcess = playerActor->GetActorRuntimeData().currentProcess; currentProcess && currentProcess->high && currentProcess->high->attackData) {
                    const auto flags = currentProcess->high->attackData->data.flags;
                    if (flags.any(RE::AttackData::AttackFlag::kPowerAttack)) return 1;
                    if (flags.any(RE::AttackData::AttackFlag::kBashAttack)) return 2;
                }
                return 0;
            });
            ConditionManager::GetSingleton()->RegisterCondition("PVEAttackMelee", "WeaponType", [source]() { return source && source->IsWeapon() ? static_cast<int>(source->As<RE::TESObjectWEAP>()->GetWeaponType()) : 0; });
            ConditionManager::GetSingleton()->RegisterCondition("PVEAttackMelee", "WeaponForm", [source]() { return source; });
            SoundManager::GetSingleton()->SendSoundEvent("PVEAttackMelee");
        } else if (type == SKSE::ActionEvent::Type::kBowDraw) {
            ConditionManager::GetSingleton()->RegisterCondition("PVEAttackMelee", "WeaponForm", [source]() { return source; });
            SoundManager::GetSingleton()->SendSoundEvent("PVEDrawBow");
        }
        return RE::BSEventNotifyControl::kContinue;
    }

    RE::BSEventNotifyControl PublicEventSink::ProcessEvent(const RE::TESPlayerBowShotEvent* e, RE::BSTEventSource<RE::TESPlayerBowShotEvent>*) {
        if (e) {
            ConditionManager::GetSingleton()->RegisterCondition("PVEReleaseBow", "WeaponForm", [e] { return RE::TESForm::LookupByID(e->weapon); });
            ConditionManager::GetSingleton()->RegisterCondition("PVEReleaseBow", "DrawPower", [e] { return e->shotPower; });
            SoundManager::GetSingleton()->SendSoundEvent("PVEReleaseBow");
        }
        return RE::BSEventNotifyControl::kContinue;
    }

    RE::BSEventNotifyControl PublicEventSink::ProcessEvent(const RE::MenuOpenCloseEvent* e, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) {
        static bool paused = false;
        if (e->opening && !paused) {
            paused = true;
            if (const auto soundManager = SoundManager::GetSingleton(); soundManager->IsSoundEventPlaying()) {
                if (e->menuName == "Loading Menu") soundManager->StopCurrentSoundEvent();
                soundManager->GetCurrentSoundHandle()->Pause();
            }
        } else if (!e->opening && paused && (RE::UI::GetSingleton() && !RE::UI::GetSingleton()->GameIsPaused())) {
            paused = false;
            if (const auto soundManager = SoundManager::GetSingleton(); soundManager->GetCurrentSoundHandle().has_value() && !soundManager->IsSoundEventPlaying()) {
                soundManager->GetCurrentSoundHandle()->Play();
            }
        }
        return RE::BSEventNotifyControl::kContinue;
    }
}