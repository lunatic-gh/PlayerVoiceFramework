// ReSharper disable CppParameterMayBeConstPtrOrRef

SKSEPluginLoad(const SKSE::LoadInterface* skse) {
    // Initialize Plugin
    SKSE::Init(skse);
    // Initialize Logger
    PVE::Logger::Init();
    SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message* message) {
        switch (message->type) {
            case SKSE::MessagingInterface::kDataLoaded:
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
                PVE::PublicEventSink::Register();
                break;
            default:
                break;
        }
    });
    return true;
}