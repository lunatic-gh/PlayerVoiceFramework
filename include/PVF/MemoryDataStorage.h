#pragma once

#include "API.h"

namespace PVF {
    class MemoryDataStorage {
    public:
        MemoryDataStorage(const MemoryDataStorage&) = delete;

        MemoryDataStorage& operator=(const MemoryDataStorage&) = delete;

        static MemoryDataStorage* GetSingleton();

        void Set(const std::string& key, const DataValue& value);

        void ModifyIfExists(const std::string& key, const std::function<DataValue(DataValue data)>& modifyFnct);

        DataValue Get(const std::string& key, DataValue def);

        bool Has(const std::string& key) const;

    private:
        MemoryDataStorage() = default;
        mutable std::mutex mutex;
        std::unordered_map<std::string, DataValue> storage;
    };
}