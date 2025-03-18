#include "CustomEventListener.h"

namespace PVE {

    void CustomEventListener::OnPlayerEnterLocation(std::string locationName) {
        Utils::PlaySound(std::format("PVELocationEnter{}", locationName));
    }
    void CustomEventListener::OnPlayerLeaveLocation(std::string locationName) {
        Utils::PlaySound(std::format("PVELocationLeave{}", locationName));
    }
}