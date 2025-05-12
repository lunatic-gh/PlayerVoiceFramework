# Minimum CMake Version
cmake_minimum_required(VERSION 3.10)

# Ignore some Warnings
set(ignoreMe "${CMAKE_TOOLCHAIN_FILE}")

# Set Version
set(VERSION 1.0.0)

# Define Project Data
project("PlayerVoiceFramework" VERSION ${VERSION} LANGUAGES CXX)

# Make dynamic vcpkg.json
configure_file("${PROJECT_SOURCE_DIR}/vcpkg.json.in" "${PROJECT_SOURCE_DIR}/vcpkg.json" @ONLY)

# Define Header Files (.h)
set(headers
        include/PVF/Logger.h
        include/PVF/PapyrusInterface.h
        include/PVF/Util.h
        include/PVF/ConditionManager.h
        include/PVF/MemoryDataStorage.h
        include/PVF/SaveDataStorage.h
        include/PVF/SoundManager.h
        include/PVF/API.h
)

# Define Source Files (.cpp)
set(sources
        src/PVF/Plugin.cpp
        src/PVF/Logger.cpp
        src/PVF/PapyrusInterface.cpp
        src/PVF/Util.cpp
        src/PVF/ConditionManager.cpp
        src/PVF/MemoryDataStorage.cpp
        src/PVF/SaveDataStorage.cpp
        src/PVF/SoundManager.cpp
        src/PVF/API.cpp
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
target_link_libraries(${PROJECT_NAME} PUBLIC yaml-cpp)

# Add export definitions, so the API-Functions get exported.
target_compile_definitions(${PROJECT_NAME} PRIVATE PVF_EXPORTS)
