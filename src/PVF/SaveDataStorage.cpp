#include "../include/PVF/SaveDataStorage.h"

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
        if (const auto it = storage.find(key); it != storage.end()) {
            it->second = modifyFnct(it->second);
        }
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
        // ReSharper disable CppExpressionWithoutSideEffects
        std::lock_guard lock(mutex);
        if (a_intfc->OpenRecord('PVF', 0)) {
            constexpr std::uint32_t version = 1;
            a_intfc->WriteRecordData(&version, sizeof(version));

            const auto count = static_cast<std::uint32_t>(storage.size());
            a_intfc->WriteRecordData(&count, sizeof(count));

            for (const auto& [key, data] : storage) {
                const auto keyLength = static_cast<std::uint32_t>(key.size());
                a_intfc->WriteRecordData(&keyLength, sizeof(keyLength));
                a_intfc->WriteRecordData(key.data(), keyLength);
                const int type = static_cast<int>(data.GetType());
                a_intfc->WriteRecordData(&type, sizeof(type));
                switch (data.GetType()) {
                    case DataValue::Type::kString: {
                        const char* str = data.AsCString();
                        const auto strLength = static_cast<std::uint32_t>(std::strlen(str));
                        a_intfc->WriteRecordData(&strLength, sizeof(strLength));
                        a_intfc->WriteRecordData(str, strLength);
                        break;
                    }
                    case DataValue::Type::kInt: {
                        const int intVal = data.AsInt();
                        a_intfc->WriteRecordData(&intVal, sizeof(intVal));
                        break;
                    }
                    case DataValue::Type::kFloat: {
                        const float floatVal = data.AsFloat();
                        a_intfc->WriteRecordData(&floatVal, sizeof(floatVal));
                        break;
                    }
                    case DataValue::Type::kBool: {
                        const bool boolVal = data.AsBool();
                        a_intfc->WriteRecordData(&boolVal, sizeof(boolVal));
                        break;
                    }
                    case DataValue::Type::kForm: {
                        const char* str = FormUtil::ToString(data.AsForm()).c_str();
                        const auto strLength = static_cast<std::uint32_t>(std::strlen(str));
                        a_intfc->WriteRecordData(&strLength, sizeof(strLength));
                        a_intfc->WriteRecordData(str, strLength);
                        break;
                    }
                    case DataValue::Type::kNull:
                    default:
                        break;
                }
            }
        }
        // ReSharper restore CppExpressionWithoutSideEffects
    }

    void SaveDataStorage::Load(const SKSE::SerializationInterface* a_intfc) {
        std::lock_guard lock(mutex);
        storage.clear();
        std::uint32_t recType = 0, version = 0, length = 0;
        while (a_intfc->GetNextRecordInfo(recType, version, length)) {
            if (recType == 'PVF') {
                std::uint32_t dataVersion = 0;
                if (a_intfc->ReadRecordData(&dataVersion, sizeof(dataVersion))) {
                    std::uint32_t count = 0;
                    if (!a_intfc->ReadRecordData(&count, sizeof(count))) {
                        Logger::LogError("Failed to read entry count.");
                        return;
                    }

                    for (std::uint32_t i = 0; i < count; ++i) {
                        std::uint32_t keyLength = 0;
                        if (!a_intfc->ReadRecordData(&keyLength, sizeof(keyLength))) {
                            Logger::LogError(std::format("Failed to read key length at entry {}.", i));
                            return;
                        }
                        std::string key(keyLength, ' ');
                        if (!a_intfc->ReadRecordData(key.data(), keyLength)) {
                            Logger::LogError(std::format("Failed to read key data at entry {}.", i));
                            return;
                        }
                        int dataTypeInt = 0;
                        if (!a_intfc->ReadRecordData(&dataTypeInt, sizeof(dataTypeInt))) {
                            Logger::LogError(std::format("Failed to read data type at entry {}.", i));
                            return;
                        }
                        switch (static_cast<DataValue::Type>(dataTypeInt)) {
                            case DataValue::Type::kString: {
                                std::uint32_t strLength = 0;
                                if (!a_intfc->ReadRecordData(&strLength, sizeof(strLength))) {
                                    Logger::LogError(std::format("Failed to read string length for key {}.", key));
                                    return;
                                }
                                std::string strValue(strLength, ' ');
                                if (!a_intfc->ReadRecordData(strValue.data(), strLength)) {
                                    Logger::LogError(std::format("Failed to read string data for key {}.", key));
                                    return;
                                }
                                storage[key] = DataValue(strValue.c_str());
                                break;
                            }
                            case DataValue::Type::kInt: {
                                int intVal = 0;
                                if (!a_intfc->ReadRecordData(&intVal, sizeof(intVal))) {
                                    Logger::LogError(std::format("Failed to read int data for key {}.", key));
                                    return;
                                }
                                storage[key] = DataValue(intVal);
                                break;
                            }
                            case DataValue::Type::kFloat: {
                                float floatVal = 0.0f;
                                if (!a_intfc->ReadRecordData(&floatVal, sizeof(floatVal))) {
                                    Logger::LogError(std::format("Failed to read float data for key {}.", key));
                                    return;
                                }
                                storage[key] = DataValue(floatVal);
                                break;
                            }
                            case DataValue::Type::kBool: {
                                bool boolVal = false;
                                if (!a_intfc->ReadRecordData(&boolVal, sizeof(boolVal))) {
                                    Logger::LogError(std::format("Failed to read bool data for key {}.", key));
                                    return;
                                }
                                storage[key] = DataValue(boolVal);
                                break;
                            }
                            case DataValue::Type::kForm: {
                                std::uint32_t formID = 0;
                                if (!a_intfc->ReadRecordData(&formID, sizeof(formID))) {
                                    Logger::LogError(std::format("Failed to read formID for key {}.", key));
                                    return;
                                }
                                RE::TESForm* form = RE::TESForm::LookupByID(formID);
                                storage[key] = DataValue(form);
                                break;
                            }
                            default:
                                Logger::LogError(std::format("Unknown data type {} for key {}.", dataTypeInt, key));
                                return;
                        }
                    }
                }
            }
        }
    }
}