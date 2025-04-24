cmake_minimum_required(VERSION 3.10)

project("PlayerVoiceEvents" VERSION 0.8 LANGUAGES CXX)

set(headers
        include/PVE.h
        include/Logger.h
        include/PapyrusInterface.h
        include/Util.h
        include/EventHandler.h
        include/ConditionManager.h
        include/MemoryDataStorage.h
        include/SaveDataStorage.h
        include/SoundManager.h
)

set(sources
        src/Plugin.cpp
        src/EventHandler.cpp
        src/SoundManager.cpp
        src/Util.cpp
)

find_package(CommonLibSSE CONFIG REQUIRED)

include(FetchContent)
FetchContent_Declare(yaml-cpp GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git GIT_TAG origin/master)
FetchContent_MakeAvailable(yaml-cpp)

add_commonlibsse_plugin(${PROJECT_NAME} SOURCES
        ${headers}
        ${sources}
)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_precompile_headers(${PROJECT_NAME} PRIVATE PCH.h)

target_link_libraries(${PROJECT_NAME} PRIVATE yaml-cpp)