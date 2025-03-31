#include "PapyrusFunctions.h"

#include "ConditionParser.h"
#include "SoundEvent.h"
#include "Utils.h"

/**
 * Every IDE-Warning related to "const references" should be ignored for this class.
 * Even if your IDE tells you that params can be a const, THEY CAN NOT.
 */
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

    void PapyrusFunctions::RegisterFloatCondition(RE::StaticFunctionTag *, std::string name, float value) {
        return ConditionParser::RegisterCondition(name, [value] { return value; });
    }

    void PapyrusFunctions::RegisterIntCondition(RE::StaticFunctionTag *, std::string name, int value) {
        return ConditionParser::RegisterCondition(name, [value] { return value; });
    }

    void PapyrusFunctions::RegisterBoolCondition(RE::StaticFunctionTag *, std::string name, bool value) {
        return ConditionParser::RegisterCondition(name, [value] { return value; });
    }

    void PapyrusFunctions::RegisterStringCondition(RE::StaticFunctionTag *, std::string name, std::string value) {
        return ConditionParser::RegisterCondition(name, [value] { return value; });
    }

    bool PapyrusFunctions::Funcs(RE::BSScript::IVirtualMachine *vm) {
        vm->RegisterFunction("PlaySound", "LNTC_PVESKSEFunctions", PlaySound);
        vm->RegisterFunction("StopSound", "LNTC_PVESKSEFunctions", StopSound);
        vm->RegisterFunction("Log", "LNTC_PVESKSEFunctions", Log);
        vm->RegisterFunction("Replace", "LNTC_PVESKSEFunctions", Replace);
        vm->RegisterFunction("RegisterFloatCondition", "LNTC_PVESKSEFunctions", RegisterFloatCondition);
        vm->RegisterFunction("RegisterIntCondition", "LNTC_PVESKSEFunctions", RegisterIntCondition);
        vm->RegisterFunction("RegisterBoolCondition", "LNTC_PVESKSEFunctions", RegisterBoolCondition);
        vm->RegisterFunction("RegisterStringCondition", "LNTC_PVESKSEFunctions", RegisterStringCondition);
        return true;
    }

}