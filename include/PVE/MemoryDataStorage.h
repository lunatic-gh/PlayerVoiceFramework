#pragma once

#include <mutex>
#include <unordered_map>

namespace PVE {
    class MemoryDataStorage {
    public:
        static MemoryDataStorage* GetSingleton() {
            static MemoryDataStorage instance;
            return &instance;
        }

        MemoryDataStorage(const MemoryDataStorage&) = delete;
        MemoryDataStorage& operator=(const MemoryDataStorage&) = delete;

        void Set(const std::string& key, const DataType& value) {
            std::lock_guard lock(mutex);
            storage[key] = value;
            Logger::GetSingleton().LogDebug(std::format("Stored: {}", value.AsString()));
        }

        void ModifyIfExists(const std::string& key, const std::function<DataType(DataType data)>& modifyFnct) {
            std::lock_guard lock(mutex);
            if (const auto it = storage.find(key); it != storage.end()) {
                it->second = modifyFnct(it->second);
            }
        }

        DataType Get(const std::string& key, DataType def) {
            std::lock_guard lock(mutex);
            if (const auto it = storage.find(key); it != storage.end()) {
                Logger::GetSingleton().LogDebug(std::format("Get: {}", it->second.AsString()));
                return it->second;
            }
            return def;
        }

        bool Has(const std::string& key) const {
            return storage.contains(key);
        }

    private:
        MemoryDataStorage() = default;
        mutable std::mutex mutex;
        std::unordered_map<std::string, DataType> storage;
    };
}