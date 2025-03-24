#pragma once

namespace PVE {
    class PapyrusFunctions {
    public:
        static bool Funcs(RE::BSScript::IVirtualMachine *vm);

    private:
        static void PlaySound(RE::StaticFunctionTag *, std::string category, std::string subCategory);

        static void StopSound(RE::StaticFunctionTag *);

        static void Log(RE::StaticFunctionTag *, std::string msg);

        static std::string Replace(RE::StaticFunctionTag *, std::string text, std::string oldSeq, std::string newSeq);
    };
}
