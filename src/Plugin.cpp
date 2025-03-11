#include "Logger.h"
#include "Main.h"

SKSEPluginLoad(const SKSE::LoadInterface* skse) {
    SKSE::Init(skse);
    PVE::Logger::Init();
    PVE::Main::Init();
    return true;
}