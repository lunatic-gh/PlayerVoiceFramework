#include "Main.h"

#include <SKSE/SKSE.h>

#include "EventListener.h"
#include "PapyrusFunctions.h"
#include "Utils.h"

namespace PVE {
    void Main::Init() {
        SKSE::GetPapyrusInterface()->Register(PapyrusFunctions::Funcs);
        // ReSharper disable once CppParameterMayBeConstPtrOrRef
        SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message *message) {
            switch (message->type) {
                case SKSE::MessagingInterface::kDataLoaded: {
                    Utils::LoadConfig();
                    DefaultEventSink::Register();
                    break;
                }
                case SKSE::MessagingInterface::kNewGame:
                case SKSE::MessagingInterface::kPostLoadGame: {
                    RegisterDynamicSinks();
                    break;
                }
                default:
                    break;
            }
        });
    }

    void Main::RegisterDynamicSinks() {
        std::thread([] {
            while (!RE::PlayerCharacter::GetSingleton()->Is3DLoaded()) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            DynamicEventSink::Register();
        }).detach();
    }
}
