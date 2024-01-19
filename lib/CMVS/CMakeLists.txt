# Library Name
set(PROJECT_NAME CMVS)

# Cpp Standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add Library
link_libraries(Rut RxHook)
include_directories("${THIRD_DIR}/")
link_libraries("${THIRD_DIR}/libwebp/lib.X86/libwebp.lib")

# Set Source File Path
list(APPEND SRC_FILE 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/MGV_Editor.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/PS3_Cryptor.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/PS3_TextEditor.cpp" )

# Source File Only x32
if (CMAKE_SIZEOF_VOID_P EQUAL 4)
list(APPEND SRC_FILE     
    "${CMAKE_CURRENT_SOURCE_DIR}/src/VFS_Hook.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/VFS_Extract.cpp" )
endif()

# Set Library
add_library(${PROJECT_NAME} "${SRC_FILE}")
