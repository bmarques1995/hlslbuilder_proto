cmake_minimum_required(VERSION 3.22)

project(hlslbuilder VERSION 0.0.1)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

option(BUILD_SHARED_LIBS "Build smake as DLL" ON)
option(BUILD_STATIC_LIBS "Build smake as Static Lib" ON)
option(BUILD_EXECUTABLE "Build the current executable" ON)

add_subdirectory(hlslbuilder)
add_subdirectory(hlslbuilder_exec)
add_subdirectory(hlslbuilder_tests)