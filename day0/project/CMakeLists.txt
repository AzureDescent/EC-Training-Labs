cmake_minimum_required(VERSION 3.16)
project(day0_git_lab CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()

add_library(day0_core
        app/control.cpp
        app/motor_monitor.cpp
        app/clamp.cpp
        base/angle.cpp
)
target_include_directories(day0_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_subdirectory(tests)
