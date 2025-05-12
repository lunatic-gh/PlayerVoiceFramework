// ReSharper disable CppParameterMayBeConstPtrOrRef

#include "../../include/PVF/ConditionManager.h"
#include "../../include/PVF/PapyrusInterface.h"
#include "../../include/PVF/SaveDataStorage.h"

#include <SKSE/Interfaces.h>

SKSEPluginLoad(const SKSE::LoadInterface* skse) {
    // Initialize Plugin
    SKSE::Init(skse);
    // Initialize Logger
    PVF::Logger::Initialize();
    SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message* message) {
        switch (message->type) {
            case SKSE::MessagingInterface::kDataLoaded:
                PVF::PapyrusInterface::Register();
                PVF_API::ptr = new PVF_API::PlayerVoiceFrameworkAPI;
                SKSE::GetSerializationInterface()->SetUniqueID('PVF');
                SKSE::GetSerializationInterface()->SetLoadCallback([](SKSE::SerializationInterface* a_intfc) {
                    if (const auto saveDataStorage = PVF::SaveDataStorage::GetSingleton()) { saveDataStorage->Load(a_intfc); }
                });
                SKSE::GetSerializationInterface()->SetSaveCallback([](SKSE::SerializationInterface* a_intfc) {
                    if (const auto saveDataStorage = PVF::SaveDataStorage::GetSingleton()) { saveDataStorage->Save(a_intfc); }
                });
                PVF::Util::LoadData();
                PVF::ConditionManager::GetSingleton()->RegisterInternalConditions();
                break;
            default:
                break;
        }
    });
    SKSE::GetMessagingInterface()->RegisterListener(NULL, [](SKSE::MessagingInterface::Message* message) {
        switch (message->type) {
            case API_TYPE_KEY: {
                message->dataLen = sizeof(PVF_API::PlayerVoiceFrameworkAPI*);
                *(PVF_API::PlayerVoiceFrameworkAPI**)message->data = PVF_API::ptr;
                break;
            }
            default:
                break;
        }
    });
    return true;
}