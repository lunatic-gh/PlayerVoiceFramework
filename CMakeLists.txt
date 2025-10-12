cmake_minimum_required(VERSION 3.31)
project(PlayerVoiceFramework VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE HEADERS "${PROJECT_SOURCE_DIR}/include/*.hpp")
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

find_package(CommonLibSSE CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)

add_commonlibsse_plugin(${PROJECT_NAME} SOURCES ${HEADERS} ${SOURCES})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

target_precompile_headers(${PROJECT_NAME} PRIVATE pch.hpp)

target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/include")

target_link_libraries(${PROJECT_NAME} PUBLIC nlohmann_json::nlohmann_json yaml-cpp::yaml-cpp CommonLibSSE::CommonLibSSE)

##############
# Deployment #
##############
set(DEPLOY_PATH_BASE "$ENV{MO2_DEV_MODS_PATH}")
if (NOT DEFINED DEPLOY_PATH_BASE OR NOT EXISTS "${DEPLOY_PATH_BASE}")
    set(DEPLOY_PATH "${PROJECT_SOURCE_DIR}/deploy")
else ()
    set(DEPLOY_PATH "${DEPLOY_PATH_BASE}/${PROJECT_NAME}")
endif ()

if (EXISTS "${DEPLOY_PATH}/..")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND powershell -Command "echo '---------------------------------------'"
            COMMAND powershell -Command "echo 'Deploying to ''${DEPLOY_PATH}'''"
            # Clean up the old deploy dir if exists
            COMMAND powershell -Command "echo '  Cleaning up old data...'"
            COMMAND ${CMAKE_COMMAND} -E rm -rf "${DEPLOY_PATH}"
            # Create a new deploy dir
            COMMAND powershell -Command "echo '  Creating Directories'"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${DEPLOY_PATH}/SKSE/Plugins"
            # Copy relevant files into the deploy dir
            COMMAND powershell -Command "echo '  Copying DLL...'"
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.dll" "${DEPLOY_PATH}/SKSE/Plugins"
            COMMAND powershell -Command "echo '  Copying Data...'"
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/Data" "${DEPLOY_PATH}"
            COMMAND powershell -Command "echo '  Compiling ^& Copying Papyrus Scripts...'"
            COMMAND "${CMAKE_SOURCE_DIR}\\compile_papyrus.bat" "${CMAKE_SOURCE_DIR}\\Data\\Scripts\\Source" "${DEPLOY_PATH}\\Scripts"
            COMMAND powershell -Command "echo '  Copying README...'"
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/README.md" "${DEPLOY_PATH}/README.md"
            COMMAND powershell -Command "echo '  Copying LICENSES...'"
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/LICENSE" "${DEPLOY_PATH}/LICENSE"
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/THIRD_PARTY_LICENSES" "${DEPLOY_PATH}/THIRD_PARTY_LICENSES"
    )
endif ()

if (EXISTS "${DEPLOY_PATH}/.." AND (${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo" OR ${CMAKE_BUILD_TYPE} STREQUAL "Debug"))
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND powershell -Command "echo '  Copying PDB...'"
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pdb" "${DEPLOY_PATH}/SKSE/Plugins"
    )
endif ()