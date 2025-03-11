#pragma once
#include <string>

namespace PVE {
    struct PVEPlayerCellChangedEvent {
    public:
        // members
        std::string oldCell; // 00
        std::string newContainer; // 04
    };

    static_assert(sizeof(TESContainerChangedEvent) == 0x18);
}
