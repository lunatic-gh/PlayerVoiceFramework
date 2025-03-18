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

    inline std::vector<std::tuple<std::string, std::string, float, float, float, float>> locations;
    inline std::vector<std::tuple<std::string, std::pair<std::string, int>, std::map<int, float>>> quests;
    inline std::map<std::string, SoundEvent> registeredSoundEvents;
    inline std::map<std::string, float> cooldownMap;
    inline std::optional<SoundEvent> currentSound;
    inline std::optional<RE::TESWorldSpace *> currentWorldspace;
    inline std::string currentLocation;

    class Utils {
    public:
        static bool IsDevelopmentMode() { return std::filesystem::exists("Data/.LNTC_PVE_DEV_MODE"); }

        static void LoadData();

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
         * This queries if the player-location has changed for the location from locData.
         * @param locData the location data to query for.
         * @return 1 If the given location was left, 2 if it was entered, 0 if nothing changed.
         */
        static int QueryLocationChange(const std::tuple<std::string, std::string, float, float, float, float> &locData) {
            const auto player = RE::PlayerCharacter::GetSingleton();

            const float x = player->GetPositionX();
            const float y = player->GetPositionY();

            auto [locName, worldspaceName, locX, locY, locRadiusEnter, locRadiusLeave] = locData;
            if (player->GetWorldspace() != nullptr && player->GetWorldspace()->GetFullName() == worldspaceName) {
                const float dist = CalculateDistance(x, y, locX, locY);
                if (dist < locRadiusEnter && locName != currentLocation) {
                    currentLocation = locName;
                    return 2;
                }
                if (dist > (locRadiusLeave) && locName == currentLocation) {
                    currentLocation = "";
                    return 1;
                }
            }
            return 0;
        }

        static std::vector<std::string> SplitByChar(const std::string &input, const char &delimiter) {
            std::vector<std::string> result;
            std::stringstream ss(input);
            std::string s;
            while (std::getline(ss, s, delimiter)) result.push_back(s);
            return result;
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
