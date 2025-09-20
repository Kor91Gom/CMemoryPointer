cmake_minimum_required(VERSION 3.10)
project(CMemoryPointerPlatform C)

add_library(cmemplatform SHARED src/CMemPlatform.c)

target_include_directories(cmemplatform PUBLIC include)

# 플랫폼별 정의
if(WIN32)
    target_compile_definitions(cmemplatform PRIVATE PLATFORM_WINDOWS)
elseif(APPLE)
    target_compile_definitions(cmemplatform PRIVATE PLATFORM_APPLE)
elseif(ANDROID)
    target_compile_definitions(cmemplatform PRIVATE PLATFORM_ANDROID)
elseif(UNIX)
    target_compile_definitions(cmemplatform PRIVATE PLATFORM_UNIX)
endif()
