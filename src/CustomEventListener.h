#pragma once
#include "Main.h"

namespace PVE {
    class CustomEventListener {
    public:
        static void OnPlayerEnterLocation(std::string locationName);

        static void OnPlayerLeaveLocation(std::string locationName);

        static void Register() {
            loopManager.StartNew("OnPlayerChangeLocation", [] {
                for (auto loc : locations) {
                    const int i = Utils::QueryLocationChange(loc);
                    if (i == 2) {
                        OnPlayerEnterLocation(std::get<0>(loc));
                        break;
                    }
                    if (i == 1) {
                        OnPlayerLeaveLocation(std::get<0>(loc));
                        break;
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            });
        }
    };
}