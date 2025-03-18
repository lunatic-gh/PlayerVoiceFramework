#include "Main.h"

#include "CustomEventListener.h"
#include "EventListener.h"
#include "Utils.h"

namespace PVE {

    void Main::Init() {
        Utils::LoadData();
        DefaultEventSink::Register();
        loopManager.Start("Cooldowns", [] {
            while (cooldownMap.empty()) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            for (auto& [event, cd] : cooldownMap) {
                if (cd == 0.0f || RE::UI::GetSingleton()->GameIsPaused()) {
                    continue;
                }
                cd = std::max(0.0f, cd - 0.020f);
                Utils::Log(std::format("Decreased cooldown of '{}' to '{}'", event, cd));
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        });
        loopManager.StartNew("FileDuration", [] {
            const float& voiceTimer = RE::PlayerCharacter::GetSingleton()->GetActorRuntimeData().voiceTimer;
            if (currentSound.has_value()) {
                Utils::Log("Start File-Timer");
                float voiceTimerDelay = 0.0f;
                while (voiceTimer <= 0.0f) {
                    voiceTimerDelay += 0.005f;
                    if (voiceTimer == -1 && voiceTimerDelay >= 5.0f) {
                        // Failsafe - I've seen VoiceTimer being -1 more than once...
                        Utils::Log("WARNING: Voice-Timer was -1, so we disabled it automatically to prevent an infinite loop...");
                        Utils::Log("If you are using fuz or lip-files, those are known to cause various issues, so you might want to consider using wav's instead.");
                        break;
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
                while (voiceTimer > voiceTimerDelay + 0.500) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
                currentSound.reset();
                Utils::Log("End File-Timer");
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        });
        CustomEventListener::Register();
    }

    void Main::Load() {
        loopManager.StartNew("DynamicSinks", [] {
            while (!RE::PlayerCharacter::GetSingleton()->Is3DLoaded()) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            Utils::Log("Registering Dynamic Sink...");
            DynamicEventSink::Register();
            loopManager.Stop("DynamicSinks");
        });
    }
}
