cmake_minimum_required(VERSION 3.25)

project(TestForYSTime
        LANGUAGES CXX)

set(target_name TestForYSTime)

add_subdirectory(../ ../build/YSTime)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS *.cpp)
add_executable(${target_name} ${sources})

target_link_libraries(${target_name} YSTime)

if (MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${target_name})
endif ()