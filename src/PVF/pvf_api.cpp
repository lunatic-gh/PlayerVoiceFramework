#include "pvf/sound_manager.hpp"
#include "pvf/pack_manager.hpp"
#include "pvf/util.hpp"

#include <pvf/pvf_api.hpp>

namespace pvf {
  uint32_t PlayerVoiceFrameworkAPI::get_api_version() const { return PVF_API_VERSION; }

  void PlayerVoiceFrameworkAPI::send_sound_event(const SoundEvent& event) {
    try {
      const auto event_name = event.name();
      log_info("Received sound-event '{}'", event_name);

      // The Framework's default variables.
      // clang-format off
      std::vector<SoundEvent::SoundEventVariable> event_variables = std::vector{
        SoundEvent::SoundEventVariable("RandomBool", lntc::random_int(0, 1) == 1),
        SoundEvent::SoundEventVariable("RandomPercentage", lntc::random_int<uint32_t>(1, 100)),
        SoundEvent::SoundEventVariable("PlayerHealthPercentage", [] {
          const auto player = RE::PlayerCharacter::GetSingleton()->AsActorValueOwner();
          const float full = player->GetPermanentActorValue(RE::ActorValue::kHealth);
          const float current = player->GetActorValue(RE::ActorValue::kHealth);
          return static_cast<uint32_t>(full > 0.0f ? current / full * 100.0f : 0.0f);
        }()),
        SoundEvent::SoundEventVariable("PlayerStaminaPercentage", [] {
          const auto player = RE::PlayerCharacter::GetSingleton()->AsActorValueOwner();
          const float full = player->GetPermanentActorValue(RE::ActorValue::kStamina);
          const float current = player->GetActorValue(RE::ActorValue::kStamina);
          return static_cast<uint32_t>(full > 0.0f ? current / full * 100.0f : 0.0f);
        }()),
        SoundEvent::SoundEventVariable("PlayerMagickaPercentage", [] {
          const auto player = RE::PlayerCharacter::GetSingleton()->AsActorValueOwner();
          const float full = player->GetPermanentActorValue(RE::ActorValue::kMagicka);
          const float current = player->GetActorValue(RE::ActorValue::kMagicka);
          return static_cast<uint32_t>(full > 0.0f ? current / full * 100.0f : 0.0f);
        }()),
        SoundEvent::SoundEventVariable("PlayerName", RE::PlayerCharacter::GetSingleton()->GetName()),
        SoundEvent::SoundEventVariable("PlayerRace", RE::PlayerCharacter::GetSingleton()->GetRace()->GetLocalFormID()),
        SoundEvent::SoundEventVariable("PlayerSex", RE::PlayerCharacter::GetSingleton()->GetActorBase()->GetSex()),
        SoundEvent::SoundEventVariable("GetEquippedItemLeft", RE::PlayerCharacter::GetSingleton()->GetEquippedObject(true)),
        SoundEvent::SoundEventVariable("GetEquippedItemRight", RE::PlayerCharacter::GetSingleton()->GetEquippedObject(false)),
        SoundEvent::SoundEventVariable("PlayerPositionX", RE::PlayerCharacter::GetSingleton()->GetPositionX()),
        SoundEvent::SoundEventVariable("PlayerPositionY", RE::PlayerCharacter::GetSingleton()->GetPositionY()),
        SoundEvent::SoundEventVariable("PlayerPositionZ", RE::PlayerCharacter::GetSingleton()->GetPositionZ()),
        SoundEvent::SoundEventVariable("PlayerWorldspaceName", [] {
          if (const auto worldspace = RE::PlayerCharacter::GetSingleton()->GetWorldspace()) {
            return worldspace->GetFullName();
          }
          return "";
        }()),
        SoundEvent::SoundEventVariable("PlayerLocationName", [] {
          if (const auto location = RE::PlayerCharacter::GetSingleton()->GetCurrentLocation()) {
            return location->GetFullName();
          }
          return "";
        }()),
        SoundEvent::SoundEventVariable("PlayerLocationKeywords", [] {
          if (const auto location = RE::PlayerCharacter::GetSingleton()->GetCurrentLocation()) {
            return lntc::clib::keywords_to_string(location->GetKeywords()).c_str();
          }
          return "";
        }()),
      };
      // clang-format on
      for (int i = 0; i < event.variables_len(); ++i) event_variables.push_back(event.variables()[i]);

      for (auto var : event_variables) {
        switch (var.type()) {
          case SoundEvent::SoundEventVariable::Type::STRING:
            log_info("Variable '{}' | '{}'", var.name(), var.as_str());
            break;
          case SoundEvent::SoundEventVariable::Type::INT:
            log_info("Variable '{}' | '{}'", var.name(), var.as_int());
            break;
          case SoundEvent::SoundEventVariable::Type::DOUBLE:
            log_info("Variable '{}' | '{}'", var.name(), var.as_double());
            break;
          case SoundEvent::SoundEventVariable::Type::BOOL:
            log_info("Variable '{}' | '{}'", var.name(), var.as_bool());
            break;
        }
      }

      const auto sounds_map = PackManager::get_singleton()->sound_events_for_active_pack();
      if (!sounds_map.contains(event_name)) {
        return;
      }

      const auto sounds = sounds_map.at(event_name);
      if (sounds.empty()) {
        return;
      }

      auto find_sound = [sounds, event_variables]() -> const Sound* {
        for (const auto& sound : sounds) {
          if (sound.conditions().empty()) return &sound;
          for (const auto& condition : sound.conditions()) {
            if (!eval_condition(condition, event_variables)) {
              return nullptr;
            }
          }
          return &sound;
        }
        return nullptr;
      };

      if (const Sound* sound = find_sound()) {
        if (sound->audio_files().empty()) {
          log_info("No audio files.");
          return;
        }
        auto [file_path, file_duration] = sound->audio_files().at(lntc::random_int<int>(0, static_cast<uint32_t>(sound->audio_files().size()) - 1));
        std::thread([event_name, file_path, sound] { SoundManager::get_singleton()->play(event_name, file_path, sound->force_play()); }).detach();
      } else {
        log_info("No Sound...");
      }
    } catch (const std::exception& e) {
      log_error("Error while processing sound event: {}", e.what());
    }
  }
}