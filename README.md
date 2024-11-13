# C++ SDL2 Cmake easy start

##### This tutorial should provide you easy start for C++ SDL2 game development if you are not familiar or not very good with Cmake. Here you can find explained template for game project with CMakeLists.txt to build it. I also use VS Code, so there will be tips for it. And don't miss troubleshooting section.

There are not much newbie-friendly material about this topic, so i'll try to fill this gap.

For now it works only on windows 64 bit, i'll maybe make it cross-platform in future. See TODO section.

### Table of contents

- [Introduction](#introduction)
- [CMakeLists.txt](#cmakeliststxt)
- [Command line app](#command-line-application-with-sdl2)
- [Logger](#logger)
- [VS Code tools](#vs-code-tools)
- [Troubleshooting](#troubleshooting)
- [.gitignore](#gitignore)
- [Great learn sources](#great-sources-and-further-reading)
- [TODO](#todo)


## Introduction

**Cmake** - is a tool, that helps you genereate commands for compilers based on provided configuration (CMakeLists.txt) and saves your time, recompiling only changed files.

**CMakeLists.txt** - configuration file for **CMake**. Also cmake understands files with .cmake extesion, but as main file it expects CMakeLists.txt. .cmake files are commonly used in libraries to add files in build, linker e.t.c.

**Make** - is a tool, on which **CMake** is based. The difference is that make requeres more hand work, along with writing all shell commands to be run. config file for make named Makefile.

**Cache** - pack of files: make files, pre-compiled files, other stuff, that used by **CMake** to control changes and build all correctly.

**Target** - think of it as a task. One may make obj file out of main.cpp, other link libraries, e.t.c. Most tasks requre some other tasks to be ran before. Main task set with .PHONY in **make** syntax.

##### How Cmake works?
It, firstly, generates **cache**, which holds states of your files and Makefile. Then builds targets, specified in Makefile. In result you get executable (or maybe library).
To generate cache run
```shell
cmake ./cmake
```
where ./cmake is place to store cache.
To build - run
```shell
cmake --build .
```
from your project directory.

## CMakeLists.txt

Lets go through Cmake configuration file and learn how it works.

```Cmake
cmake_minimum_required(VERSION 3.27.5)
```

This line contains minimal version of Cmake, that is able to work with files. Don't mind about it much, you can leave and it will work just fine. Of course Cmake installed on your machine should have version 3.27.5 or higher. This line required.

```Cmake
project(game VERSION 0.1.0)
```

As you guessed, this line describes your project in format ```project(project_name VERSION project_version)```. This line also required.

```Cmake
option(DEBUG_MODE "If toggled ON - logs messages to opened console or file. See logger.h for logger usage" ON)
option(COPY_ASSETS_TO_EXECUTABLE_DIR "If toggled ON - assets directory will be copied next to game executable" ON)
option(BUILD_CLIENT_APP "IF toggled ON - client app will be compiled according to its own CMakeLists.txt" OFF)
```

Next you'll see options. I added them for fast change, they have explanation and two options "ON" and "OFF".
First one will fulfill ```#ifdef _DEBUG #endif``` and run code inside it if set "ON".
Second option may come handy if your game contains relative paths. It will copy "assets" folder next to place, where final executable will be placed.
Third one is option to compile also command-line application, you may want to build. (I need it to use SDL objects in game server, that shouldn't have GUI). You can read more in [this section](#command-line-application-with-sdl2). And if you consider using VS Code, there is [even easier way](#vs-code-tools) to set build and debug targets.

```Cmake
set(SDL2_DIR "${CMAKE_SOURCE_DIR}/lib/SDL2-2.28.5/cmake")
set(SDL2_image_DIR "${CMAKE_SOURCE_DIR}/lib/SDL2_image-2.8.2/cmake")
set(SDL2_ttf_DIR "${CMAKE_SOURCE_DIR}/lib/SDL2_ttf-2.22.0/cmake")
set(SDL2_net_DIR "${CMAKE_SOURCE_DIR}/lib/SDL2_net-2.2.0/cmake")
```

These lines set location of SDL2 libraries, we will use variables ```SDL2_DIR```, ```SDL2_image_DIR``` later. My decision about storing libraries all stuff in project may be not what you want, its better not to load all these files to github, or store next to project. Most common approach - is to install needed libraries somewhere and add paths to them to system variables. If so, you'll no longer need to set ```SDL2_DIR``` and others, Cmake could find them. But I wanted to have out of the box working dev set, so my friend doesn't need to install anything and we surely will have same version. Also every added library will be pulled from github repo. If you have all libraries installed on your machine - just remove these lines.

```Cmake
if (DEBUG_MODE)
    add_definitions(-D_DEBUG)
endif()
```

So, this code checks if DEBUG_MODE variable, which we set in option section equals ON, and if so it adds ```-D_DEBUG``` flag. Note: if you want add flag named ```MY_FLAG``` you need to pass it with -D prefix: ```-DMY_FLAG```.

```Cmake
if (COPY_ASSETS_TO_EXECUTABLE_DIR)
    file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
endif()
```

Same as previous, file() instruction will work if ```COPY_ASSETS_TO_EXECUTABLE_DIR``` is set ON. Command inside copies assets folder to place, where executable will be put. This path is hold in ```CMAKE_BINARY_DIR``` variable. &{} thing "dereferences" real path stored in variable.

```Cmake
if (BUILD_COMMAND_LINE_APP)
    include_directories(commandline-app)
endif()
```

Now we say, that we want to add commanline-app directory for Cmake to work with. It contains its own instructios in its own CMakeLists.txt.

```Cmake
file(GLOB PROJECT_SRC "src/*.cpp")
```

That line takes all files that match "src/*.cpp" regex (* means any amount of any symbols) and stores them in ```PROJECT_SRC``` variable.

```Cmake
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_net REQUIRED)
```

Now we search for libraries SDL2, SDL2_image e.t.c. Cmake will search for them in system paths, or, if didn't found - in variables named ```library_name_DIR```. There is place where we need variables ```SDL2_DIR```, ```SDL2_image_DIR``` e.t.c. that we set before. The word ```REQUIRED``` tells CMake, that we absolutly need these libraries in order to sucsessfully build our game.

```Cmake
add_executable(game WIN32 ${PROJECT_SRC})
```

Command ```add_executable()``` tells CMake, that it needs to build (as you guessed) our game executable. first argument - executable name. Second, ```WIN32``` says that our game will be GUI windows application. Third - adds all source files to compile them. If you remember - ```PROJECT_SRC``` variable holds all files that ends with .cpp and located in src directory.

```Cmake
set_target_properties(game PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")
```

That one line says, that target named "game" sould be placed in directory "${CMAKE_SOURCE_DIR}/build". ```CMAKE_SOURCE_DIR``` holds path to CMakeLists.txt directory. If you have nested CMakeLists, ```CMAKE_SOURCE_DIR``` will always hold main config path. if you want path to current executed CMakeLists - use ```CMAKE_CURRENT_SOURCE_DIR``` instead.

```Cmake
target_link_libraries(game SDL2::SDL2main SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)
```

That line links all passed libraries to our game execurable. ```SDL2::SDL2main```, especcially, is needed to set SDL2main as entry point. Only after your main() will be called.

```Cmake
target_include_directories(game PUBLIC "${CMAKE_SOURCE_DIR}/include")
```

Last line also adds our headers to linker, which are located in include directory.

Now all things set, run
```shell
cmake ./cache
```
to generate cache and
```shell
cmake --build .
```
to build game.

Here you are, if code is correct, game.exe will appear in build directory with assets. Also, don't forget to add needed .dll files next to executable, because our game is dynamicly compiled.

## Command line application with SDL2

At one point you may need an SDL application without GUI, and there are several things we need to change to achieve this. Example presented in commandline-app directory.

1) Remove SDL2::SDL2main from target_link_libraries().
```Cmake
target_link_libraries(server SDL2::SDL2)
```
2) Explicitly specify executable type - windows command line application
```Cmake
target_link_options(server PRIVATE -mconsole)
```
3) Set SDL_MAIN_HANDLED preprocessor flag in main.cpp, which says, that we manually work with sdl main
```cpp
#define SDL_MAIN_HANDLED

int main() {
    ...
}
```
Now we don't nessecerely need argc - argv for main(), we can remove them.

Note, that now we need to replace ```CMAKE_SOURCE_DIR``` to ```CMAKE_CURRENT_SOURCE_DIR``` in several places, so cmake works in directory where file located, not in top directory.

## Logger

Also I added logger, which I use for debugging, io. GUI apps can't cout, cin, cerr, so i made a logger singleton, that allocates new windows console and resets std in, out and err to it. (By default SDL sets them to dev/null, discards). You can use it as always std::cout or with logger instance to have pretty out or log in file. Also I set alias for easy use:
```cpp
#define logger Logger::getInstance(LMode_Console)
```

It works only on Windows for now, because it uses windows API (as expected).
Logger is documented, so you can read code directly.

## VS Code tools

For now, please see [official cmake in VS Code setup page](https://code.visualstudio.com/docs/cpp/CMake-linux)
Quick start with images.

Or [VS Code cmake tool documentation](https://github.com/microsoft/vscode-cmake-tools/blob/main/docs/README.md)
This documentations covers features of extensions, also contains images - very user-friendly.

These extensions will automate routine and help you focus on code, rather typing commands and learning Cmake.

## .gitignore

Don't forget to add all cache to .gitignore, it contains a lot of info and trash, that you don't want to appear on your github repo. You can use github's cmake gitignore template, or add these lines:

```gitignore
build/*
!build/SDL2.dll
!build/SDL2_image.dll
!build/SDL2_ttf.dll

cmake/

.vscode/
```

First 4 lines delete everything in build folder except dll files

remove folder with cache

and in case you use VS Code - add .vscode/

## Troubleshooting

Cmake doesn't track every possible change, so recreating cache may solve your problem. For example, if it can't find newly added library.

Not much, sorry.

## Great sources and further reading

- [VS Code Cmake quick start tutorial](https://code.visualstudio.com/docs/cpp/CMake-linux)
Official VS Code quick start with Cmake. This tutorial is made for linux, but appliable for windows.
- [SDL tutorials list](https://wiki.libsdl.org/SDL2/Tutorials)
Tutorials and materiales, that were selected by SDL team.
- [SDL2 Tutorial series](https://youtube.com/playlist?list=PLUFd3gYWwiYFdSoqZp35RU6mja4N5u4dz)
Youtube tutorial series of SDL2 basics.
- [SDL2 Simple Directmedia Library](https://youtube.com/playlist?list=PLvv0ScY6vfd-p1gSnbQhY7vMe2rng0IL0&si=c6x4DYHXvskAoV-3)
Youtube playlist of video covering more specific topics.
- [Cmake, How it Works (At Three Different Levels)](https://www.youtube.com/watch?v=SDX0oYqdv_g)
Fast video about how Cmake works.
- [Installing MinGW to build C++ Code on Windows](https://www.youtube.com/watch?v=oC69vlWofJQ)
VS Code youtube video, that covers mingw (gcc) installation on windows.
- [C++ in VS Code: Building your Code with CMake](https://www.youtube.com/watch?v=_BWU5mWqVA4)
VS Code youtube video, but this covers work with Cmake in VS Code.
- [Cmake getting started](https://cmake.org/getting-started/)
List of materials for easy start with Cmake by Cmake team.


## TODO

This part contains updates, that I want to add in future. But if you want - you can help me and contribute to repo, covering these topics:

- Change CMakeLists to compile on linux machines.
- Change CMakeLists to compile based on word size (32 bit or 64 bit).
- Add static linking. 
- Update logger to work on linux machines.
- Add automated libraries download from repos.
- Better troubleshooting approaches.
- Add more great sources.

## Afterwords

Im not a C++ or CMake guru, so my code or configs could contain mistakes or bad approaches. Please make an issue if you see any or have suggestions. :eyes: