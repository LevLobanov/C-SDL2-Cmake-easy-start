cmake_minimum_required(VERSION 3.27.5)

project(game_server VERSION 0.1.0)

set(SDL2_DIR "${CMAKE_SOURCE_DIR}/lib/SDL2-2.28.5/cmake")
set(SDL2_net_DIR "${CMAKE_SOURCE_DIR}/lib/SDL2_net-2.2.0/cmake")

file(GLOB PROJECT_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

find_package(SDL2_net REQUIRED)
find_package(SDL2 REQUIRED)

add_executable(server ${PROJECT_SRC})

set_target_properties(server PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")
target_link_options(server PRIVATE -mconsole)

target_link_libraries(server SDL2::SDL2 SDL2_net::SDL2_net)
target_include_directories(server PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")