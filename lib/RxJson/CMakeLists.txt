# Library Name
set(PROJECT_NAME RxJson)

# Cpp Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set Source File Path
list(APPEND SRC_FILE 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Parser.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Value.cpp" 
)

# Set Current Library
add_library(${PROJECT_NAME} "${SRC_FILE}")
