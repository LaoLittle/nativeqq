cmake_minimum_required(VERSION 3.23)
project (nativeqq_protocol)

set(CMAKE_CXX_STANDARD 23)

#find_package(Protobuf CONFIG REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(${PROJECT_NAME} ${SOURCES})
add_library(nativeqq::protocol ALIAS ${PROJECT_NAME})

#target_link_libraries(${PROJECT_NAME} protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)
target_include_directories(${PROJECT_NAME} PUBLIC
        ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} nativeqq::tars)

