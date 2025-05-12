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

    std::optional<SoundEvent> SoundManager::GetCurrentSoundEvent() const {
        return currentSoundEvent;
    }

    std::optional<RE::BSSoundHandle> SoundManager::GetCurrentSoundHandle() const {
        return currentHandle;
    }

    std::vector<SoundEvent> SoundManager::GetRegisteredSoundEvents(const std::string& pack) const {
        return registeredSoundEvents.at(pack);
    }
    std::vector<std::string> SoundManager::GetRegisteredPacks() const {
        std::vector<std::string> packs;
        for (auto key : registeredSoundEvents | std::views::keys) {
            packs.push_back(key);
        }
        return packs;
    }

    void SoundManager::RegisterSoundEvent(const std::string& packName, const std::string& name, const int chance, const int cooldown, const std::vector<std::string>& overrideBlacklist, const std::vector<std::pair<std::string, std::vector<std::string>>>& audios) {
        this->registeredSoundEvents[packName].emplace_back(name, chance, cooldown, overrideBlacklist, audios);
    }

    void SoundManager::SendSoundEvent(const std::string& name) {
        Logger::LogInfo(std::format("Sending event '{}'", name));
        if (const auto activePack = Util::GetActivePack(); !activePack.empty()) {
            if (const auto packIt = registeredSoundEvents.find(activePack); packIt != registeredSoundEvents.end()) {
                for (const auto& event : packIt->second) {
                    if (event.name == name) {
                        SendSoundEvent(event);
                        return;
                    }
                }
            }
        }
    }

    void SoundManager::StopCurrentSoundEvent() {
        if (this->currentHandle.has_value()) { this->currentHandle->Stop(); }
        this->currentHandle.reset();
        this->currentSoundEvent.reset();
    }

    bool SoundManager::IsSoundEventPlaying() const {
        return this->currentHandle.has_value() && this->currentHandle->IsPlaying();
    }

    void SoundManager::SendSoundEvent(const SoundEvent& event) {
        std::thread([this, event]() {
            const std::string cooldownKey = std::format("{}.cooldown", event.name);
            if (const auto storage = SaveDataStorage::GetSingleton()) {
                if (!storage->Get(cooldownKey).AsInt()) {
                    if (this->currentSoundEvent.has_value() && std::ranges::find(currentSoundEvent->overrideBlacklist, event.name) != currentSoundEvent->overrideBlacklist.end()) {
                        ConditionManager::GetSingleton()->UnregisterConditions(event.name);
                        return;
                    }
                    if (Util::RandomInt(0, 99) >= event.chance) {
                        ConditionManager::GetSingleton()->UnregisterConditions(event.name);
                        return;
                    }
                    for (int i = 0; i < event.audios.size(); ++i) {
                        const auto expr = event.audios[i].first;
                        Logger::LogDebug("1");
                        if (const auto files = event.audios[i].second; !files.empty() && ConditionManager::GetSingleton()->EvaluateExpression(event.name, expr)) {
                            ConditionManager::GetSingleton()->UnregisterConditions(event.name);
                            this->StopCurrentSoundEvent();
                            while (this->currentHandle.has_value() || this->currentSoundEvent.has_value()) {
                                std::this_thread::sleep_for(std::chrono::milliseconds(2));
                            }
                            std::string filePath;
                            const auto rngMode = MemoryDataStorage::GetSingleton()->Get("rngMode", DataValue("default"));
                            if (rngMode.AsString() == "biased") {
                                static std::map<std::string, std::vector<std::string>> availableFiles;
                                auto& fileList = availableFiles[std::format("{}|{}", event.name, i)];
                                if (fileList.empty()) {
                                    fileList = files;
                                    std::ranges::shuffle(fileList, std::mt19937{std::random_device{}()});
                                }
                                const std::string s = fileList.front();
                                filePath = std::format("Sound/PlayerVoiceFramework/SoundData/{}", s);
                                if (fileList.size() == 1) {
                                    fileList = files;
                                    int i1 = 0;
                                    do {
                                        std::ranges::shuffle(fileList, std::mt19937{std::random_device{}()});
                                        i1++;
                                    } while (i1 < 10 && fileList.size() > 1 && fileList.front() == s);
                                } else {
                                    fileList.erase(fileList.begin());
                                }
                            } else {
                                filePath = std::format("Sound/PlayerVoiceFramework/SoundData/{}", files.at(Util::RandomInt(0, static_cast<int>(files.size()) - 1)));
                            }
                            RE::BSSoundHandle handle;
                            RE::BSResource::ID id;
                            id.GenerateFromPath(filePath.c_str());
                            RE::BSAudioManager::GetSingleton()->BuildSoundDataFromFile(handle, id, 0x1A, 1);

                            this->currentHandle.emplace(handle);
                            this->currentSoundEvent.emplace(event);
                            this->currentHandle->Play();
                            Logger::LogDebug(std::format("Playing '{}'", filePath));
                            storage->Set(cooldownKey, DataValue(event.cooldown));
                        }
                    }
                }
            }
        }).detach();
    }

    // ReSharper disable once CppDFAEndlessLoop, CppDFAUnreachableCode
    SoundManager::SoundManager() {
        std::thread([this] {
            while (true) {
                if (const auto saveDataStorage = SaveDataStorage::GetSingleton()) {
                    if (const auto activePack = Util::GetActivePack(); !activePack.empty()) {
                        for (auto event : this->registeredSoundEvents[activePack]) {
                            const std::string s = std::format("{}.cooldown", event.name);
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
    }
}