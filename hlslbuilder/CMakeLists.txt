cmake_minimum_required(VERSION 3.22)

file(GLOB_RECURSE SM_HDRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "./include/*.hh")
file(GLOB_RECURSE SM_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "./lib/*.cc")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_PREFIX_PATH})

find_package(JsonCpp REQUIRED)
find_package(spdlog REQUIRED)

set(LIB_POSTFIX d)

if(BUILD_SHARED_LIBS)
    set(LIB_NAME ${CMAKE_PROJECT_NAME}_dynamic)
    add_library(${LIB_NAME} SHARED ${SM_HDRS} ${SM_SRCS} )
    target_precompile_headers(${LIB_NAME} PRIVATE ./include/private/smpch.hh)
    target_include_directories(${LIB_NAME} PUBLIC "${CMAKE_PREFIX_PATH}/include" $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/HLSLBuilder> $<INSTALL_INTERFACE:${CMAKE_PREFIX_PATH}/include>)
    target_include_directories(${LIB_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/private")
    target_link_directories(${LIB_NAME} PUBLIC "${CMAKE_PREFIX_PATH}/lib")
    target_link_libraries(${LIB_NAME} PUBLIC jsoncpp_lib spdlog::spdlog)
    target_compile_definitions(${LIB_NAME} PUBLIC HLSLB_DYNAMIC_LINK SPDLOG_WCHAR_TO_UTF8_SUPPORT)
    target_compile_definitions(${LIB_NAME} PRIVATE HLSLB_BUILD_DLL)
    if(WIN32)
        target_compile_definitions(${LIB_NAME} PUBLIC HLSLB_WINDOWS)
        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    endif()

    set_target_properties(${LIB_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_PREFIX_PATH}/bin")
    set_target_properties(${LIB_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_PREFIX_PATH}/bin")
    set_target_properties(${LIB_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_PREFIX_PATH}/bin")
    set_target_properties(${LIB_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_PREFIX_PATH}/lib")
    set_target_properties(${LIB_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_PREFIX_PATH}/lib")
    set_target_properties(${LIB_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_PREFIX_PATH}/lib")
    set_target_properties(${LIB_NAME} PROPERTIES CXX_STANDARD 17)
    set_target_properties(${LIB_NAME} PROPERTIES DEBUG_POSTFIX ${LIB_POSTFIX})
endif()

if(BUILD_STATIC_LIBS OR BUILD_EXECUTABLE)
    set(LIB_NAME ${CMAKE_PROJECT_NAME}_static)
    add_library(${LIB_NAME} STATIC ${SM_HDRS} ${SM_SRCS})
    target_precompile_headers(${LIB_NAME} PRIVATE ./include/private/smpch.hh)
    target_include_directories(${LIB_NAME} PUBLIC "${CMAKE_PREFIX_PATH}/include" $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/HLSLBuilder> $<INSTALL_INTERFACE:${CMAKE_PREFIX_PATH}/include>)
    target_include_directories(${LIB_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/private")
    target_link_directories(${LIB_NAME} PUBLIC "${CMAKE_PREFIX_PATH}/lib")
    target_link_libraries(${LIB_NAME} PUBLIC jsoncpp_static spdlog::spdlog)
    target_compile_definitions(${LIB_NAME} PUBLIC SPDLOG_WCHAR_TO_UTF8_SUPPORT)

    if(WIN32)
        target_compile_definitions(${LIB_NAME} PUBLIC HLSLB_WINDOWS)
    endif()

    set_target_properties(${LIB_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_PREFIX_PATH}/bin")
    set_target_properties(${LIB_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_PREFIX_PATH}/bin")
    set_target_properties(${LIB_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_PREFIX_PATH}/bin")
    set_target_properties(${LIB_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_PREFIX_PATH}/lib")
    set_target_properties(${LIB_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_PREFIX_PATH}/lib")
    set_target_properties(${LIB_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_PREFIX_PATH}/lib")
    set_target_properties(${LIB_NAME} PROPERTIES CXX_STANDARD 17)
    set_target_properties(${LIB_NAME} PROPERTIES DEBUG_POSTFIX ${LIB_POSTFIX})
endif()

file(READ ${CMAKE_CURRENT_SOURCE_DIR}/include/HLSLBuilder/HLSLBuilderVersion.hh.in HEADER_CONTENTS)
string(REGEX REPLACE "CMAKE_VERSION" "${CMAKE_PROJECT_VERSION}" OUTPUT_HEADER ${HEADER_CONTENTS})
file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/include/HLSLBuilder/HLSLBuilderVersion.hh ${OUTPUT_HEADER})

set(HLSL_BUILDER_GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(HLSL_BUILDER_VERSION_CONFIG "${HLSL_BUILDER_GENERATED_DIR}/${PROJECT_NAME}ConfigVersion.cmake")
set(HLSL_BUILDER_PROJECT_CONFIG "${HLSL_BUILDER_GENERATED_DIR}/${PROJECT_NAME}Config.cmake")
set(HLSL_BUILDER_TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")
set(HLSL_BUILDER_CONFIG_INSTALL_DIR "lib/cmake/${PROJECT_NAME}")
set(HLSL_BUILDER_NAMESPACE "${PROJECT_NAME}::")
set(HLSL_BUILDER_VERSION ${PROJECT_VERSION})

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${HLSL_BUILDER_VERSION_CONFIG}" VERSION ${HLSL_BUILDER_VERSION} COMPATIBILITY SameMajorVersion
)
configure_file("${PROJECT_SOURCE_DIR}/cmake_modules/config.cmake.in" "${HLSL_BUILDER_PROJECT_CONFIG}" @ONLY)

# Install cmake config files
install(
    FILES "${HLSL_BUILDER_PROJECT_CONFIG}" "${HLSL_BUILDER_VERSION_CONFIG}"
    DESTINATION "${HLSL_BUILDER_CONFIG_INSTALL_DIR}")
# Install cmake targets files
install(
    EXPORT "${HLSL_BUILDER_TARGETS_EXPORT_NAME}"
    NAMESPACE "${HLSL_BUILDER_NAMESPACE}"
    DESTINATION "${HLSL_BUILDER_CONFIG_INSTALL_DIR}")

install(TARGETS ${CMAKE_PROJECT_NAME}_dynamic ${CMAKE_PROJECT_NAME}_static
        EXPORT ${HLSL_BUILDER_TARGETS_EXPORT_NAME}
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}")

install(
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/HLSLBuilder"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/include"
    FILES_MATCHING PATTERN "*.hh")