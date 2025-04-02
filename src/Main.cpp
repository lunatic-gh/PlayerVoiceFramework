#include "Main.h"

#include "ConditionParser.h"
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
        ConditionParser::Init();
        loopManager.StartNew("Cooldowns", [] {
            while (eventCooldowns.empty()) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            for (auto& cd : eventCooldowns | std::views::values) {
                if (cd == 0.0f || RE::UI::GetSingleton()->GameIsPaused() || !RE::PlayerCharacter::GetSingleton()->Is3DLoaded()) {
                    continue;
                }
                cd = std::max(0.0f, cd - 0.020f);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        });
        loopManager.StartNew("LipHandler", [] {
            if (currentSound && currentSound.has_value()) {
                auto sound = currentSound.value();
                if (sound.GetHandle().GetDuration() >= 450) {
                    Utils::Log("Playing Lip Dummy...");
                    Utils::RunConsoleCommand("player.speaksound \"PlayerVoiceEvents/SoundData/PVE/lip_dummy.lip\"");
                    std::this_thread::sleep_for(std::chrono::milliseconds(300));
                    return;
                }
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
