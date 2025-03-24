#include "CustomEventListener.h"

#include "Utils.h"
namespace PVE {

    void CustomEventListener::OnPlayerEnterLocation(std::string locationName) {
        Utils::PlaySound(std::format("PVELocationEnter{}", locationName));
    }

    void CustomEventListener::OnPlayerLeaveLocation(std::string locationName) {
        Utils::PlaySound(std::format("PVELocationLeave{}", locationName));
    }


    void CustomEventListener::Register() {
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
}