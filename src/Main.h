#pragma once
#include "LoopManager.h"

namespace PVE {
    inline LoopManager loopManager;
    class Main {
    public:
        static void Init();

        static void Load();
    };
}
