// ReSharper disable CppParameterMayBeConstPtrOrRef

#include "PVE.h"

SKSEPluginLoad(const SKSE::LoadInterface* skse) {
    // Initialize Plugin
    SKSE::Init(skse);
    // Initialize Logger
    PVE::Logger::Init();
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
                break;
            default:
                break;
        }
    });
    return true;
}