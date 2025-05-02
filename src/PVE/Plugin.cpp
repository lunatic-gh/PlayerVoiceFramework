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
                PVE::Util::LoadData();
                PVE::ConditionManager::GetSingleton()->RegisterInternalConditions();
                PVE::PublicEventSink::Register();
                if (!PVE_API::api_ptr) PVE_API::api_ptr = new PVE_API::PlayerVoiceEventsAPI;
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
    SKSE::GetMessagingInterface()->RegisterListener(NULL, [](SKSE::MessagingInterface::Message* message) {
        switch (message->type) {
            case API_TYPE_KEY: {
                message->dataLen = sizeof(PVE_API::PlayerVoiceEventsAPI*);
                *(PVE_API::PlayerVoiceEventsAPI**)message->data = PVE_API::api_ptr;
                break;
            }
            default:
                break;
        }
    });
    return true;
}