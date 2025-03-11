#include "Logger.h"
#include "Main.h"

SKSEPluginLoad(const SKSE::LoadInterface* skse) {
    SKSE::Init(skse);
    SetupLog();
    PVE::Main::Init();
    return true;
}