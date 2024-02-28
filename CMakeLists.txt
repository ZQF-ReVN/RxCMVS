# Main CMakeLists
cmake_minimum_required(VERSION 3.20)

# Project Name
project(CMVS_Tools)

# Option
set(UVAR_BUILD_TEST TRUE)
set(UVAR_IPO_OPTIMIZATION TRUE)
set(UVAR_STATIC_MSVC_RUNTIME TRUE)

# Value Define
set(ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}")

# Out Path
if (CMAKE_SIZEOF_VOID_P EQUAL 4)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(EXECUTABLE_OUTPUT_PATH "${ROOT_DIR}/.bin/x32/debug")
        set(LIBRARY_OUTPUT_PATH "${ROOT_DIR}/.bin/x32/debug")
    else()
        set(EXECUTABLE_OUTPUT_PATH "${ROOT_DIR}/.bin/x32/release")
        set(LIBRARY_OUTPUT_PATH "${ROOT_DIR}/.bin/x32/release")
    endif()
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(EXECUTABLE_OUTPUT_PATH "${ROOT_DIR}/.bin/x64/debug")
        set(LIBRARY_OUTPUT_PATH "${ROOT_DIR}/.bin/x64/debug")
    else()
        set(EXECUTABLE_OUTPUT_PATH "${ROOT_DIR}/.bin/x64/release")
        set(LIBRARY_OUTPUT_PATH "${ROOT_DIR}/.bin/x64/release")
    endif()
endif()

# IPO
if(UVAR_IPO_OPTIMIZATION)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT IPO_CHECK_RESULT OUTPUT output)
    if(IPO_CHECK_RESULT)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO TRUE)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_MINSIZEREL TRUE)
    else()
        message(WARNING "IPO is not supported: ${output}")
    endif()
endif()

# MSVC Setting
if(MSVC)
    # Runtime
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        if (UVAR_STATIC_MSVC_RUNTIME)
            set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreadedDebug)
        else()
            set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreadedDebugDLL)
        endif()
    else()
        if (UVAR_STATIC_MSVC_RUNTIME)
            set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded)
        else()
            set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreadedDLL)
        endif()
    endif()

    # Macro
    add_definitions(-DUNICODE -D_UNICODE)

    # Compile Flags
    if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(/Gy)
        add_compile_options(/Zc:inline)
    endif()
endif()

# Subdirectory
add_subdirectory("lib")
add_subdirectory("src")

# Test
if(UVAR_BUILD_TEST)
    # add_subdirectory("test")
endif()
