#pragma once

#include <mutex>
#include <unordered_map>
#include <variant>
#include <RE/T/TESForm.h>

namespace PVE {
    class MemoryDataStorage {
    public:
        static MemoryDataStorage& GetSingleton() {
            static MemoryDataStorage instance;
            return instance;
        }

        MemoryDataStorage(const MemoryDataStorage&) = delete;
        MemoryDataStorage& operator=(const MemoryDataStorage&) = delete;

        void Set(const std::string& key, const std::variant<std::string, int, float, RE::TESForm*>& value) {
            std::lock_guard lock(_mutex);
            storage[key] = value;
        }

        std::variant<std::string, int, float, RE::TESForm*> Get(const std::string& key) const {
            std::lock_guard lock(_mutex);
            return storage.at(key);
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

    private:
        MemoryDataStorage() = default;
        mutable std::mutex _mutex;
        std::unordered_map<std::string, std::variant<std::string, int, float, RE::TESForm*>> storage;
    };
}