cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

if(POLICY CMP0091)
	cmake_policy(SET CMP0091 NEW)
endif()

if(POLICY CMP0092)
	cmake_policy(SET CMP0092 NEW) # Don't add -W3 warning level by default.
endif()

project(plugify-source-2
	VERSION 1.0.0.0
	DESCRIPTION "CS2 SDK Plugin"
	HOMEPAGE_URL "https://github.com/untrustedmodders/plugify-source-2"
	LANGUAGES C CXX
)

option(CS2SDK_BUILD_DOCS "Enable building with documentation." OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

string(REGEX REPLACE "^[^-]*-" "" PROJECT_NAME_SUBSTRING "${PROJECT_NAME}")
string(TOLOWER "${PROJECT_NAME_SUBSTRING}" PROJECT_NAME_LOWER)
string(TOUPPER "${PROJECT_NAME_SUBSTRING}" PROJECT_NAME_UPPER)

set(PROJECT_OUTPUT_NAME "cs2sdk")

set(PROJECT_AUTHOR "Untrusted Modders Team")
set(PROJECT_DESCRIPTION_FULL "Source 2 SDK Plugify plugin")
set(PROJECT_LICENSE "GPLv3")
string(TIMESTAMP PROJECT_BUILD_DATE "%Y-%m-%d")
string(TIMESTAMP PROJECT_BUILD_TIME "%H:%M:%S")

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(SYMBOLS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sym")
set(EXTERNAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external")
set(EXPORTS_BINARY_DIR "${CMAKE_BINARY_DIR}/exports")

function(set_or_external_dir VAR_NAME TARGET)
	if(${VAR_NAME})
		file(TO_CMAKE_PATH "${${VAR_NAME}}" ${VAR_NAME})
	else()
		set(${VAR_NAME} "${EXTERNAL_DIR}/${TARGET}")
	endif()

	set(${VAR_NAME} "${${VAR_NAME}}" PARENT_SCOPE)
endfunction()

set_or_external_dir(DYNLIBUTILS_DIR "dynlibutils")
set_or_external_dir(LOGGER_DIR "logger")
set_or_external_dir(METAMOD_DIR "metamod-source")
set_or_external_dir(PLUGIFY_DIR "plugify")
set_or_external_dir(SOURCESDK_DIR "sourcesdk")

include(GenerateExportHeader)

if(NOT CS2SDK_BUILD_DOCS)
	include(cmake/platform/shared.cmake)
	include(cmake/compat_format.cmake)
	include(cmake/dynlibutils.cmake)
	include(cmake/logger.cmake)
	include(cmake/metamod.cmake)
	include(cmake/plugify.cmake)
	include(cmake/sourcesdk.cmake)
endif()

set(COMPILE_DEFINITIONS
	CS2SDK_LOGGING_PREFIX="cs2-sdk"
	CS2SDK_PLATFORM_WINDOWS=$<BOOL:${WIN32}>
	CS2SDK_PLATFORM_APPLE=$<BOOL:${APPLE}>
	CS2SDK_PLATFORM_LINUX=$<BOOL:${LINUX}>
	CS2SDK_LIBRARY_SUFFIX="${CMAKE_SHARED_LIBRARY_SUFFIX}"
	CS2SDK_LIBRARY_PREFIX="${CMAKE_SHARED_LIBRARY_PREFIX}"
	CS2SDK_PLATFORM="linux"
	CS2SDK_BINARY="linuxsteamrt64"
	CS2SDK_ROOT_BINARY="/bin/linuxsteamrt64/"
	CS2SDK_GAME_BINARY="/bin/linuxsteamrt64/../../csgo/bin/linuxsteamrt64/" # Metamod hack
)

file(GLOB_RECURSE SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp")

set(INCLUDE_DIRS
	${EXPORTS_BINARY_DIR}
	${SOURCE_DIR}
)

set(PRECOMPILER_HEADERS
	${SOURCE_DIR}/pch.h
)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${METAMOD_SOURCESDK_SOURCE_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
	C_STANDARD 11
	C_STANDARD_REQUIRED ON
	C_EXTENSIONS OFF

	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_OUTPUT_NAME})

if(WINDOWS)
	set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
elseif(MACOS)
	set_target_properties(${PROJECT_NAME} PROPERTIES OSX_ARCHITECTURES "x86_64")
endif()

target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILER_OPTIONS} ${SOURCESDK_COMPILE_OPTIONS} ${PLUGIFY_COMPILE_OPTIONS})

generate_export_header(${PROJECT_NAME} EXPORT_MACRO_NAME PLUGIN_API EXPORT_FILE_NAME ${EXPORTS_BINARY_DIR}/plugin_export.h)
target_precompile_headers(${PROJECT_NAME} PRIVATE ${PRECOMPILER_HEADERS})

target_compile_definitions(${PROJECT_NAME} PRIVATE ${COMPILE_DEFINITIONS} ${METAMOD_COMPILE_DEFINITIONS} ${PLUGIFY_COMPILE_DEFINITIONS})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS} ${DYNLIBUTILS_INCLUDE_DIRS} ${METAMOD_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE ${DYNLIBUTILS_BINARY_DIR} ${PLUGIFY_LINK_LIBRARIES} ${SOURCESDK_BINARY_DIR})

configure_file(
	${CMAKE_SOURCE_DIR}/cs2sdk.pplugin.in
	${CMAKE_BINARY_DIR}/cs2sdk.pplugin
)

configure_file(${CMAKE_SOURCE_DIR}/assets/configs/core.txt ${CMAKE_BINARY_DIR}/core.txt COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/assets/gamedata/cs2sdk.games.txt ${CMAKE_BINARY_DIR}/cs2sdk.games.txt COPYONLY)

# ------------------------------------------------------------------------------
# Documentation
if(CS2SDK_BUILD_DOCS)
    find_package(Doxygen REQUIRED)

    if(DOXYGEN_FOUND)
        add_subdirectory(docs)
    endif()
endif()
