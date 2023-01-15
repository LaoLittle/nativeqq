cmake_minimum_required(VERSION 3.23)
project (nativeqq_core)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(${PROJECT_NAME} ${SOURCES})
add_library(nativeqq::core ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} cppcoro)
target_link_libraries(${PROJECT_NAME} spdlog::spdlog)
target_link_libraries(${PROJECT_NAME} uvw::uvw)
target_link_libraries(${PROJECT_NAME} httplib)
target_include_directories(${PROJECT_NAME} PUBLIC
        ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} nativeqq::tars)
