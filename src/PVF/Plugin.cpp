#include <pvf/pvf_api.hpp>
#include <pvf/configuration.hpp>
#include <pvf/sound_manager.hpp>
#include <pvf/pack_manager.hpp>
#include <pvf/papyrus_interface.hpp>

SKSEPluginLoad(const SKSE::LoadInterface* intfc) {
  SKSE::Init(intfc);
  init_logger();
  std::this_thread::sleep_for(500ms); // Without this, the logger won't be ready early enough. No idea why...
  pvf_api::pvf_api = new pvf_api::PlayerVoiceFrameworkAPI;
  log_info("Registered SKSE-API.");

  PapyrusInterface::init();
  log_info("Registered Papyrus-API.");

  if (Configuration::get_singleton()->load()) {
    log_info("Loaded Configuration.");
  } else {
    log_error("Failed to load Configuration.");
  }

  if (PackManager::get_singleton()->load()) {
    log_info("Loaded Voice-Pack Data.");
  } else {
    log_error("Failed to load Voice-Pack Data.");
  }

  SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message* a_msg) {
    switch (a_msg->type) {
      case SKSE::MessagingInterface::kDataLoaded: {
        break;
      }
      case SKSE::MessagingInterface::kPostLoadGame: {
        break;
      }
    }
  });

  SKSE::GetMessagingInterface()->RegisterListener(nullptr, [](SKSE::MessagingInterface::Message* message) {
    if (!message) return;
    switch (message->type) {
      case PVF_API_KEY:
        log_info("Received API-Load Request from '{}'", message->sender);
        message->dataLen = sizeof(pvf_api::PlayerVoiceFrameworkAPI*);
        *static_cast<pvf_api::PlayerVoiceFrameworkAPI**>(message->data) = pvf_api::pvf_api;
        break;
      default:
        break;
    }
  });
  return true;
}