cmake_minimum_required(VERSION 3.25)

set(PROJECT_NAME "tplib")

if (UNIX)
    message(STATUS "Compiling for linux.")
    set(SOURCES_OS_DIR src/linux)
    set(HEADERS_OS_DIR include/linux)
endif(UNIX)
if (WIN32)
    message(STATUS "Compiling for windows.")
    set(SOURCES_OS_DIR src/windows)
    set(HEADERS_OS_DIR include/windows)
endif(WIN32)


# Source files
    # Common sources
set(SOURCES_COMMON_DIR src/common)
set(SOURCES_COMMON io.cpp math.cpp)
list(TRANSFORM SOURCES_COMMON PREPEND "${SOURCES_COMMON_DIR}/")

    # OS-specific sources
set(SOURCES_OS system.cpp system_io.cpp)
list(TRANSFORM SOURCES_OS PREPEND "${SOURCES_OS_DIR}/")

set(SOURCES ${SOURCES_COMMON} ${SOURCES_OS})
set(SOURCES_DIR ${SOURCES_COMMON_DIR} ${SOURCES_OS_DIR})


# Header files
    # Common headers
set(HEADERS_COMMON_DIR include/common)
set(HEADERS_COMMON io.h string_utils.h system_io.h system.h math.h)
list(TRANSFORM HEADERS_COMMON PREPEND "${HEADERS_COMMON_DIR}/")

    # OS-sepecific headers
set(HEADERS_OS system_params.h)
list(TRANSFORM HEADERS_OS PREPEND "${HEADERS_OS_DIR}/")

set(HEADERS ${HEADERS_COMMON} ${HEADERS_OS})
set(HEADERS_DIR ${HEADERS_COMMON_DIR} ${HEADERS_OS_DIR})


# Project settings
project(${PROJECT_NAME})
add_executable(${PROJECT_NAME} ${SOURCES} main.cpp ${HEADERS})

set (CMAKE_CXX_STANDARD 98)

target_include_directories(${PROJECT_NAME} PUBLIC define)
target_compile_definitions(${PROJECT_NAME} PUBLIC DEV_DEBUG)

target_include_directories(${PROJECT_NAME} PRIVATE ${HEADERS_DIR})

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -fno-exceptions
    -fno-rtti
    -fsanitize=address
    -g3
)
target_link_options(${PROJECT_NAME} PRIVATE
    -fsanitize=address
)
