cmake_minimum_required(VERSION 3.25)

project(YSTime
    LANGUAGES CXX)

set(YS_DESIGN_PATTERN_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/submodules/YSDesignPattern)
set(YS_DESIGN_PATTERN_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/YSDesignPattern)
execute_process(COMMAND ${CMAKE_COMMAND} -S ${YS_DESIGN_PATTERN_SOURCE_DIR} -B ${YS_DESIGN_PATTERN_BINARY_DIR} -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX})
execute_process(COMMAND ${CMAKE_COMMAND} --build ${YS_DESIGN_PATTERN_BINARY_DIR} --target install)

set(YS_EVENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/submodules/YSEvent)
set(YS_EVENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/YSEvent)
execute_process(COMMAND ${CMAKE_COMMAND} -S ${YS_EVENT_SOURCE_DIR} -B ${YS_EVENT_BINARY_DIR} -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX})
execute_process(COMMAND ${CMAKE_COMMAND} --build ${YS_EVENT_BINARY_DIR} --target install)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(ysTime ${sources})

target_include_directories(ysTime
    PUBLIC ${CMAKE_INSTALL_PREFIX}/inc)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/inc/
    DESTINATION ${CMAKE_INSTALL_PREFIX}/inc/YSTime)
install(TARGETS ysTime
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/YSTime)