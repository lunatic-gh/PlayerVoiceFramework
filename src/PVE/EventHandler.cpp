#include "../../include/PVE/EventHandler.h"
#include "../../include/PVE/SoundManager.h"
#include "../../include/PVE/Util.h"

namespace PVE {
    RE::BSEventNotifyControl PublicEventSink::ProcessEvent(const SKSE::ActionEvent* e, RE::BSTEventSource<SKSE::ActionEvent>*) {
        if (e && e->actor && e->actor->IsPlayerRef()) {
            if (e->type == SKSE::ActionEvent::Type::kWeaponSwing) {
                if (const auto source = e->sourceForm; source->Is(RE::FormType::Weapon)) {
                    SoundManager::GetSingleton()->SendSoundEvent("PVEAttackMelee");
                }
            }
        }
        return RE::BSEventNotifyControl::kContinue;
    }

    RE::BSEventNotifyControl PublicEventSink::ProcessEvent(const RE::MenuOpenCloseEvent* e, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) {
        static bool paused = false;
        if (e->opening && !paused) {
            Util::LogDebug("Pausing...");
            paused = true;
            if (const auto soundManager = SoundManager::GetSingleton(); soundManager->IsSoundEventPlaying()) {
                soundManager->GetCurrentSoundHandle()->Pause();
            }
        } else if (!e->opening && paused && (RE::UI::GetSingleton() && !RE::UI::GetSingleton()->GameIsPaused())) {
            Util::LogDebug("Playing...");
            paused = false;
            if (const auto soundManager = SoundManager::GetSingleton(); soundManager->GetCurrentSoundHandle().has_value() && !soundManager->IsSoundEventPlaying()) {
                soundManager->GetCurrentSoundHandle()->Play();
            }
        }
        return RE::BSEventNotifyControl::kContinue;
    }
}