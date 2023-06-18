cmake_minimum_required(VERSION 3.25)

project(YSTimeTest
    LANGUAGES CXX)

execute_process(COMMAND ${CMAKE_COMMAND} -S ./.. -B ./../build -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX})
execute_process(COMMAND ${CMAKE_COMMAND} --build ./../build --target install)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(ysTimeTest main.cpp)

if (MSVC)
    file(GLOB_RECURSE libraries CONFIGURE_DEPENDS ${CMAKE_INSTALL_PREFIX}/lib/*.lib)
else()
    file(GLOB_RECURSE libraries CONFIGURE_DEPENDS ${CMAKE_INSTALL_PREFIX}/lib/*.a)
endif()

target_include_directories(ysTimeTest
    PUBLIC ${CMAKE_INSTALL_PREFIX}/inc)
target_link_libraries(ysTimeTest
    PUBLIC ${libraries})

if (MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ysTimeTest)
endif ()