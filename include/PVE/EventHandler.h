#pragma once

namespace PVE {
    class PublicEventSink final : public RE::BSTEventSink<SKSE::ActionEvent> {
    public:
        RE::BSEventNotifyControl ProcessEvent(const SKSE::ActionEvent* a_event, RE::BSTEventSource<SKSE::ActionEvent>*) override;

        static void Register() {
            static PublicEventSink sink;
            SKSE::GetActionEventSource()->AddEventSink(&sink);
        }
    };
}