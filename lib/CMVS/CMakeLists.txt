# Library Name
set(PROJECT_NAME CMVS)

# Cpp Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add Library
link_libraries(Rut RxHook)
include_directories("${THIRD_DIR}/")
link_libraries("${THIRD_DIR}/libwebp/lib.X86/libwebp.lib")

# Set Source File Path
list(APPEND SRC_FILE 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/VFS_Hook.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/VFS_Extract.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/MGV_Editor.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/PS3_Coder.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/PS3_Editor.cpp" )

# Set Library
add_library(${PROJECT_NAME} "${SRC_FILE}")
