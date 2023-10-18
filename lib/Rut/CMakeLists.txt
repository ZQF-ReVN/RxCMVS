# Library Name
set(PROJECT_NAME Rut)

# Cpp Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set Source File Path
list(APPEND SRC_FILE 
    "${CMAKE_CURRENT_SOURCE_DIR}/Platform/Win32/File.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/Platform/Win32/Path.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/Platform/Win32/String.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/Platform/Win32/System.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/RxBench/Record.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/RxPath/Enum.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/RxPath/Path.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/RxConsole/Alloc.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/RxConsole/Put.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/RxFile/Basic.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/RxFile/Binary.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/RxFile/Text.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/RxMem/Auto.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/RxStr/RxStr.cpp")

# Set Library
add_library(${PROJECT_NAME} "${SRC_FILE}")
