# x32 Only
if (NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
	return()
endif()

# Project Name
project(VFS_Hook)

# Current Project
add_library("${PROJECT_NAME}" MODULE "dllmain.cpp")
target_compile_features("${PROJECT_NAME}" PRIVATE cxx_std_23)
target_compile_definitions("${PROJECT_NAME}" PRIVATE UNICODE _UNICODE)
target_compile_options("${PROJECT_NAME}" PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")

# Add Library
target_link_libraries("${PROJECT_NAME}" PRIVATE Rut RxHook CMVS)