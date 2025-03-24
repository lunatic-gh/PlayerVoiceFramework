#pragma once
#include <RE/T/TESWorldSpace.h>

#include <nlohmann/json.hpp>

#include "SoundEvent.h"

namespace PVE {

    inline std::vector<std::tuple<std::string, std::string, float, float, float, float>> locations;
    inline std::vector<std::tuple<std::string, std::pair<std::string, int>>> quests;
    inline std::optional<RE::TESWorldSpace *> currentWorldspace;
    inline std::string currentLocation;
    inline std::map<std::string, SoundEvent> registeredSoundEvents;
    inline std::optional<SoundEvent> currentSound;
    inline std::map<std::string, float> cooldownMap;

    class Utils {
    public:
        static void LoadData();

        static void PlaySound(const std::string &soundEventName, const std::string &subSoundEventName = "");

        static void Log(const std::string &msg);

        static std::string Replace(const std::string &text, const std::string &oldSeq, const std::string &newSeq);

        static int GenerateRandomInt(const int minInclusive, const int maxInclusive);

        static void RunConsoleCommand(const std::string &command);

        static bool FormHasKeywordString(RE::TESForm *form, const std::string &keyword);

        static float CalculateDistance(const float x1, const float y1, const float x2, const float y2);

        /**
         * This queries if the player-location has changed for the location from locData.
         * @param locData the location data to query for.
         * @return 1 If the given location was left, 2 if it was entered, 0 if nothing changed.
         */
        static int QueryLocationChange(const std::tuple<std::string, std::string, float, float, float, float> &locData);

        static std::vector<std::string> SplitByChar(const std::string &input, const char &delimiter);

    private:
        static void CompileAndRun(RE::Script *script, RE::TESObjectREFR *targetRef, const RE::COMPILER_NAME name = RE::COMPILER_NAME::kSystemWindowCompiler);

        static void CompileAndRunImpl(RE::Script *script, RE::ScriptCompiler *compiler, RE::COMPILER_NAME name, RE::TESObjectREFR *targetRef);

        static RE::NiPointer<RE::TESObjectREFR> GetSelectedRef();

        static RE::ObjectRefHandle GetSelectedRefHandle();
    };
};
