cmake_minimum_required(VERSION 3.10)

project("PlayerVoiceEvents" VERSION 0.8 LANGUAGES CXX)

file(GLOB headers
        "include/*.h"
)

file(GLOB sources
        "src/*.cpp"
)

find_package(CommonLibSSE CONFIG REQUIRED)
add_commonlibsse_plugin(${PROJECT_NAME} SOURCES
        ${headers}
        ${sources}
)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_precompile_headers(${PROJECT_NAME} PRIVATE PCH.h)

include(FetchContent)
FetchContent_Declare(yaml-cpp GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git GIT_TAG origin/master)
FetchContent_MakeAvailable(yaml-cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC
        yaml-cpp::yaml-cpp
)
