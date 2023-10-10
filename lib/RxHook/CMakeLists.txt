# Library Name
set(PROJECT_NAME RxHook)

# Cpp Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add Library
include_directories("${THIRD_DIR}/detours/include/")
link_libraries("${THIRD_DIR}/detours/lib.X86/detours.lib")

# Set Source File Path
list(APPEND SRC_FILE 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/API.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Mem.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Hook.cpp" 
)

# Set Current Library
add_library(${PROJECT_NAME} "${SRC_FILE}")
