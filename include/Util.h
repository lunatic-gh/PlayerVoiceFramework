#pragma once
#include <API.h>

namespace PVE {

    /**
     * Various Form-Related Utils
     */
    class FormUtil {
    public:
        /**
         * Returns whether a form contains a keyword string
         * @param form the form
         * @param keyword the keyword-string to check for
         * @return
         */
        static bool HasKeyword(RE::TESForm* form, const std::string& keyword);

        /**
         * Retrieves a form from the given Plugin-Name and FormID
         * @param pluginName the name of the plugin that contains the form
         * @param formId the local form id (without the load order)
         * @return The retrieved form, or nullptr if none found
         */
        static RE::TESForm* FromID(const std::string& pluginName, const RE::FormID& formId);

        /**
         * Converts the given form to a string
         * @param form The Form
         * @return a stringified version of the form in the format <PluginFile>|<LocalFormID>
         */
        static std::string ToString(const RE::TESForm* form);

        /**
         * Retrieves a form from the given string
         * @param formString the Plugin and Local FormID of the form, in the format <PluginFile>|<LocalFormID>
         * @return The form if found, or nullptr if none found
         */
        static RE::TESForm* FromString(const std::string& formString);

        static bool CompareForms(const std::string& first, const std::string& second);

        static bool CompareForms(const RE::TESForm* first, const std::string& second);

        static std::string ToKeywordString(RE::TESForm* form);
    };

    /**
     * Various String-Related Utils
     */
    class StringUtil {
    public:
        /**
         * Replaces all occurrences of oldSequence in text with newSequence.
         * @param text the input text
         * @param oldSeq the sequence to replace
         * @param newSeq the new sequence to replace the old sequence with
         * @return the modified string
         */
        static std::string ReplaceInString(const std::string& text, const std::string& oldSeq, const std::string& newSeq);

        /**
         * Removes all trailing and leading whitespaces from the given string
         * @param text the string to trim
         * @return the trimmed string
         */
        static std::string TrimString(const std::string& text);

        /**
         * Splits a text by the given delimiter.
         * @param text the input text
         * @param delimiter the character to split the text with
         * @return a vector containing the split parts
         */
        static std::vector<std::string> SplitString(const std::string& text, const char& delimiter);
    };

    /**
     * Various Miscellaneous Utilities
     */
    class Util {
    public:
        static void LoadData();

        /**
         * Generates a random int between minInclusive and maxInclusive
         * @param minInclusive Minimum value (inclusive)
         * @param maxInclusive Maximum value (inclusive)
         * @return Random int between min and max
         */
        static int RandomInt(int minInclusive, int maxInclusive);

        /**
         * Generates a random float between minInclusive and maxInclusive
         * @param minInclusive Minimum value (inclusive)
         * @param maxInclusive Maximum value (inclusive)
         * @return Random float between min and max
         */
        static float RandomFloat(float minInclusive, float maxInclusive);

        /**
         * Checks whether a container contains the given value or not.
         * @param container the container to check
         * @param value the value to search for
         * @return true if the container contains the value, false otherwise
         */
        template <typename T>
        static bool Contains(std::vector<T> container, T value) {
            return std::ranges::find(container, value) != container.end();
        }

        /**
         * Various logging functions
         */
        template <class... Args>
        static void LogInfo(spdlog::format_string_t<Args...> fmt, Args&&... args) {
            logger::info(fmt, std::forward<Args>(args)...);
        }
        static void LogInfo(const std::string& s) {
            LogInfo("{}", s);
        }
        template <class... Args>
        static void LogWarning(spdlog::format_string_t<Args...> fmt, Args&&... args) {
            logger::warn(fmt, std::forward<Args>(args)...);
        }
        static void LogWarning(const std::string& s) {
            LogWarning("{}", s);
        }
        template <class... Args>
        static void LogError(spdlog::format_string_t<Args...> fmt, Args&&... args) {
            logger::error(fmt, std::forward<Args>(args)...);
        }
        static void LogError(const std::string& s) {
            LogError("{}", s);
        }
        template <class... Args>
        static void LogCritical(spdlog::format_string_t<Args...> fmt, Args&&... args) {
            logger::critical(fmt, std::forward<Args>(args)...);
        }
        static void LogCritical(const std::string& s) {
            LogCritical("{}", s);
        }
        template <class... Args>
        static void LogDebug(spdlog::format_string_t<Args...> fmt, Args&&... args) {
            logger::debug(fmt, std::forward<Args>(args)...);
        }
        static void LogDebug(const std::string& s) {
            LogDebug("{}", s);
        }
    };
}