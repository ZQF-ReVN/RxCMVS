# Project
project(CMVS)

# Source File Path
list(APPEND "${PROJECT_NAME}_SRC_FILE" 
    "src/CMVS/MGV_Editor.cpp" 
    "src/CMVS/PS3_Cryptor.cpp" 
    "src/CMVS/PS3_TextEditor.cpp" )

# Source File Only x32
if (CMAKE_SIZEOF_VOID_P EQUAL 4)
list(APPEND "${PROJECT_NAME}_SRC_FILE"     
    "src/CMVS/VFS_Hook.cpp" 
    "src/CMVS/VFS_Extract.cpp" )
endif()

# Set Library
add_library("${PROJECT_NAME}" "${${PROJECT_NAME}_SRC_FILE}")
target_include_directories("${PROJECT_NAME}" INTERFACE "src")
target_compile_features("${PROJECT_NAME}" PRIVATE cxx_std_23)
target_compile_definitions("${PROJECT_NAME}" PRIVATE UNICODE _UNICODE)
target_compile_options("${PROJECT_NAME}" PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")

# Add Library
target_link_libraries("${PROJECT_NAME}" PRIVATE Rut RxHook)
target_link_libraries("${PROJECT_NAME}" PRIVATE "${ROOT_DIR}/third/libwebp/lib.X86/libwebp.lib")