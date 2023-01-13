cmake_minimum_required(VERSION 3.23)
project (nativeqq_tars)

set(CMAKE_CXX_STANDARD 23)

aux_source_directory(src DIR_SRCS)

add_library(${PROJECT_NAME} ${DIR_SRCS})
add_library(nativeqq::tars ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} nativeqq::protocol)


