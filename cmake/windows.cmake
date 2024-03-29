add_definitions(
    -DCOMPILER_MSVC -DCOMPILER_MSVC64 -D_WIN32 -D_WINDOWS -D_ALLOW_KEYWORD_MACROS -D__STDC_LIMIT_MACROS
    -D_CRT_SECURE_NO_WARNINGS=1 -D_CRT_SECURE_NO_DEPRECATE=1 -D_CRT_NONSTDC_NO_DEPRECATE=1
)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4819 /wd4828 /wd5033 /permissive- /utf-8 /wd4005")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /OPT:REF /OPT:ICF")

set(CS2SDK_LINK_LIBRARIES
        ${SOURCESDK_LIB}/public/win64/tier0.lib
        ${SOURCESDK_LIB}/public/win64/tier1.lib
        ${SOURCESDK_LIB}/public/win64/interfaces.lib
        #${SOURCESDK_LIB}/public/win64/mathlib.lib
        ${SOURCESDK_LIB}/public/win64/2015/libprotobuf.lib
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
        CS2SDK_PLATFORM="windows"
        CS2SDK_BINARY="win64"
        CS2SDK_ROOT_BINARY="/bin/win64/"
        CS2SDK_GAME_BINARY="/csgo/bin/win64/"
)

set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")