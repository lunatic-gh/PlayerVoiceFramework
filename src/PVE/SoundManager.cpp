#include "../../include/PVE/SoundManager.h"

#include "../../include/PVE/ConditionManager.h"
#include "../../include/PVE/MemoryDataStorage.h"
#include "../../include/PVE/SaveDataStorage.h"
#include "../../include/PVE/Util.h"
#include <ranges>

namespace PVE {
    void SoundManager::RegisterSoundEvent(const std::string& name, const int chance, const float cooldown, const std::vector<std::string>& overrideBlacklist, const std::vector<std::pair<std::string, std::vector<std::string>>>& audios) {
        this->registeredSoundEvents[name] = SoundEvent{name, chance, cooldown, overrideBlacklist, audios};
    }

    void SoundManager::SendSoundEvent(const SoundEvent& event) {
        static std::atomic cancelFlag(false);
        std::thread([this, event]() mutable {
            const std::string cooldownKey = std::format("pve_{}Cooldown", event.name);
            if (const auto storage = SaveDataStorage::GetSingleton(); !(storage ? storage->Get<int>(cooldownKey, 0) : 0)) {
                if (this->currentSoundEvent.has_value() && Util::Contains(currentSoundEvent->overrideBlacklist, event.name)) {
                    return;
                }

                if (Util::RandomInt(0, 99) >= event.chance) {
                    return;
                }

                for (const auto& [expr, files] : event.audios) {
                    if (files.empty() || !ConditionManager::GetSingleton()->EvaluateExpression(event.name, expr)) {
                        continue;
                    }

                    cancelFlag.store(true);
                    while (this->currentHandle.has_value() || this->currentSoundEvent.has_value()) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(5));
                    }
                    std::string filePath;
                    if (const std::string rngMode = MemoryDataStorage::GetSingleton()->Get<std::string>("pve_rngMode", "default"); rngMode == "default") {
                        filePath = std::format("Sound/PlayerVoiceEvents/SoundData/{}", files.at(Util::RandomInt(0, static_cast<int>(files.size()) - 1)));
                    } else if (rngMode == "biased") {
                        static std::map<std::string, std::vector<std::string>> availableFiles;
                        auto& fileList = availableFiles[event.name];
                        if (fileList.empty()) {
                            fileList = files;
                            std::ranges::shuffle(fileList, std::mt19937{std::random_device{}()});
                        }
                        const std::string s = fileList.front();
                        filePath = std::format("Sound/PlayerVoiceEvents/SoundData/{}", s);
                        if (fileList.size() == 1) {
                            fileList = files;
                            int i = 0;
                            do {
                                std::ranges::shuffle(fileList, std::mt19937{std::random_device{}()});
                                i++;
                            } while (i < 10 && fileList.size() > 1 && fileList.front() == s);
                        } else {
                            fileList.erase(fileList.begin());
                        }
                    }

                    RE::BSSoundHandle handle;
                    RE::BSResource::ID id;
                    id.GenerateFromPath(filePath.c_str());
                    RE::BSAudioManager::GetSingleton()->BuildSoundDataFromFile(handle, id, 0x1A, 1);

                    this->currentHandle.emplace(handle);
                    this->currentSoundEvent.emplace(event);
                    this->currentHandle->Play();
                    Util::LogDebug("Played file {}", filePath);

                    if (storage) {
                        storage->Set(cooldownKey, static_cast<int>(event.cooldown));
                    }

                    cancelFlag.store(false);

                    uint64_t delay = 0;
                    while (this->currentHandle && this->GetCurrentSoundHandle()->GetDuration() <= 0) {
                        if (cancelFlag.load()) {
                            this->StopCurrentSoundEvent();
                            Util::LogInfo("Stopped playing");
                            return;
                        }
                        std::this_thread::sleep_for(std::chrono::milliseconds(5));
                        delay += 5;
                        if (delay >= 2000) {
                            Util::LogWarning("Last sound could not be detected as 'playing' - This is a known issue within the Skyrim Engine.");
                            this->StopCurrentSoundEvent();
                            Util::LogInfo("Stopped playing");
                            return;
                        }
                    }
                    while (this->currentHandle && this->GetCurrentSoundHandle()->GetDuration() > 0) {
                        if (cancelFlag.load()) {
                            this->StopCurrentSoundEvent();
                            Util::LogInfo("Stopped playing");
                            return;
                        }
                        std::this_thread::sleep_for(std::chrono::milliseconds(5));
                    }
                    this->StopCurrentSoundEvent();
                    Util::LogInfo("Stopped playing");
                }
            }
        }).detach();
    }

    void SoundManager::SendSoundEvent(const std::string& name) {
        if (const auto it = this->registeredSoundEvents.find(name); it != this->registeredSoundEvents.end()) {
            SendSoundEvent(it->second);
        }
    }

    void SoundManager::StopCurrentSoundEvent() {
        if (this->currentHandle.has_value()) {
            this->currentHandle->Stop();
        }
        this->currentHandle.reset();
        this->currentSoundEvent.reset();
    }

    bool SoundManager::IsSoundEventPlaying() const {
        return this->currentHandle.has_value() && this->currentHandle->IsPlaying();
    }

    // ReSharper disable once CppDFAEndlessLoop, CppDFAUnreachableCode
    SoundManager::SoundManager() {
        std::thread([this] {
            while (true) {
                if (const auto saveDataStorage = SaveDataStorage::GetSingleton()) {
                    for (auto name : this->registeredSoundEvents | std::views::keys) {
                        const std::string s = std::format("pve_{}Cooldown", name);
                        if (const auto i = saveDataStorage->Get<int>(s, 0); i > 0) {
                            saveDataStorage->Set(s, i - 1);
                        }
                    }
                }
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }).detach();
    }
}