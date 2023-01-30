cmake_minimum_required(VERSION 3.16)

set(TARGET_NAME core)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp)

add_library(${TARGET_NAME} STATIC ${SOURCES})

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
