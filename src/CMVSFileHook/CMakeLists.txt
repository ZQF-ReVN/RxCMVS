# Value Define
set(PROJECT_NAME CMVSFileHook)

# Cpp Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add Library
link_libraries(CMVSTools RxINI)

# Current Project
project("${PROJECT_NAME}")
add_library(${PROJECT_NAME} MODULE "dllmain.cpp")
