#include "Main.h"

#include "ConditionManager.h"
#include "CustomEventListener.h"
#include "EventListener.h"

namespace PVE {
    void Main::Init() {
        Utils::LoadData();
        Utils::Log("Registering Default Event-Sink...");
        DefaultEventSink::Register();
        Utils::Log("Registering Misc Event-Sink...");
        CustomEventListener::Register();
        Utils::Log("Registering Event-Conditions...");
        ConditionManager::Init();
        loopManager.StartNew("Cooldowns", [] {
            while (cooldownMap.empty()) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            for (auto& cd : cooldownMap | std::views::values) {
                if (cd == 0.0f || RE::UI::GetSingleton()->GameIsPaused() || !RE::PlayerCharacter::GetSingleton()->Is3DLoaded()) {
                    continue;
                }
                cd = std::max(0.0f, cd - 0.020f);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        });
    }

    void Main::Load() {
        loopManager.StartNew("DynamicSink", [] {
            while (!RE::PlayerCharacter::GetSingleton()->Is3DLoaded()) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            Utils::Log("Registering Dynamic Event-Sink...");
            DynamicEventSink::Register();
            loopManager.Stop("DynamicSink");
        });
    }
}
