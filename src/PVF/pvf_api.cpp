#include <pvf/pvf_api.hpp>

#include <lntc/lunatic_utils.hpp>
#include <pvf/pack_manager.hpp>
#include <pvf/sound_manager.hpp>

namespace pvf_api {
  uint32_t PlayerVoiceFrameworkAPI::api_version() const { return 1; }

  void PlayerVoiceFrameworkAPI::send_sound_event(const SoundEvent* event) {
    if (event) {
      try {
        const std::string event_name = std::string(event->name().c_str());
        log_info("Received sound-event '{}'", event_name);

        // The Framework's default variables.
        // clang-format off
        std::vector<SoundEvent::Variable*> event_variables = std::vector{
          new SoundEvent::Variable("RandomBool", lntc::random_int(0, 1) == 1),
          new SoundEvent::Variable("RandomPercentage", lntc::random_int<uint32_t>(1, 100)),
          new SoundEvent::Variable("PlayerHealthPercentage", [] {
            const auto player = RE::PlayerCharacter::GetSingleton()->AsActorValueOwner();
            const float full = player->GetPermanentActorValue(RE::ActorValue::kHealth);
            const float current = player->GetActorValue(RE::ActorValue::kHealth);
            return static_cast<uint32_t>(full > 0.0f ? current / full * 100.0f : 0.0f);
          }()),
          new SoundEvent::Variable("PlayerStaminaPercentage", [] {
            const auto player = RE::PlayerCharacter::GetSingleton()->AsActorValueOwner();
            const float full = player->GetPermanentActorValue(RE::ActorValue::kStamina);
            const float current = player->GetActorValue(RE::ActorValue::kStamina);
            return static_cast<uint32_t>(full > 0.0f ? current / full * 100.0f : 0.0f);
          }()),
          new SoundEvent::Variable("PlayerMagickaPercentage", [] {
            const auto player = RE::PlayerCharacter::GetSingleton()->AsActorValueOwner();
            const float full = player->GetPermanentActorValue(RE::ActorValue::kMagicka);
            const float current = player->GetActorValue(RE::ActorValue::kMagicka);
            return static_cast<uint32_t>(full > 0.0f ? current / full * 100.0f : 0.0f);
          }()),
          new SoundEvent::Variable("PlayerName", RE::PlayerCharacter::GetSingleton()->GetName()),
          new SoundEvent::Variable("PlayerRace", RE::PlayerCharacter::GetSingleton()->GetRace()->GetLocalFormID()),
          new SoundEvent::Variable("PlayerSex", RE::PlayerCharacter::GetSingleton()->GetActorBase()->GetSex()),
          new SoundEvent::Variable("GetEquippedItemLeft", RE::PlayerCharacter::GetSingleton()->GetEquippedObject(true)),
          new SoundEvent::Variable("GetEquippedItemRight", RE::PlayerCharacter::GetSingleton()->GetEquippedObject(false)),
          new SoundEvent::Variable("PlayerPositionX", RE::PlayerCharacter::GetSingleton()->GetPositionX()),
          new SoundEvent::Variable("PlayerPositionY", RE::PlayerCharacter::GetSingleton()->GetPositionY()),
          new SoundEvent::Variable("PlayerPositionZ", RE::PlayerCharacter::GetSingleton()->GetPositionZ()),
          new SoundEvent::Variable("PlayerWorldspaceName", [] {
            if (const auto worldspace = RE::PlayerCharacter::GetSingleton()->GetWorldspace()) {
              return worldspace->GetFullName();
            }
            return "";
          }()),
          new SoundEvent::Variable("PlayerLocationName", [] {
            if (const auto location = RE::PlayerCharacter::GetSingleton()->GetCurrentLocation()) {
              return location->GetFullName();
            }
            return "";
          }()),
          new SoundEvent::Variable("PlayerLocationKeywords", [] {
            if (const auto location = RE::PlayerCharacter::GetSingleton()->GetCurrentLocation()) {
              return lntc::clib::keywords_to_string(location->GetKeywords()).c_str();
            }
            return "";
          }()),
        };
        // clang-format on

        for (auto variable : event->variables()) event_variables.push_back(variable);

        for (const auto var : event_variables) {
          switch (var->type()) {
            case SoundEvent::Variable::Type::STRING:
              log_info("Variable '{}' | {}", var->name().c_str(), var->as_str().c_str());
              break;
            case SoundEvent::Variable::Type::INT:
              log_info("Variable '{}' | {}", var->name().c_str(), var->as_int());
              break;
            case SoundEvent::Variable::Type::FLOAT:
              log_info("Variable '{}' | {}", var->name().c_str(), var->as_float());
              break;
            case SoundEvent::Variable::Type::BOOL:
              log_info("Variable '{}' | {}", var->name().c_str(), var->as_bool());
              break;
          }
        }

        if (const auto sounds_map = PackManager::get_singleton()->sound_events_for_active_pack(); sounds_map.contains(event_name)) {
          if (const auto sounds = sounds_map.at(event_name); !sounds.empty()) {
            auto find_sound = [sounds, event_variables]() -> const std::optional<Sound> {
              for (const auto& sound : sounds) {
                if (sound.conditions().empty()) return sound;
                for (const auto& condition : sound.conditions()) {
                  if (!eval_condition(condition, event_variables)) {
                    return std::nullopt;
                  }
                }
                return sound;
              }
              return std::nullopt;
            };

            if (const auto sound = find_sound()) {
              if (!sound->audio_files().empty()) {
                auto [file_path, file_duration] = sound->audio_files().at(lntc::random_int<int>(0, static_cast<uint32_t>(sound->audio_files().size()) - 1));
                std::thread([event_name, file_path, sound] { SoundManager::get_singleton()->play(event_name, file_path, sound->force_play()); }).detach();
              }
            }
          }
        }
        for (auto var : event_variables) {
          delete var;
        }
      } catch (const std::exception& e) {
        log_error("Error while processing sound event: {}", e.what());
      }
      delete event;
    }
  }
}