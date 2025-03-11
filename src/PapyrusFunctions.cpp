// ReSharper disable CppPassValueParameterByConstReference
#include "PapyrusFunctions.h"

#include "Utils.h"


void PlaySound(RE::StaticFunctionTag *, std::string category, std::string subCategory) {
    PVE::Utils::PlaySound(category, subCategory);
}

void StopSound(RE::StaticFunctionTag *) {
    PVE::Utils::StopSound();
}

bool IsDevelopmentMode(RE::StaticFunctionTag *) {
    return PVE::Utils::IsDevelopmentMode();
}

void Log(RE::StaticFunctionTag *, std::string msg) {
    PVE::Utils::Log(std::format("[Papyrus] {}", msg));
}

void LogDebug(RE::StaticFunctionTag *, std::string msg) {
    PVE::Utils::LogDebug(std::format("[Papyrus] {}", msg));
}

bool PVE::PapyrusFunctions::Funcs(RE::BSScript::IVirtualMachine *vm) {
    vm->RegisterFunction("PlaySound", "LNTC_PVESKSEFunctions", PlaySound);
    vm->RegisterFunction("StopSound", "LNTC_PVESKSEFunctions", StopSound);
    vm->RegisterFunction("IsDevelopmentMode", "LNTC_PVESKSEFunctions", IsDevelopmentMode);
    vm->RegisterFunction("Log", "LNTC_PVESKSEFunctions", Log);
    vm->RegisterFunction("LogDebug", "LNTC_PVESKSEFunctions", LogDebug);
    return true;
}
