#pragma once
#include "LoopManager.h"

namespace PVE {

    static LoopManager loopManager;
    class Main {
    public:
        static void Init();

        static void Load();
    };
}
