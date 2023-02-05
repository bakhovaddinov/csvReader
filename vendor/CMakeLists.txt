cmake_minimum_required(VERSION 3.16)

macro(AddExternalProject MACRO_PROJECT_NAME VERSION GIT_REPOSITORY GIT_TAG)

message(STATUS "Trying to configure ${MACRO_PROJECT_NAME} dependency")
find_package(${MACRO_PROJECT_NAME} ${VERSION} QUIET)

if (NOT ${MACRO_PROJECT_NAME}_FOUND)
    message(STATUS "Could not find ${MACRO_PROJECT_NAME}, trying to download...")
    FetchContent_Declare(
            ${MACRO_PROJECT_NAME}
            GIT_REPOSITORY ${GIT_REPOSITORY}
            GIT_TAG        ${GIT_TAG}
    )

    FetchContent_MakeAvailable(${MACRO_PROJECT_NAME})
endif()

message(STATUS "Configured ${MACRO_PROJECT_NAME}")
endmacro()

include(FetchContent)
# AddExternalProject(spdlog 1.11.0 https://github.com/gabime/spdlog v1.11.0)
# AddExternalProject(fmt 9.1.0 https://github.com/fmtlib/fmt 9.1.0)
