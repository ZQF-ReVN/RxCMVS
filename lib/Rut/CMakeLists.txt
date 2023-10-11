# Library Name
set(PROJECT_NAME Rut)

# Cpp Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set Source File Path
list(APPEND SRC_FILE 
    "${CMAKE_CURRENT_SOURCE_DIR}/RxBench/Record.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/RxPath/Enum.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/RxPath/Path.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/RxConsole/Alloc.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/RxConsole/Put.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/RxFS/Basic.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/RxFS/Text.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/RxMem/Auto.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/RxCvt/Cvt.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/RxStr/Trim.cpp")

# Set Library
add_library(${PROJECT_NAME} "${SRC_FILE}")
