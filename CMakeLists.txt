cmake_minimum_required(VERSION 3.21)

project("PlayerVoiceEvents" VERSION 1.0 LANGUAGES CXX)

set(OUTPUT_FOLDER "$ENV{SKYRIM_MODS_DIR_DEV}/${PROJECT_NAME}")

set(CMAKE_BUILD_TYPE RelWithDebInfo)

set(sources
        src/Plugin.cpp
        src/Main.cpp
        src/PapyrusFunctions.cpp
        src/EventListener.cpp
        src/Utils.cpp
)

find_package(CommonLibSSE CONFIG REQUIRED)
add_commonlibsse_plugin(${PROJECT_NAME} SOURCES ${sources})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_precompile_headers(${PROJECT_NAME} PRIVATE PCH.h)

if (DEFINED OUTPUT_FOLDER)
    set(DLL_FOLDER "${OUTPUT_FOLDER}/SKSE/Plugins")

    message(STATUS "SKSE plugin output folder: ${DLL_FOLDER}")

    add_custom_command(
            TARGET "${PROJECT_NAME}"
            POST_BUILD
            COMMAND echo Copying Plugin-DLL...
            COMMAND "${CMAKE_COMMAND}" -E make_directory "${DLL_FOLDER}"
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:${PROJECT_NAME}>" "${DLL_FOLDER}/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
            VERBATIM
    )

    add_custom_command(
            TARGET "${PROJECT_NAME}"
            POST_BUILD
            COMMAND echo Copying Plugin-PDB...
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_PDB_FILE:${PROJECT_NAME}>" "${DLL_FOLDER}/$<TARGET_PDB_FILE_NAME:${PROJECT_NAME}>"
            VERBATIM
    )
    add_custom_command(TARGET "${PROJECT_NAME}" POST_BUILD
            COMMAND echo Setting up Pyro...
            COMMAND setup-pyro.bat
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
    add_custom_command(TARGET "${PROJECT_NAME}" POST_BUILD
            COMMAND echo Compiling Papyrus-Scripts...
            COMMAND pyro/pyro.exe --input-path skyrimse.ppj --game-path "C:/Game-Dev/Skyrim Special Edition"
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
    add_custom_command(TARGET "${PROJECT_NAME}" POST_BUILD
            COMMAND echo Creating Release...
            COMMAND create-release.bat
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endif ()
