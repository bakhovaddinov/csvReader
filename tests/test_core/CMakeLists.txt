cmake_minimum_required(VERSION 3.16)

set(TARGET_NAME test_core)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp)

add_executable(${TARGET_NAME} ${SOURCES})

target_link_libraries(${TARGET_NAME} PRIVATE Catch2::Catch2WithMain core)
