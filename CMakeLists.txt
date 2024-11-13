cmake_minimum_required(VERSION 3.27.5)

project(game VERSION 0.1.0)

option(DEBUG_MODE "If toggled ON - logs messages to opened console or file. See logger.h for logger usage" ON)
option(COPY_ASSETS_TO_EXECUTABLE_DIR "If toggled ON - assets directory will be copied next to game executable" ON)
option(BUILD_COMMAND_LINE_APP "IF toggled ON - command line app will be compiled according to its own CMakeLists.txt" OFF)

set(SDL2_DIR "${CMAKE_SOURCE_DIR}/lib/SDL2-2.28.5/cmake")
set(SDL2_image_DIR "${CMAKE_SOURCE_DIR}/lib/SDL2_image-2.8.2/cmake")
set(SDL2_ttf_DIR "${CMAKE_SOURCE_DIR}/lib/SDL2_ttf-2.22.0/cmake")
set(SDL2_net_DIR "${CMAKE_SOURCE_DIR}/lib/SDL2_net-2.2.0/cmake")

if (DEBUG_MODE)
    add_definitions(-D_DEBUG)
endif()

if (COPY_ASSETS_TO_EXECUTABLE_DIR)
    file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
endif()

if (BUILD_COMMAND_LINE_APP)
    include_directories(commandline-app)
endif()

file(GLOB PROJECT_SRC "src/*.cpp")


find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_net REQUIRED)

add_executable(game WIN32 ${PROJECT_SRC})

set_target_properties(game PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")

target_link_libraries(game SDL2::SDL2main SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)
target_include_directories(game PUBLIC "${CMAKE_SOURCE_DIR}/include")