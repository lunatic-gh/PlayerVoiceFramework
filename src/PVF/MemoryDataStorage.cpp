#include "../include/PVF/MemoryDataStorage.h"

namespace PVF {

    MemoryDataStorage* MemoryDataStorage::GetSingleton() {
        static MemoryDataStorage instance;
        return &instance;
    }

    void MemoryDataStorage::Set(const std::string& key, const DataValue& value) {
        std::lock_guard lock(mutex);
        storage[key] = value;
    }

    void MemoryDataStorage::ModifyIfExists(const std::string& key, const std::function<DataValue(DataValue data)>& modifyFnct) {
        std::lock_guard lock(mutex);
        if (const auto it = storage.find(key); it != storage.end()) {
            it->second = modifyFnct(it->second);
        }
    }

    DataValue MemoryDataStorage::Get(const std::string& key, DataValue def) {
        std::lock_guard lock(mutex);
        if (const auto it = storage.find(key); it != storage.end()) {
            return it->second;
        }
        return def;
    }

    bool MemoryDataStorage::Has(const std::string& key) const {
        return storage.contains(key);
    }
}