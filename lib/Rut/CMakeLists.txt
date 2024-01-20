# Library Name
set(PROJECT_NAME Rut)

# Cpp Standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set Source File Path
list(APPEND SRC_FILE 
    "${CMAKE_CURRENT_SOURCE_DIR}/RxSys/Win32/File.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/RxSys/Win32/Path.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/RxSys/Win32/String.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/RxSys/Win32/System.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/RxBench.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/RxCmd.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/RxPath.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/RxFile.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/RxMem.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/RxINI.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/RxJson.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/RxStr.cpp" )

# Set Library
add_library(${PROJECT_NAME} "${SRC_FILE}")
