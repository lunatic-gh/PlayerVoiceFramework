#pragma once
#include "Utils.h"

namespace PVE {

    class LoopManager {
    public:
        LoopManager() = default;
        ~LoopManager() {
            StopAll();
        }

        void Start(const std::string& id, const std::function<void()>& fnct);

        void StartNew(const std::string& id, const std::function<void()>& fnct);

        void Stop(const std::string& id);

        void StopAll();

        [[nodiscard]] bool IsRunning(const std::string& id) const;

    private:
        std::unordered_map<std::string, std::atomic_bool*> loops;
        std::mutex mutex_;
    };
}
