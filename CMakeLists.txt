cmake_minimum_required(VERSION 3.16)
project(RayLibTemplate)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
file(GLOB SOURCES src/*.cpp)
add_executable(RayLibTemplate WIN32 src/darkstyle.h ${SOURCES} libs/cmrc.hpp)
# Setting parameters for raylib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # or games

if(UNIX)
    # Add file textension *.bin to all executables on Unix (Linux, OSX, BSD ...)
    set(CMAKE_EXECUTABLE_SUFFIX ".bin")
endif()


# ------------ Download CmakeRC Resource compiler ----------------#
file(DOWNLOAD "https://raw.githubusercontent.com/vector-of-bool/cmrc/master/CMakeRC.cmake"
        "${CMAKE_BINARY_DIR}/CMakeRC.cmake")
include("${CMAKE_BINARY_DIR}/CMakeRC.cmake")
cmrc_add_resource_library(
        skins

        NAMESPACE skns
        "skins/audio interQT1.png"
        skins/pauseRec.png
        skins/asset.png
        skins/playRec.png
        skins/resumeRec.png
        skins/Buildd.png
        skins/settin.png
)


#========== Targets Configurations ============#

add_subdirectory(libs/raylib-4.1 ./libs/raylib)
target_link_libraries(RayLibTemplate PRIVATE raylib skins)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")