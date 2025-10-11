#pragma once

// Disable some stupid compiler warnings
#pragma warning(disable : 4100) // Unreferenced parameter
#pragma warning(disable : 4101) // Unreferenced local variable
#pragma warning(disable : 4189) // local variable X is initialized but not referenced

#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>

namespace fs = std::filesystem;
using namespace std::literals;

#include <pvf/logger.hpp>
using namespace pvf;