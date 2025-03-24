// ReSharper disable CppPassValueParameterByConstReference

#include "PapyrusFunctions.h"

#include "SoundEvent.h"
#include "Utils.h"

namespace PVE {
    void PapyrusFunctions::PlaySound(RE::StaticFunctionTag *, std::string category, std::string subCategory) {
        Utils::PlaySound(category, subCategory);
    }

    void PapyrusFunctions::StopSound(RE::StaticFunctionTag *) {
        if (currentSound && currentSound.has_value()) {
            currentSound->Stop();
        }
    }

    void PapyrusFunctions::Log(RE::StaticFunctionTag *, std::string msg) {
        Utils::Log(std::format("[Papyrus] {}", msg));
    }

    std::string PapyrusFunctions::Replace(RE::StaticFunctionTag *, std::string text, std::string oldSeq, std::string newSeq) {
        return Utils::Replace(text, oldSeq, newSeq);
    }

    bool PapyrusFunctions::Funcs(RE::BSScript::IVirtualMachine *vm) {
        vm->RegisterFunction("PlaySound", "LNTC_PVESKSEFunctions", PlaySound);
        vm->RegisterFunction("StopSound", "LNTC_PVESKSEFunctions", StopSound);
        vm->RegisterFunction("Log", "LNTC_PVESKSEFunctions", Log);
        vm->RegisterFunction("Replace", "LNTC_PVESKSEFunctions", Replace);
        return true;
    }

}