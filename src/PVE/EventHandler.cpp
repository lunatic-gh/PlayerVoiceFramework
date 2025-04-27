#include "../../include/PVE/EventHandler.h"

#include "../../include/PVE/ConditionManager.h"
#include "../../include/PVE/SoundManager.h"
#include "../../include/PVE/Util.h"

namespace PVE {
    RE::BSEventNotifyControl PublicEventSink::ProcessEvent(const SKSE::ActionEvent* e, RE::BSTEventSource<SKSE::ActionEvent>*) {
        if (!e || !e->actor) {
            return RE::BSEventNotifyControl::kContinue;
        }

        if (!e->actor || !e->actor->IsPlayerRef()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        if (e->type == SKSE::ActionEvent::Type::kWeaponSwing) {
            if (const auto source = e->sourceForm; source->Is(RE::FormType::Weapon)) {
                ConditionManager::GetSingleton()->RegisterCondition("PVEAttackMelee", "AttackType", [e] {
                    const auto proc = e->actor->GetHighProcess();
                    if (!proc) return -1;
                    const auto data = proc->attackData.get();
                    if (!data) return -1;
                    if (data->data.flags.any(RE::AttackData::AttackFlag::kPowerAttack)) return 1;
                    if (data->data.flags.any(RE::AttackData::AttackFlag::kBashAttack)) return 2;
                    return 0;
                });
                SoundManager::GetSingleton()->SendSoundEvent("PVEAttackMelee");
            }
        }
        return RE::BSEventNotifyControl::kContinue;
    }

    RE::BSEventNotifyControl PublicEventSink::ProcessEvent(const RE::MenuOpenCloseEvent* e, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) {
        static bool paused = false;
        if (e->opening && !paused) {
            paused = true;
            if (const auto soundManager = SoundManager::GetSingleton(); soundManager->IsSoundEventPlaying()) {
                if (e->menuName == "Loading Menu") {
                    soundManager->StopCurrentSoundEvent();
                }
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