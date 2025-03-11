#pragma once
#include <nlohmann/json.hpp>

#include "Logger.h"

namespace PVE {
    struct SoundEvent {
        int chance;
        float cooldown;
        int overrideMode;
        std::map<std::string, float> files;
    };

    inline std::map<std::string, SoundEvent> registeredSoundEvents;
    inline std::vector<std::string> soundEventCooldowns;
    inline bool isPickupOnCooldown = false;
    inline bool isSoundPlaying = false;

    class Utils {
    public:
        static bool IsDevelopmentMode() {
            return std::filesystem::exists("Data/.LNTC_PVE_DEV_MODE");
        }

        static void LoadConfig();

        static void PlaySound(const std::string &soundEventName, const std::string &subSoundEventName = "");

        static void StopSound() {
            RE::PlayerCharacter::GetSingleton()->PauseCurrentDialogue();
        }

        static void Log(const std::string &msg) {
            logger::info("{}", msg);
        }


        static void LogDebug(std::string msg) {
            if (IsDevelopmentMode()) {
                Log(msg);
            }
        }

        static int generateRandomInt(int minInclusive, int maxInclusive) {
            std::random_device randomDev;
            std::mt19937 randomGen(randomDev());
            std::uniform_int_distribution<> randomRange(minInclusive, maxInclusive);
            return randomRange(randomGen);
        }

        static void RunConsoleCommand(std::string command) {
            const auto scriptFactory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::Script>();
            const auto script = scriptFactory ? scriptFactory->Create() : nullptr;
            if (script) {
                script->SetCommand(command);
                CompileAndRun(script, GetSelectedRef().get());
                delete script;
            }
        }

        static bool FormHasKeywordString(RE::TESForm *form, const std::string &keyword) {
            if (!form) return false;
            const auto keywordForm = form->As<RE::BGSKeywordForm>();
            if (!keywordForm) return false;
            return keywordForm->HasKeywordString(keyword);
        }

    private:
        static void CompileAndRun(RE::Script *script, RE::TESObjectREFR *targetRef, RE::COMPILER_NAME name = RE::COMPILER_NAME::kSystemWindowCompiler) {
            RE::ScriptCompiler compiler;
            CompileAndRunImpl(script, &compiler, name, targetRef);
        }

        static void CompileAndRunImpl(RE::Script *script, RE::ScriptCompiler *compiler, RE::COMPILER_NAME name, RE::TESObjectREFR *targetRef) {
            using func_t = decltype(CompileAndRunImpl);
            REL::Relocation<func_t> func{RELOCATION_ID(21416, REL::Module::get().version().patch() < 1130 ? 21890 : 441582)};
            return func(script, compiler, name, targetRef);
        }

        static RE::NiPointer<RE::TESObjectREFR> GetSelectedRef() {
            auto handle = GetSelectedRefHandle();
            return handle.get();
        }

        static RE::ObjectRefHandle GetSelectedRefHandle() {
            REL::Relocation<RE::ObjectRefHandle *> selectedRef{RELOCATION_ID(519394, REL::Module::get().version().patch() < 1130 ? 405935 : 504099)};
            return *selectedRef;
        }
    };
};
