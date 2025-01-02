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
set(SOURCES_OS
    system.cpp
    system_io.cpp
)
list(TRANSFORM SOURCES_OS PREPEND "${SOURCES_OS_DIR}/")

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
)
list(TRANSFORM HEADERS_COMMON PREPEND "${DIR_COMMON}/")

    # String Headers
set(HEADERS_STRING string.h manipulation.h format.h scan.h)
list(TRANSFORM HEADERS_STRING PREPEND "${DIR_STRING}/")

    # Io Headers
set(HEADERS_IO io.h file.h)
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
set(HEADERS_OS system_io.h system_files.h system_params.h)
list(TRANSFORM HEADERS_OS PREPEND "${HEADERS_OS_DIR}/")

# Combining all the headers
set(HEADERS ${HEADERS_AGNOSTIC} ${HEADERS_OS})
set(HEADERS_DIR ${DIRS_HEADERS_AGNOSTIC} ${HEADERS_OS_DIR})

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
