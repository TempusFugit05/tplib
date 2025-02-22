include(GenerateExportHeader)

# Basic stuff
cmake_minimum_required(VERSION 3.25)
set(PROJECT_NAME "tplib")
set (CMAKE_CXX_STANDARD 11)

project(
    ${PROJECT_NAME}
    VERSION 1.0.0
    DESCRIPTION
        "This is the base library for my projects :)"
    LANGUAGES CXX
)

# Settings for generating test/library.
# option(test "Compile test for the library." OFF)

# if(NOT test)
    add_library(${PROJECT_NAME})
    generate_export_header(
    ${PROJECT_NAME}
    EXPORT_FILE_NAME
    export/${PROJECT_NAME}.hpp
    )

    target_include_directories(
        ${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}/export
    )
# endif(NOT test)

# Setup for OS specific configurations
if (UNIX)
    message(STATUS "Compiling for linux.")
    set(SOURCES_OS_DIR src/linux)
endif(UNIX)
if (WIN32)
    message(STATUS "Compiling for windows.")
    set(SOURCES_OS_DIR src/windows)
endif(WIN32)

# This is a utility function to change a list of items from PLACEHOLDER root directory to src or include.
# This is done since my project has two mirrored directory trees, one for sources, and one for headers.
set(PLACEHOLDER PLACEHOLDER)
function(SET_SOURCE_TYPE TYPE FILE_LIST OUT_LIST) 
    set(MODIFIED_LIST)
    foreach(FILE IN LISTS ${FILE_LIST})
        string(REPLACE "${PLACEHOLDER}" "${TYPE}" FILE ${FILE})
        list(APPEND MODIFIED_LIST "${FILE}")
    endforeach()

    # Update the list
    set(${OUT_LIST} "${MODIFIED_LIST}" PARENT_SCOPE)
endfunction()

# Setting-up file general structure
set(DIR_COMMON "${PLACEHOLDER}/common")
set(DIR_STRING "${DIR_COMMON}/string")
set(DIR_IO "${DIR_COMMON}/io")

set(DIRS_AGNOSTIC_BASE
    ${DIR_COMMON}
    ${DIR_STRING}
    ${DIR_IO}
)

# Create header directory path
set(DIRS_HEADERS_AGNOSTIC)
set_source_type("include" DIRS_AGNOSTIC_BASE DIRS_HEADERS_AGNOSTIC)

# Source files
    # Unsorted sources
set(SOURCES_COMMON 
    math.cpp
)
list(TRANSFORM SOURCES_COMMON PREPEND "${DIR_COMMON}/")

    # IO sources
set(SOURCES_IO
    console_io.cpp
    io.cpp
)
list(TRANSFORM SOURCES_IO PREPEND "${DIR_IO}/")

    # String sources
set(SOURCES_STRING
    format_int.cpp
    manipulation.cpp
    format.cpp
    scan.cpp
)
list(TRANSFORM SOURCES_STRING PREPEND "${DIR_STRING}/")

    # Combine sources
set(SOURCES_AGNOSTIC
    ${SOURCES_COMMON}
    ${SOURCES_IO}
    ${SOURCES_STRING}
)

# OS-specific sources
set(SOURCES_OS)
file(GLOB_RECURSE SOURCES_OS ${SOURCES_OS_DIR}/*.cpp)

set(SOURCES_BASE
    ${SOURCES_AGNOSTIC}
    ${SOURCES_OS}
)
set(SOURCES)
set_source_type("src" SOURCES_BASE SOURCES)

# OS-Agnostic headers
    # Uncategorized Headers
set(HEADERS_COMMON
    system.h
    math.h
    macro_utils.h
    macro_helpers.h
)
list(TRANSFORM HEADERS_COMMON PREPEND "${DIR_COMMON}/")

    # String Headers
set(HEADERS_STRING
    tp_string.h
    manipulation.h
    format.h
    scan.h
)

list(TRANSFORM HEADERS_STRING PREPEND "${DIR_STRING}/")

    # Io Headers
set(HEADERS_IO 
    io.h
    file.h
)

list(TRANSFORM HEADERS_IO PREPEND "${DIR_IO}/")

    # Combine OS-Agnostic Headers
set(HEADERS_BASE
    ${HEADERS_COMMON}
    ${HEADERS_STRING}
    ${HEADERS_IO}
)
set(HEADERS_AGNOSTIC)
set_source_type("include" HEADERS_BASE HEADERS_AGNOSTIC)

# OS-Sepecific Headers
set(HEADERS_OS_DIR include/system)
set(HEADERS_OS system_io.h system_files.h system_params.h)
list(TRANSFORM HEADERS_OS PREPEND "${HEADERS_OS_DIR}/")

# Combining all the headers
set(HEADERS ${HEADERS_AGNOSTIC} ${HEADERS_OS})
set(HEADERS_DIR ${DIRS_HEADERS_AGNOSTIC} ${HEADERS_OS_DIR})

# if(test)
#     message(STATUS "Compiling test!")
#     add_executable(${PROJECT_NAME} ${SOURCES} test/main.cpp ${HEADERS})
# endif(test)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})

target_compile_definitions(${PROJECT_NAME} PRIVATE DEV_DEBUG)

target_include_directories(${PROJECT_NAME} PUBLIC define)
target_include_directories(${PROJECT_NAME} PUBLIC ${HEADERS_DIR})

target_compile_options(${PROJECT_NAME} PRIVATE 
    -fvisibility-inlines-hidden
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -fno-exceptions
    -fno-rtti
    # -fsanitize=address
    -g3
)
target_link_options(${PROJECT_NAME} PRIVATE
    # -fsanitize=address
)
