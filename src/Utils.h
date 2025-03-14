#pragma once
#include <nlohmann/json.hpp>

#include "Logger.h"

namespace PVE {
    struct SoundEvent {
        int chance;
        float cooldown;
        bool canBeOverridden;
        bool forceOverrideOthers;
        std::vector<std::string> files;
    };

    inline std::map<std::string, SoundEvent> registeredSoundEvents;
    inline std::map<std::string, float> cooldownMap;
    inline std::optional<SoundEvent> currentSound;
    inline std::optional<RE::TESWorldSpace *> currentWorldspace;

    class Utils {
    public:
        static bool IsDevelopmentMode() { return std::filesystem::exists("Data/.LNTC_PVE_DEV_MODE"); }

        static void LoadConfig();

        static void PlaySound(const std::string &soundEventName, const std::string &subSoundEventName = "");

        static void StopSound() { RE::PlayerCharacter::GetSingleton()->PauseCurrentDialogue(); }

        static void Log(const std::string &msg) { logger::info("{}", msg); }

        static void LogDebug(const std::string &msg) {
            if (IsDevelopmentMode()) {
                Log(msg);
            }
        }

        static std::string Replace(const std::string &text, const std::string &oldSeq, const std::string &newSeq) {
            if (oldSeq.empty()) {
                return text;
            }
            std::string result = text;
            size_t pos = 0;
            while ((pos = result.find(oldSeq, pos)) != std::string::npos) {
                result.replace(pos, oldSeq.length(), newSeq);
                pos += newSeq.length();
            }
            return result;
        }

        static int GenerateRandomInt(int minInclusive, int maxInclusive) {
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

        static float CalculateDistance(const float x1, const float y1, const float x2, const float y2) {
            return std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
        }

        /**
         * This basically queries if the current player-location has changed from the saved "currentLocation"
         * @param player the player (RE::PlayerCharacter).
         * @param locData the location data to query for. This should be a tuple with the location's name, the X and Y coordinates of the location's center, and the
         * distance between location-center and border.
         * @param currentLocation the player's current location. This will be used for tracking enter & leave.
         * @return 0 If the location was left, 1 if it was entered, -1 if nothing changed.
         */
        static int QueryLocationChange(const RE::PlayerCharacter &player, const std::tuple<std::string, float, float, float> &locData, std::string &currentLocation) {
            const float x = player.GetPositionX();
            const float y = player.GetPositionY();

            auto [locName, locX, locY, locRadius] = locData;
            const float dist = CalculateDistance(x, y, locX, locY);
            if (dist <= locRadius && locName != currentLocation) {
                currentLocation = locName;
                return 1;
            }
            if ((dist * 1.15f) > locRadius && locName == currentLocation) {
                currentLocation = "";
                return 0;
            }
            return -1;
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
