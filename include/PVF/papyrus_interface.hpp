#pragma once

#include <RE/I/IVirtualMachine.h>
#include <SKSE/API.h>
#include <pvf/logger.hpp>

class PapyrusInterface {
public:
  static void init() {
    if (!SKSE::GetPapyrusInterface()->Register([](RE::BSScript::IVirtualMachine* vm) -> bool {
          try {
            // vm->RegisterFunction("FNCT", "CLASS", FNCT);
            // ...
            return true;
          } catch (...) {
            return false;
          }
        })) {
      log_error("Failed to register Papyrus-API.");
    }
  }
};