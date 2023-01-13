cmake_minimum_required(VERSION 3.23)
project (nativeqq)

set(CMAKE_CXX_STANDARD 23)

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    add_definitions(-D _WIN_PLATFORM_)
    add_definitions(-D UNICODE)

    link_libraries(ws2_32 wsock32)
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag(-fcoroutines COMPILER_HAS_F_COROUTINES)
    if (COMPILER_HAS_F_COROUTINES)
        message(STATUS
                "GCC has support for C++ coroutines, setting flag for Folly build."
        )
        add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-fcoroutines>)
    else()
        message(STATUS
                "GCC does not have support for C++ coroutines, "
                "disabling Folly coroutine support."
        )
    endif()
endif()


set(BUILD_UVW_SHARED_LIB OFF)
set(BUILD_UVW_LIBS ON)
set(BUILD_TESTING OFF)
set(USE_LIBCPP OFF)
set(BUILD_CPPCORO_SHARED OFF)
set(SPDLOG_BUILD_SHARED OFF)

# You need to manually clone the repository from github.
# 请手动从Github克隆仓库
add_subdirectory(library/spdlog)
add_subdirectory(library/uvw)
add_subdirectory(library/cpp-httplib)
add_subdirectory(library/cppcoro)

add_subdirectory(core)
add_subdirectory(tars)
add_subdirectory(protocol)

add_executable(nativeqq main.cpp)

target_link_libraries(${PROJECT_NAME} nativeqq::tars)
target_link_libraries(${PROJECT_NAME} nativeqq::protocol)
target_link_libraries(${PROJECT_NAME} nativeqq::core)

MESSAGE("Load project ${PROJECT_NAME}")
