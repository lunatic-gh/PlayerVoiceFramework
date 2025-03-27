#include "LoopManager.h"

namespace PVE {

    void LoopManager::Start(const std::string& id, const std::function<void()>& fnct) {
        std::lock_guard lock(mutex_);
        if (loops.contains(id)) {
            Utils::Log(std::format("Warning: Tried to start already running loop '{}', Ignoring call.", id));
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
        Utils::Log(std::format("Started loop '{}'.", id));
    }

    void LoopManager::StartNew(const std::string& id, const std::function<void()>& fnct) {
        Stop(id);
        Start(id, fnct);
    }

    void LoopManager::Stop(const std::string& id) {
        std::lock_guard lock(mutex_);
        if (!loops.contains(id)) {
            return;
        }

        *(loops[id]) = false;
        delete loops[id];

        loops.erase(id);
        Utils::Log(std::format("Stopped loop '{}'", id));
    }

    void LoopManager::StopAll() {
        std::lock_guard lock(mutex_);
        for (const auto& val : loops | std::views::values) {
            *(val) = false;
            delete val;
        }
        loops.clear();

        Utils::Log("Stopped all running loops.");
    }

    bool LoopManager::IsRunning(const std::string& id) const {
        return loops.contains(id);
    }
}