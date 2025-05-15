#include "../../include/PVF/SoundManager.h"

#include "../../include/PVF/ConditionManager.h"
#include "../../include/PVF/MemoryDataStorage.h"
#include "../../include/PVF/SaveDataStorage.h"
#include <ranges>

namespace PVF {
    SoundManager* SoundManager::GetSingleton() {
        static SoundManager instance;
        return &instance;
    }

    std::vector<SoundEvent> SoundManager::GetRegisteredSoundEvents(const std::string& pack) const {
        return registeredSoundEvents.at(pack);
    }
    std::vector<std::string> SoundManager::GetRegisteredPacks() const {
        std::vector<std::string> packs;
        for (const auto& key : registeredSoundEvents | std::views::keys) {
            packs.push_back(key);
        }
        return packs;
    }

    void SoundManager::RegisterSoundEvent(const std::string& packName, const std::string& name, const int chance, const int cooldown, const std::vector<std::string>& overrideBlacklist, const std::vector<std::pair<std::string, std::vector<std::string>>>& audios) {
        this->registeredSoundEvents[packName].emplace_back(name, chance, cooldown, overrideBlacklist, audios);
    }

    void SoundManager::SendSoundEvent(const std::string& name) {
        Logger::LogDebug(std::format("Sending event '{}'", name));
        if (const auto activePack = Util::GetActivePack(); !activePack.empty()) {
            if (const auto packIt = registeredSoundEvents.find(activePack); packIt != registeredSoundEvents.end()) {
                for (const auto& event : packIt->second) {
                    if (event.name == name) {
                        std::thread([this, event, activePack] { SendSoundEvent(event, activePack); }).detach();
                        return;
                    }
                }
            }
        }
    }

    SoundManager::SoundManager() {
        std::thread([this] {
            // ReSharper disable once CppDFAEndlessLoop
            while (true) {
                if (const auto saveDataStorage = SaveDataStorage::GetSingleton()) {
                    if (const auto activePack = Util::GetActivePack(); !activePack.empty()) {
                        for (auto event : this->registeredSoundEvents[activePack]) {
                            const std::string s = std::format("{}.{}.cooldown", activePack, event.name);
                            if (const int intVal = saveDataStorage->Get(s).AsInt(); intVal > 0) {
                                saveDataStorage->ModifyIfExists(s, [](DataValue data) {
                                    data.SetInt(data.AsInt() - 1);
                                    return data;
                                });
                            }
                        }
                    }
                }
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }).detach();
        std::thread([this] {
            // ReSharper disable once CppDFAEndlessLoop
            while (true) {
                if (this->currentSoundHandle.has_value() && !this->currentSoundHandle->IsPlaying()) { this->StopCurrentSoundEvent(); }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }).detach();
    }

    void SoundManager::SendSoundEvent(const SoundEvent& event, const std::string& pack) {
        if (const auto storage = SaveDataStorage::GetSingleton()) {
            if (!this->currentSoundEvent.has_value() || std::ranges::find(this->currentSoundEvent->overrideBlacklist, event.name) == this->currentSoundEvent->overrideBlacklist.end()) {
                if (const std::string cooldownKey = std::format("{}.{}.cooldown", pack, event.name); !storage->Get(cooldownKey).AsInt() && Util::RandomInt(0, 99) < event.chance) {
                    std::vector<std::string> files;
                    for (const auto& [left, right] : event.audios) {
                        if (ConditionManager::GetSingleton()->EvaluateExpression(event.name, left)) {
                            files = right;
                            break;
                        }
                    }
                    if (!files.empty()) {
                        StopCurrentSoundEvent();
                        const unsigned int eventToken = currentSoundID.load();
                        const std::string file = GetRandomFile(pack, files);
                        RE::BSSoundHandle handle;
                        RE::BSResource::ID id;
                        id.GenerateFromPath(file.c_str());
                        RE::BSAudioManager::GetSingleton()->BuildSoundDataFromFile(handle, id, 0x1A, 1);
                        handle.Play();
                        Logger::LogDebug(std::format("Playing '{}'", file));
                        storage->Set(cooldownKey, DataValue(event.cooldown));
                        int i = 0;
                        while (!handle.IsPlaying()) {
                            if (eventToken != currentSoundID.load()) { break; }
                            if (i++ >= 500) break;
                            std::this_thread::sleep_for(std::chrono::milliseconds(1));
                        }
                        if (eventToken == currentSoundID.load()) {
                            this->currentSoundHandle.emplace(handle);
                            this->currentSoundEvent.emplace(event);
                        }
                    }
                }
            }
        }
        ConditionManager::GetSingleton()->UnregisterConditions(event.name);
    }

    std::string SoundManager::GetRandomFile(const std::string& pack, std::vector<std::string> files) {
        return std::format("Sound/PlayerVoiceFramework/Packs/{}/{}", pack, files.at(Util::RandomInt(0, static_cast<int>(files.size()) - 1)));
    }

    void SoundManager::StopCurrentSoundEvent() {
        ++currentSoundID;
        if (this->currentSoundHandle.has_value()) {
            this->currentSoundHandle->Stop();
            this->currentSoundHandle.reset();
        }
        if (this->currentSoundEvent.has_value()) { this->currentSoundEvent.reset(); }
    }
}