cmake_minimum_required(VERSION 3.25)

project(YSTime
        LANGUAGES CXX)

set(target_name YSTime)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.cpp)
add_library(${target_name} ${sources})

add_subdirectory(submodules/YSDesignPattern)
add_subdirectory(submodules/YSEvent)

target_include_directories(${target_name} PUBLIC inc/)

install(TARGETS ${target_name}
        DESTINATION ${CMAKE_SOURCE_DIR}/lib/)
if (NOT ${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/inc
            DESTINATION ${CMAKE_SOURCE_DIR}/inc/)
endif()