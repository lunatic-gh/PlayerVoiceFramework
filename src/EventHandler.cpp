#include "../include/EventHandler.h"

namespace PVE {
    RE::BSEventNotifyControl PublicEventSink::ProcessEvent(const SKSE::ActionEvent* a_event, RE::BSTEventSource<SKSE::ActionEvent>*) {
        if (a_event->type == SKSE::ActionEvent::Type::kWeaponSwing) {
            SoundManager::GetSingleton().PlaySoundEvent("SomeEvent");
        }
        return RE::BSEventNotifyControl::kContinue;
    }
}