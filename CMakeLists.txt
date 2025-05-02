# Minimum CMake Version
cmake_minimum_required(VERSION 3.10)

# Ignore some Warnings
set(ignoreMe "${CMAKE_TOOLCHAIN_FILE}")

# Set Version
set(VERSION 1.0.5)

# Define Project Data
project("PlayerVoiceEvents" VERSION ${VERSION} LANGUAGES CXX)

# Make dynamic vcpkg.json
configure_file("${PROJECT_SOURCE_DIR}/vcpkg.json.in" "${PROJECT_SOURCE_DIR}/vcpkg.json" @ONLY)

# Define Header Files (.h)
set(headers
        include/PVE/Logger.h
        include/PVE/PapyrusInterface.h
        include/PVE/Util.h
        include/PVE/EventHandler.h
        include/PVE/ConditionManager.h
        include/PVE/MemoryDataStorage.h
        include/PVE/SaveDataStorage.h
        include/PVE/SoundManager.h
        include/PVE/DF_API.h
        include/PVE/API.h
)

# Define Source Files (.cpp)
set(sources
        src/PVE/Plugin.cpp
        src/PVE/EventHandler.cpp
        src/PVE/SoundManager.cpp
        src/PVE/Util.cpp
        src/PVE/API.cpp
)

# Dependencies
find_package(CommonLibSSE CONFIG REQUIRED)
include(FetchContent)
FetchContent_Declare(yaml-cpp GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git GIT_TAG origin/master)
FetchContent_MakeAvailable(yaml-cpp)

# Create CLib Plugin
add_commonlibsse_plugin(${PROJECT_NAME} SOURCES ${headers} ${sources})

# Define C++ Version as 23
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

# Precompile Header
target_precompile_headers(${PROJECT_NAME} PRIVATE PCH.h)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE yaml-cpp)

# Add export definitions, so the API-Functions get exported.
target_compile_definitions(${PROJECT_NAME} PRIVATE PVE_EXPORTS)
