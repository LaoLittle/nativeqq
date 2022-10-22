cmake_minimum_required(VERSION 3.23)
project (nativeqq_core)

set(CMAKE_CXX_STANDARD 23)

find_package(uvw CONFIG REQUIRED)
find_path(CPP_HTTPLIB_INCLUDE_DIRS "httplib.h")

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(${PROJECT_NAME} ${SOURCES})
add_library(nativeqq::core ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} uvw::uvw)
target_include_directories(${PROJECT_NAME} PUBLIC ${CPP_HTTPLIB_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC
        ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} nativeqq::tars)
target_link_options(${PROJECT_NAME} PRIVATE -static)
