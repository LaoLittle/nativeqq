cmake_minimum_required(VERSION 3.23)
project (nativeqq)

set(CMAKE_CXX_STANDARD 23)

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    add_definitions(-D _WIN_PLATFORM_)
    add_definitions(-D UNICODE)

    link_libraries(ws2_32 wsock32)
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

add_subdirectory(core)
add_subdirectory(tars)
add_subdirectory(protocol)

add_executable(nativeqq main.cpp)

target_link_libraries(${PROJECT_NAME} nativeqq::tars)
target_link_libraries(${PROJECT_NAME} nativeqq::protocol)
target_link_libraries(${PROJECT_NAME} nativeqq::core)

MESSAGE("Load project ${PROJECT_NAME}")
