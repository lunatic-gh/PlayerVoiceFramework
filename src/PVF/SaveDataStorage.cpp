#include "../include/PVF/SaveDataStorage.h"

#include <nlohmann/json.hpp>

namespace PVF {

    SaveDataStorage* SaveDataStorage::GetSingleton() {
        if (const auto player = RE::PlayerCharacter::GetSingleton(); player && player->Is3DLoaded()) {
            static SaveDataStorage instance;
            return &instance;
        }
        return nullptr;
    }

    void SaveDataStorage::Set(const std::string& key, const DataValue& value) {
        std::lock_guard lock(mutex);
        storage[key] = value;
    }

    void SaveDataStorage::ModifyIfExists(const std::string& key, const std::function<DataValue(DataValue data)>& modifyFnct) {
        std::lock_guard lock(mutex);
        if (const auto it = storage.find(key); it != storage.end()) { it->second = modifyFnct(it->second); }
    }

    void SaveDataStorage::Remove(const std::string& key) {
        std::lock_guard lock(mutex);
        if (const auto it = storage.find(key); it != storage.end()) storage.erase(it);
    }

    DataValue SaveDataStorage::Get(const std::string& key, const DataValue& def) {
        std::lock_guard lock(mutex);
        if (const auto it = storage.find(key); it != storage.end()) return it->second;
        return def;
    }

    DataValue SaveDataStorage::Get(const std::string& key) {
        std::lock_guard lock(mutex);
        if (const auto it = storage.find(key); it != storage.end()) return it->second;
        return DataValue();
    }

    bool SaveDataStorage::Has(const std::string& key) const {
        std::lock_guard lock(mutex);
        return storage.contains(key);
    }

    void SaveDataStorage::Save(const SKSE::SerializationInterface* a_intfc) {
        std::lock_guard lock(mutex);
        // ReSharper disable CppExpressionWithoutSideEffects
        // ReSharper disable once CppMultiCharacterLiteral
        if (a_intfc->OpenRecord('PVF', 0)) {
            nlohmann::json json;
            json["version"] = storageVersion;
            for (const auto& [key, data] : storage) {
                nlohmann::json valObj;
                valObj["type"] = data.GetType();
                switch (data.GetType()) {
                    case DataValue::Type::kString: {
                        valObj["value"] = data.AsString();
                        break;
                    }
                    case DataValue::Type::kInt: {
                        valObj["value"] = data.AsInt();
                        break;
                    }
                    case DataValue::Type::kFloat: {
                        valObj["value"] = data.AsFloat();
                        break;
                    }
                    case DataValue::Type::kBool: {
                        valObj["value"] = data.AsBool();
                        break;
                    }
                    case DataValue::Type::kForm: {
                        valObj["value"] = FormUtil::ToString(data.AsForm());
                        break;
                    }
                    default: {
                        break;
                    }
                }
                json["storage"][key] = valObj;
            }
            const std::string jsonData = json.dump();
            const std::uint32_t jsonSize = static_cast<std::uint32_t>(jsonData.size());
            a_intfc->WriteRecordData(&jsonSize, sizeof(jsonSize));
            a_intfc->WriteRecordData(jsonData.data(), jsonSize);
        }
        // ReSharper restore CppExpressionWithoutSideEffects
    }

    void SaveDataStorage::Load(const SKSE::SerializationInterface* a_intfc) {
        std::lock_guard lock(mutex);
        storage.clear();
        std::uint32_t recType = 0, version = 0, length = 0;
        while (a_intfc->GetNextRecordInfo(recType, version, length)) {
            // ReSharper disable once CppMultiCharacterLiteral
            if (recType == 'PVF') {
                std::uint32_t jsonSize = 0;
                if (!a_intfc->ReadRecordData(&jsonSize, sizeof(jsonSize))) return;

                std::string jsonData(jsonSize, '\0');
                if (!a_intfc->ReadRecordData(&jsonData[0], jsonSize)) return;
                try {
                    const nlohmann::json json = nlohmann::json::parse(jsonData);
                    if (!json.contains("version")) {
                        Logger::LogWarn("Failed to read save-data - Data-Version not found. If the mod was just installed, this can be ignored.");
                        return;
                    }
                    if (const auto v = json["version"].get<std::uint32_t>(); v != storageVersion) {
                        Logger::LogError(std::format("Failed to read save-data - Expected Data-Version {}, got {}", storageVersion, v));
                        return;
                    }
                    if (!json.contains("storage") || !json["storage"].is_object()) {
                        Logger::LogDebug("Save-Data empty...");
                        return;
                    }
                    const auto& storageJson = json["storage"];
                    for (auto it = storageJson.begin(); it != storageJson.end(); ++it) {
                        const std::string& key = it.key();
                        const auto& valObj = it.value();
                        switch (int type = valObj["type"].get<int>()) {
                            case static_cast<int>(DataValue::Type::kString): {
                                const auto val = valObj["value"].get<std::string>();
                                storage[key] = DataValue(val.c_str());
                                break;
                            }
                            case static_cast<int>(DataValue::Type::kInt): {
                                const auto val = valObj["value"].get<int>();
                                storage[key] = DataValue(val);
                                break;
                            }
                            case static_cast<int>(DataValue::Type::kFloat): {
                                const auto val = valObj["value"].get<float>();
                                storage[key] = DataValue(val);
                                break;
                            }
                            case static_cast<int>(DataValue::Type::kBool): {
                                const auto val = valObj["value"].get<bool>();
                                storage[key] = DataValue(val);
                                break;
                            }
                            case static_cast<int>(DataValue::Type::kForm): {
                                const auto val = FormUtil::FromString(valObj["value"].get<std::string>());
                                storage[key] = DataValue(val);
                                break;
                            }
                            default:
                                Logger::LogWarn(std::format("Failed to read save-data value - Unrecognized data type '{}', ignoring...", type));
                                break;
                        }
                    }
                } catch (const std::exception& e) {
                    Logger::LogError(std::format("JSON parsing error: {}", e.what()));
                    return;
                }
            }
        }
    }
}