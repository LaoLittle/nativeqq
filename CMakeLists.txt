cmake_minimum_required(VERSION 3.23)
project (nativeqq)

set(CMAKE_CXX_STANDARD 23)

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    add_definitions(-D _WIN_PLATFORM_)
    add_definitions(-D UNICODE)

    link_libraries(ws2_32 wsock32)
endif()

add_subdirectory(core)
add_subdirectory(tars)
add_subdirectory(protocol)

add_executable(nativeqq main.cpp)

target_link_libraries(${PROJECT_NAME} nativeqq::tars)
target_link_libraries(${PROJECT_NAME} nativeqq::protocol)
target_link_libraries(${PROJECT_NAME} nativeqq::core)

target_link_options(${PROJECT_NAME} PRIVATE -static)


message(STATUS ${PROJECT_NAME})
