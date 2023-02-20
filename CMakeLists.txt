cmake_minimum_required(VERSION 3.25)

project(YSTime
        LANGUAGES CXX)

set(target_name YSTime)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.cpp)
add_library(${target_name} ${sources})

add_subdirectory(submodules/YSDesignPattern)
add_subdirectory(submodules/YSEvent)

target_include_directories(${target_name} PUBLIC inc/)