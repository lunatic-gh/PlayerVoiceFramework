#pragma once

namespace PVE {
    class CustomEventListener {
    public:
        static void OnPlayerEnterLocation(std::string locationName);

        static void OnPlayerLeaveLocation(std::string locationName);

        static void Register();
    };
}