#include "../include/PVE.h"

namespace PVE {
    RE::BSEventNotifyControl PublicEventSink::ProcessEvent(const SKSE::ActionEvent* a_event, RE::BSTEventSource<SKSE::ActionEvent>*) {
        if (a_event->type == SKSE::ActionEvent::Type::kWeaponSwing) {
        }
        return RE::BSEventNotifyControl::kContinue;
    }
}