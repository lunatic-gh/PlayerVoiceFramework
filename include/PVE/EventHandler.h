#pragma once
#pragma warning(disable : 4100) // "unreferenced formal parameter"
#include "API.h"
#include "ConditionManager.h"
#include "SoundManager.h"
#include "Util.h"

namespace PVE {
    class PublicEventSink final : public RE::BSTEventSink<SKSE::ActionEvent>,
                                  public RE::BSTEventSink<SKSE::CameraEvent>,
                                  public RE::BSTEventSink<RE::TESPlayerBowShotEvent>,
                                  public RE::BSTEventSink<RE::MenuOpenCloseEvent>,
                                  public RE::BSTEventSink<RE::TESDeathEvent>,
                                  public RE::BSTEventSink<RE::TESHitEvent>,
                                  public RE::BSTEventSink<RE::TESSpellCastEvent> {
    public:
        static void Register() {
            static PublicEventSink sink;
            if (const auto actionEventSource = SKSE::GetActionEventSource()) {
                actionEventSource->AddEventSink<SKSE::ActionEvent>(&sink);
            }
            if (const auto scriptEventSourceHolder = RE::ScriptEventSourceHolder::GetSingleton()) {
                scriptEventSourceHolder->AddEventSink<RE::TESPlayerBowShotEvent>(&sink);
                scriptEventSourceHolder->AddEventSink<RE::TESDeathEvent>(&sink);
                scriptEventSourceHolder->AddEventSink<RE::TESHitEvent>(&sink);
                scriptEventSourceHolder->AddEventSink<RE::TESSpellCastEvent>(&sink);
            }
            if (const auto ui = RE::UI::GetSingleton()) {
                ui->AddEventSink<RE::MenuOpenCloseEvent>(&sink);
            }
            if (const auto cameraEventSource = SKSE::GetCameraEventSource()) {
                cameraEventSource->AddEventSink<SKSE::CameraEvent>(&sink);
            }
            //std::thread([] {
            //    static bool isInKillmove = false;
            //    while (true) {
            //        if (const auto player = RE::PlayerCharacter::GetSingleton()) {
            //            if (player->IsInKillMove() && !isInKillmove) {
            //                isInKillmove = true;
            //                sink.ProcessKillmoveEvent(0);
            //                continue;
            //            }
            //            if (!player->IsInKillMove() && isInKillmove) {
            //                isInKillmove = false;
            //                sink.ProcessKillmoveEvent(1);
            //                continue;
            //            }
            //        }
            //        std::this_thread::sleep_for(std::chrono::milliseconds(100));
            //    }
            //}).detach();
        }

        RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;
        RE::BSEventNotifyControl ProcessEvent(const SKSE::CameraEvent* a_event, RE::BSTEventSource<SKSE::CameraEvent>* a_eventSource) override;
        RE::BSEventNotifyControl ProcessEvent(const SKSE::ActionEvent* a_event, RE::BSTEventSource<SKSE::ActionEvent>* a_eventSource) override;
        RE::BSEventNotifyControl ProcessEvent(const RE::TESPlayerBowShotEvent* a_event, RE::BSTEventSource<RE::TESPlayerBowShotEvent>* a_eventSource) override;
        RE::BSEventNotifyControl ProcessEvent(const RE::TESDeathEvent* a_event, RE::BSTEventSource<RE::TESDeathEvent>* a_eventSource) override;
        RE::BSEventNotifyControl ProcessEvent(const RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>* a_eventSource) override;
        RE::BSEventNotifyControl ProcessEvent(const RE::TESSpellCastEvent* a_event, RE::BSTEventSource<RE::TESSpellCastEvent>* a_eventSource) override;
        void ProcessKillmoveEvent(int state);
    };

    class DynamicEventSink final : public RE::BSTEventSink<RE::BSAnimationGraphEvent> {
    public:
        static void Register() {
            static DynamicEventSink sink;
            if (const auto player = RE::PlayerCharacter::GetSingleton()) {
                player->AddAnimationGraphEventSink(&sink);
            }
        }

        RE::BSEventNotifyControl ProcessEvent(const RE::BSAnimationGraphEvent* e, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource) override;
    };
}