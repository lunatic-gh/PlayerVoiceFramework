#pragma once

namespace PVE {
    class PublicEventSink final : public RE::BSTEventSink<SKSE::ActionEvent>, public RE::BSTEventSink<RE::MenuOpenCloseEvent> {
    public:
        RE::BSEventNotifyControl ProcessEvent(const SKSE::ActionEvent* e, RE::BSTEventSource<SKSE::ActionEvent>*) override;

        RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* e, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override;

        static void Register() {
            static PublicEventSink sink;
            SKSE::GetActionEventSource()->AddEventSink<SKSE::ActionEvent>(&sink);
            if (const auto ui = RE::UI::GetSingleton()) {
                ui->AddEventSink<RE::MenuOpenCloseEvent>(&sink);
            }
        }
    };
}