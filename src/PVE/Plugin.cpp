// ReSharper disable CppParameterMayBeConstPtrOrRef

#include "../../include/PVE/ConditionManager.h"
#include "../../include/PVE/EventHandler.h"
#include "../../include/PVE/PapyrusInterface.h"
#include "../../include/PVE/SaveDataStorage.h"

SKSEPluginLoad(const SKSE::LoadInterface* skse) {
    // Initialize Plugin
    SKSE::Init(skse);
    // Initialize Logger
    PVE::Logger::GetSingleton();
    SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message* message) {
        switch (message->type) {
            case SKSE::MessagingInterface::kDataLoaded:
                PVE::PapyrusInterface::Register();
                SKSE::GetSerializationInterface()->SetUniqueID(0x455650);
                SKSE::GetSerializationInterface()->SetLoadCallback([](SKSE::SerializationInterface* a_intfc) {
                    if (const auto saveDataStorage = PVE::SaveDataStorage::GetSingleton()) {
                        saveDataStorage->Load(a_intfc);
                    }
                });
                SKSE::GetSerializationInterface()->SetSaveCallback([](SKSE::SerializationInterface* a_intfc) {
                    if (const auto saveDataStorage = PVE::SaveDataStorage::GetSingleton()) {
                        saveDataStorage->Save(a_intfc);
                    }
                });
                if (const auto api = PVE::PlayerVoiceEventsAPI::GetSingleton()) {
                    PVE::Util::LogDebug("Successfully loaded API");
                } else {
                    PVE::Util::LogError("Failed to load API");
                }
                if (const auto api = PVE::PlayerVoiceEventsAPI::GetSingleton()) {
                    PVE::Util::LogDebug("Successfully loaded API #2");
                } else {
                    PVE::Util::LogError("Failed to load API #2");
                }
                PVE::Util::LoadData();
                PVE::ConditionManager::GetSingleton()->RegisterInternalConditions();
                PVE::PublicEventSink::Register();
                break;
            case SKSE::MessagingInterface::kNewGame:
            case SKSE::MessagingInterface::kPostLoadGame: {
                std::thread([] {
                    while (!RE::PlayerCharacter::GetSingleton()->Is3DLoaded()) {
                        std::this_thread::sleep_for(std::chrono::seconds(1));
                    }
                    PVE::DynamicEventSink::Register();
                }).detach();
            }
            default:
                break;
        }
    });
    return true;
}