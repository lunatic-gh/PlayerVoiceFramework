#pragma once
#include "Util.h"

namespace PVE {
    class SaveDataStorage {
    public:
        static SaveDataStorage* GetSingleton() {
            if (const auto player = RE::PlayerCharacter::GetSingleton(); player && player->Is3DLoaded()) {
                static SaveDataStorage instance;
                return &instance;
            }
            return nullptr;
        }

        SaveDataStorage(const SaveDataStorage&) = delete;
        SaveDataStorage& operator=(const SaveDataStorage&) = delete;

        void Set(const std::string& key, const std::variant<std::string, int, float, RE::TESForm*>& value) {
            std::lock_guard lock(_mutex);
            storage[key] = value;
        }

        std::variant<std::string, int, float, RE::TESForm*> Get(const std::string& key, const std::variant<std::string, int, float, RE::TESForm*>& def) const {
            std::lock_guard lock(_mutex);
            if (const auto it = storage.find(key); it != storage.end())
                return it->second;
            return def;
        }

        bool Has(const std::string& key) const {
            std::lock_guard lock(_mutex);
            return storage.contains(key);
        }

        void Save(const SKSE::SerializationInterface* a_intfc) {
            // ReSharper disable CppExpressionWithoutSideEffects
            std::lock_guard lock(_mutex);
            if (a_intfc->OpenRecord(0x524f5453, 0)) {
                constexpr std::uint32_t version = 1;
                a_intfc->WriteRecordData(&version, sizeof(version));

                const auto count = static_cast<std::uint32_t>(storage.size());
                a_intfc->WriteRecordData(&count, sizeof(count));

                for (const auto& [key, value] : storage) {
                    auto keyLength = static_cast<std::uint32_t>(key.size());
                    a_intfc->WriteRecordData(&keyLength, sizeof(keyLength));
                    a_intfc->WriteRecordData(key.data(), keyLength);
                    std::visit(
                        [&]<typename T0>(T0&& arg) {
                            using T = std::decay_t<T0>;
                            if constexpr (std::is_same_v<T, std::string>) {
                                constexpr int type = kString;
                                a_intfc->WriteRecordData(&type, sizeof(type));
                                const auto strLength = static_cast<std::uint32_t>(arg.size());
                                a_intfc->WriteRecordData(&strLength, sizeof(strLength));
                                a_intfc->WriteRecordData(arg.data(), strLength);
                            } else if constexpr (std::is_same_v<T, int>) {
                                constexpr int type = kInt;
                                a_intfc->WriteRecordData(&type, sizeof(type));
                                a_intfc->WriteRecordData(&arg, sizeof(arg));
                            } else if constexpr (std::is_same_v<T, float>) {
                                constexpr int type = kFloat;
                                a_intfc->WriteRecordData(&type, sizeof(type));
                                a_intfc->WriteRecordData(&arg, sizeof(arg));
                            } else if constexpr (std::is_same_v<T, RE::TESForm*>) {
                                constexpr int type = kForm;
                                const std::uint32_t formID = arg ? arg->GetFormID() : 0;
                                a_intfc->WriteRecordData(&type, sizeof(type));
                                a_intfc->WriteRecordData(&formID, sizeof(formID));
                            }
                        },
                        value);
                }
            }
            // ReSharper restore CppExpressionWithoutSideEffects
        }

        void Load(const SKSE::SerializationInterface* a_intfc) {
            std::lock_guard lock(_mutex);
            std::uint32_t recType = 0, version = 0, length = 0;
            while (a_intfc->GetNextRecordInfo(recType, version, length)) {
                if (recType == 0x524f5453) {
                    std::uint32_t dataVersion = 0;
                    if (a_intfc->ReadRecordData(&dataVersion, sizeof(dataVersion))) {
                        std::uint32_t count = 0;
                        if (!a_intfc->ReadRecordData(&count, sizeof(count))) {
                            Util::LogError("Failed to read entry count.");
                            return;
                        }

                        for (std::uint32_t i = 0; i < count; ++i) {
                            std::uint32_t keyLength = 0;
                            if (!a_intfc->ReadRecordData(&keyLength, sizeof(keyLength))) {
                                Util::LogError("Failed to read key length at entry {}.", i);
                                return;
                            }
                            std::string key(keyLength, ' ');
                            if (!a_intfc->ReadRecordData(key.data(), keyLength)) {
                                Util::LogError("Failed to read key data at entry {}.", i);
                                return;
                            }
                            int variantType = 0;
                            if (!a_intfc->ReadRecordData(&variantType, sizeof(variantType))) {
                                Util::LogError("Failed to read data type at entry {}.", i);
                                return;
                            }
                            switch (variantType) {
                                case kString: {
                                    std::uint32_t strLength = 0;
                                    if (!a_intfc->ReadRecordData(&strLength, sizeof(strLength))) {
                                        Util::LogError("Failed to read string length for key {}.", key);
                                        return;
                                    }
                                    std::string value(strLength, ' ');
                                    if (!a_intfc->ReadRecordData(value.data(), strLength)) {
                                        Util::LogError("Failed to read string data for key {}.", key);
                                        return;
                                    }
                                    storage[key] = value;
                                    break;
                                }
                                case kInt: {
                                    int value = 0;
                                    if (!a_intfc->ReadRecordData(&value, sizeof(value))) {
                                        Util::LogError("Failed to read int data for key {}.", key);
                                        return;
                                    }
                                    storage[key] = value;
                                    break;
                                }
                                case kFloat: {
                                    float value = 0.0f;
                                    if (!a_intfc->ReadRecordData(&value, sizeof(value))) {
                                        Util::LogError("Failed to read float data for key {}.", key);
                                        return;
                                    }
                                    storage[key] = value;
                                    break;
                                }
                                case kForm: {
                                    std::uint32_t formID = 0;
                                    if (!a_intfc->ReadRecordData(&formID, sizeof(formID))) {
                                        Util::LogError("Failed to read formID for key {}.", key);
                                        return;
                                    }
                                    RE::TESForm* form = RE::TESForm::LookupByID(formID);
                                    storage[key] = form;
                                    break;
                                }
                                default:
                                    Util::LogWarning("Unknown variant type {} for key {}.", variantType, key);
                                    return;
                            }
                        }
                    }
                }
            }
        }

    private:
        SaveDataStorage() = default;
        mutable std::mutex _mutex;
        enum VariantType : int {
            kString = 0,
            kInt = 1,
            kFloat = 2,
            kForm = 3
        };
        std::unordered_map<std::string, std::variant<std::string, int, float, RE::TESForm*>> storage;
    };
}