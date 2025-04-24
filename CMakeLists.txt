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
FetchContent_Declare(
        yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG origin/master
)
FetchContent_MakeAvailable(yaml-cpp)

add_commonlibsse_plugin(${PROJECT_NAME} SOURCES
        ${headers}
        ${sources}
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

target_precompile_headers(${PROJECT_NAME} PRIVATE PCH.h)
target_compile_definitions(${PROJECT_NAME} PRIVATE PVE_EXPORTS)

target_include_directories(${PROJECT_NAME} PRIVATE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries(${PROJECT_NAME} PRIVATE yaml-cpp)

get_target_property(INC_DIRS ${PROJECT_NAME} INTERFACE_INCLUDE_DIRECTORIES)
if (INC_DIRS)
    list(FILTER INC_DIRS EXCLUDE REGEX "^${CMAKE_SOURCE_DIR}")
    set_target_properties(${PROJECT_NAME} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${INC_DIRS}")
endif ()

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-targets)
install(EXPORT ${PROJECT_NAME}-targets NAMESPACE ${PROJECT_NAME}:: DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
configure_file(cmake/config.cmake.in "${PROJECT_NAME}Config.cmake" @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
install(FILES ${headers} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")