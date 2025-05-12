#pragma once

#include "Util.h"

namespace PVF {

    class SaveDataStorage {
    public:
        SaveDataStorage(const SaveDataStorage&) = delete;

        SaveDataStorage& operator=(const SaveDataStorage&) = delete;

        static SaveDataStorage* GetSingleton();

        void Set(const std::string& key, const DataValue& value);

        void ModifyIfExists(const std::string& key, const std::function<DataValue(DataValue data)>& modifyFnct);

        DataValue Get(const std::string& key, const DataValue& def);

        DataValue Get(const std::string& key);

        bool Has(const std::string& key) const;

        void Save(const SKSE::SerializationInterface* a_intfc);

        void Load(const SKSE::SerializationInterface* a_intfc);

    private:
        SaveDataStorage() = default;
        mutable std::mutex mutex;
        std::unordered_map<std::string, DataValue> storage;
    };

}
