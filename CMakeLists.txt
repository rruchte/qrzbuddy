cmake_minimum_required(VERSION 3.5)

project(qrzbuddy LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    # Use dynamic CRT everywhere (matches Qt + Poco + OpenSSL):
    #   Debug        -> /MDd (MultiThreadedDLLDebug)
    #   everything else (Release, RelWithDebInfo, MinSizeRel) -> /MD
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLLDebug")
    else()
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
    endif()
endif()

#enable_testing()

add_subdirectory(src)
#add_subdirectory(test)