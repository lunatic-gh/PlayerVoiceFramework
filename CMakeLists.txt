cmake_minimum_required(VERSION 3.10)

project("PlayerVoiceEvents" VERSION 0.8 LANGUAGES CXX)

file(GLOB headers
    "include/*.h"
)

file(GLOB sources
    "src/*.cpp"
)

find_package(CommonLibSSE CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
add_commonlibsse_plugin(${PROJECT_NAME} SOURCES ${headers} ${sources})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_precompile_headers(${PROJECT_NAME} PRIVATE PCH.h)

target_link_libraries(${PROJECT_NAME} PRIVATE yaml-cpp)