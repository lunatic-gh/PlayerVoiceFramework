#pragma once

#include <atomic>
#include <functional>
#include <future>
#include <mutex>
#include <string>
#include <thread>

#include "Utils.h"

namespace PVE {

    class LoopManager {
    public:
        LoopManager() = default;
        ~LoopManager() { StopAll(); }

        void Start(const std::string& id, std::function<void()> fnct) {
            std::lock_guard lock(mutex_);
            if (loops.contains(id)) {
                Utils::LogDebug(std::format("Warning: Tried to start already running loop '{}', Ignoring call.", id));
                return;
            }
            auto running = new std::atomic_bool(true);
            loops[id] = running;
            std::thread([fnct, running, this, id]() {
                thread_local std::string localLoopId = id;
                while (*running) {
                    fnct();
                }
            }).detach();
            Utils::LogDebug(std::format("Started loop '{}'.", id));
        }

        void Stop(const std::string& id) {
            std::lock_guard lock(mutex_);
            if (!loops.contains(id)) {
                return;
            }

            *(loops[id]) = false;
            delete loops[id];
            loops.erase(id);
            Utils::LogDebug(std::format("Stopped loop '{}'", id));
        }

        void StopAll() {
            std::lock_guard lock(mutex_);
            for (auto& loop : loops) {
                *(loop.second) = false;
                delete loop.second;
            }
            loops.clear();
            Utils::LogDebug("Stopped all running loops.");
        }

        bool IsRunning(const std::string& id) {
            return loops.contains(id);
        }

    private:
        std::unordered_map<std::string, std::atomic_bool*> loops;
        std::mutex mutex_;
    };
}
